
#include "Database.h"
#include "Configuration.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <unordered_set>

#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

// Protect simultaneous updates inside this application.
/*
A mutex (mutual exclusion) is a synchronization mechanism that prevents multiple threads from accessing a protected section of code simultaneously.

Imagine two administrators trying to update the database at the same time:
- Administrator 1 wants to assign train TR001 to track T001.
- Administrator 2 wants to assign train TR002 to track T002.
Without synchronization, both operations might read the same original YAML file and then write different versions of it.

One update could overwrite the other.

This is known as a race condition.

The mutex helps prevent this within one running application.
*/
static std::mutex databaseMutex; //At file scope, static gives the variable internal linkage. This means that it is only accessible within this .cpp file.


// ------------------------------------------
//  ADMIN AUTHORIZATION
// ------------------------------------------

bool Database::authorizeAdmin(
    const std::string& username,
    const std::string& password)
{
    //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to get data to database.\n";
        return false;
    }
    std::string loginFile = Configuration::loginDatabase;

    try
    {
        // First authenticate the user
        if (!authenticateUser(username, password))
        {
            std::cerr << "Authentication failed.\n";
            return false;
        }

        YAML::Node root = YAML::LoadFile(loginFile);

        // Find the authenticated user
        for (const auto& user : root["Users"])
        {
            if (user["username"].as<std::string>() == username)
            {
                // Check administrative role
                std::string role = user["role"].as<std::string>();

                if (role == "admin")
                {
                    return true;
                }

                std::cerr << "Access denied: Admin only.\n";
                return false;
            }
        }

        return false;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Authorization error: "
                  << e.what() << '\n';

        return false;
    }
}

// ------------------------------------------
//  USER AUTHORIZATION
// ------------------------------------------
bool Database::authenticateUser(
    const std::string& username,
    const std::string& password)
{
       //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to get data to database.\n";
        return false;
    }
    std::string loginFile = Configuration::loginDatabase;

    try
    {
        // Load login credentials
        YAML::Node root = YAML::LoadFile(loginFile);

        // Iterate through registered users
        for (const auto& user : root["Users"])
        {
            if (!user["username"] || !user["password"])
            {
                std::cerr << "Invalid user record.\n";
                return false;
            }

            std::string storedUsername = user["username"].as<std::string>();

            std::string storedPassword = user["password"].as<std::string>();

            // Compare entered credentials
            if (username == storedUsername && password == storedPassword)
            {
                return true;
            }
        }

        // No matching credentials found
        std::cout << "Invalid username or password.\n";

        return false;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Login database error: "
                  << e.what() << '\n';

        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//                                              TRACK
//////////////////////////////////////////////////////////////////////////////////////////////

// ------------------------------------------
//  TRACK:  DATABASE VALIDATION
// ------------------------------------------
/*
This function checks whether the existing YAML database contains valid and consistent track information.
The purpose is to prevent your application from working with malformed or inconsistent railway data.
*/
bool Database::validateDatabaseTrack(
    const std::string& filePath)
{
    try
    {
        YAML::Node root = YAML::LoadFile(filePath);

        if (!root["tracks"] ||
            !root["tracks"].IsSequence())
        {
            std::cerr << "Invalid database structure.\n";
            return false;
        }

        /*
        std::unordered_set: 
        Key properties:
        No duplicates: If you insert the same string twice, only one copy is stored.
        Unordered: The elements are not stored in any particular order; iteration order is not guaranteed.
        Fast lookups: Uses hashing internally, so find() and insert() are typically O(1) on average.
        */
        std::unordered_set<std::string> trackIDs;

        for (const auto& track : root["tracks"])
        {
            if (!track.IsMap() ||
                !track["id"] ||
                !track["occupancy"] ||
                !track["trainId"] ||
                !track["signal"])
            {
                std::cerr << "Missing track information.\n";
                return false;
            }

            std::string id =
                track["id"].as<std::string>();

            std::string occupancy =
                track["occupancy"].as<std::string>();

            std::string trainID =
                track["trainId"].as<std::string>();

            std::string signal =
                track["signal"].as<std::string>();

            // Prevent duplicate track IDs.
            if (id.empty() ||
                !trackIDs.insert(id).second)
            {
                std::cerr << "Invalid or duplicate track ID.\n";
                return false;
            }

            // Validate occupancy.
            if (occupancy != "free" &&
                occupancy != "occupied")
            {
                std::cerr << "Invalid occupancy value.\n";
                return false;
            }

            // Validate signal.
            if (signal != "red" &&
                signal != "green")
            {
                std::cerr << "Invalid signal value.\n";
                return false;
            }

            // Check consistency between occupancy and signal.
            if (occupancy == "occupied")
            {
                if (trainID == "no_train" || signal != "red")
                {
                    std::cerr << "Unsafe track state detected.\n";
                    return false;
                }
            }

            if (occupancy == "free")
            {
                if (trainID != "no_train" || signal != "green")
                {
                    std::cerr << "Inconsistent free track state.\n";
                    return false;
                }
            }
        }

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Database validation error: "
                  << e.what() << '\n';

        return false;
    }
}


// ------------------------------------------
// TRACK:  SECURE DATABASE UPDATE
// ------------------------------------------

bool Database::updateTrackOccupancy(const std::string& trackID)
{
    //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to update data to database.\n";
        return false;
    }
    std::string trainFile = Configuration::trainDatabase;
    std::string tempTrainDatabase = Configuration::tempTrainDatabase;
    std::string backupTrainDatabase = Configuration::backupTrainDatabase;

    std::string enteredValue_item;
    std::string newValue_occupency;
    std::string newValue_trainId;
    std::string newValue_id;

    /*  std::mutex:
        A synchronization object that allows only one thread at a time to hold its lock
        std::lock_guard:
        A C++ utility that automatically locks a mutex when constructed and unlocks it when destroyed.
    */
    std::lock_guard<std::mutex> lock(databaseMutex);

    const std::string filePath = trainFile;

    const std::string tempPath = tempTrainDatabase;

    const std::string backupPath = backupTrainDatabase;
    
    // Validate user input

    if (trackID.empty())
    {
        std::cerr << "Invalid track ID.\n";
        return false;
    }

    // Validate existing database

    if (!validateDatabaseTrack(filePath))
    {
        std::cerr << "Existing database is invalid.\n";
        return false;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(filePath);

        bool trackFound = false;

        //Find and update the requested track

        for (auto track : root["tracks"])
        {
            if (track["id"].as<std::string>() == trackID)
            {
                trackFound = true;

                std::cout << "1. Update Track Occupancy (Enter O)\n";
                std::cout << "2. Exit (Enter exit)\n";
                std::cout << "-> ";

                std::getline(std::cin >> std::ws, enteredValue_item);

                if (enteredValue_item == "O")
                {
                    

                    std::cout << "Set New occupency (Enter letter \"O\" to Occupy the track and \"R\" to Release the track):\n->  ";
                    std::getline(std::cin >> std::ws, newValue_occupency);

                    if(newValue_occupency == "R"){
                        track["occupancy"] = "free";
                        track["trainId"] = "no_train";
                        track["signal"] = "green";
                        break;     
                    }
                    if(newValue_occupency == "O"){
                        std::cout << "Enter Train Id:\n-> ";
                        std::getline(std::cin >> std::ws, newValue_trainId);

                        //check if the train id exist in train list
                        YAML::Node roottrain = YAML::LoadFile(trainFile);

                        bool trainFound = false;

                        for (auto train : roottrain["trains"])
                        {
                            if (train["id"].as<std::string>() == newValue_trainId)
                            {
                                trainFound = true;
                                break;
                            }
                        }

                        if (trainFound)
                        {
                            //check if the train id is already on a track. If it is, delete from that track and set to this track
                            bool trainTrackFound = false;
                            for (auto track1 : root["tracks"])
                            {
                                if (track1["trainId"].as<std::string>() == newValue_trainId)
                                {
                                    trainTrackFound = true;

                                    std::string yesorno;
                                    std::cout << "This train ID is already on a track. Do you want to set it on this track?\n";
                                    std::cout << "Enter Y for yes and N for no \n->";
                                    std::cin >> yesorno;
                                    
                                    if(yesorno == "Y"){
                                        //delete train id from old track
                                        track1["occupancy"] = "free";
                                        track1["trainId"] = "no_train";
                                        track1["signal"] = "green";

                                        //set train id on new track
                                        track["occupancy"] = "occupied";
                                        track["trainId"] = newValue_trainId;
                                        track["signal"] = "red";
                                        break;
                                    }
                                    else if(yesorno == "N"){
                                        return false;
                                    }
                                    else{
                                        std::cerr << "Wrong option \n->";
                                        return false; 
                                    }
                                }
                            }
                            if(!trainTrackFound){
                                track["occupancy"] = "occupied";
                                track["trainId"] = newValue_trainId;
                                track["signal"] = "red";
                                break;
                            }  
                        }
                        if(!trainFound){
                            std::cerr << "\nTrain Id not found in Train list\n";
                            return false;
                        }
                    }
                }
                else if (enteredValue_item == "exit")
                {
                    return false;
                }
                else
                {
                    std::cerr << "This option is not available.\n";
                    return false;
                }

            }
        }

        if (!trackFound)
        {
            std::cerr << "Track not found.\n";
            return false;
        }

        // Save changes to a temporary file
        {
            std::ofstream fout(tempPath);

            if (!fout.is_open())
            {
                std::cerr << "Cannot create temporary database.\n";
                return false;
            }

            fout << root;
            fout.flush();

            if (!fout.good())
            {
                fout.close();
                fs::remove(tempPath);

                std::cerr << "Failed to write temporary database.\n";
                return false;
            }
        }

        // Validate the modified database
        if (!validateDatabaseTrack(tempPath))
        {
            fs::remove(tempPath);

            std::cerr << "Updated database failed validation.\n";
            return false;
        }

        //Backup original database
        if (fs::exists(backupPath))
        {
            fs::remove(backupPath);
        }

        fs::rename(filePath, backupPath);

        //Replace original database
        try
        {
            fs::rename(tempPath, filePath);
        }
        catch (...)
        {
            // Restore original database if replacement fails.
            fs::rename(backupPath, filePath);
            throw;
        }

        //Remove backup after successful replacement
        fs::remove(backupPath);

        std::cout << "\nDatabase updated successfully!\n";

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: "
                  << e.what() << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File operation error: "
                  << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database update error: "
                  << e.what() << '\n';
    }

    // Remove an incomplete temporary file.
    std::error_code error;
    fs::remove(tempPath, error);

    return false;
}


bool Database::deleteTrack(const std::string& trackID){
    
    //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to delete data to database.\n";
        return false;
    }
    std::string trainFile = Configuration::trainDatabase;
    std::string tempTrainDatabase = Configuration::tempTrainDatabase;
    std::string backupTrainDatabase = Configuration::backupTrainDatabase;
    
    std::lock_guard<std::mutex> lock(databaseMutex);

    const std::string filePath = trainFile;

    const std::string tempPath = tempTrainDatabase;

    const std::string backupPath = backupTrainDatabase;
    
    // Validate user input

    if (trackID.empty())
    {
        std::cerr << "Invalid track ID.\n";
        return false;
    }

    // Validate existing database

    if (!validateDatabaseTrack(filePath))
    {
        std::cerr << "Existing database is invalid.\n";
        return false;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(filePath);
        YAML::Node updated(YAML::NodeType::Sequence);

        bool trackFound = false;

        //Find and update the requested track

        for (auto track : root["tracks"])
        {
            if (track["id"].as<std::string>() == trackID)
            {
                trackFound = true;

                for (const auto& track : root["tracks"])
                {
                    if (track["id"].as<std::string>() != trackID)
                    {
                        updated.push_back(track);
                    }
                }
                break;
            }
        }

        if (!trackFound)
        {
            std::cerr << "Track not found.\n";
            return false;
        }

        // Save changes to a temporary file
        {
            std::ofstream fout(tempPath);

            if (!fout.is_open())
            {
                std::cerr << "Cannot create temporary database.\n";
                return false;
            }

            root["tracks"] = updated;
            fout << root;
            fout.flush();

            if (!fout.good())
            {
                fout.close();
                fs::remove(tempPath);

                std::cerr << "Failed to write temporary database.\n";
                return false;
            }
        }

        // Validate the modified database
        if (!validateDatabaseTrack(tempPath))
        {
            fs::remove(tempPath);

            std::cerr << "Updated database failed validation.\n";
            return false;
        }

        //Backup original database
        if (fs::exists(backupPath))
        {
            fs::remove(backupPath);
        }

        fs::rename(filePath, backupPath);

        //Replace original database
        try
        {
            fs::rename(tempPath, filePath);
        }
        catch (...)
        {
            // Restore original database if replacement fails.
            fs::rename(backupPath, filePath);
            throw;
        }

        //Remove backup after successful replacement
        fs::remove(backupPath);

        std::cout << "\nDatabase updated successfully!\n";

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: "
                  << e.what() << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File operation error: "
                  << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database update error: "
                  << e.what() << '\n';
    }

    // Remove an incomplete temporary file.
    std::error_code error;
    fs::remove(tempPath, error);

    return false;
}


bool Database::addTrack(const std::string& trackID){

     //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to add data to database.\n";
        return false;
    }
    std::string trainFile = Configuration::trainDatabase;
    std::string tempTrainDatabase = Configuration::tempTrainDatabase;
    std::string backupTrainDatabase = Configuration::backupTrainDatabase;

    std::string occupancy = "free";
    std::string trainId = "no_train";
    std::string signal = "green";
    
    std::lock_guard<std::mutex> lock(databaseMutex);

    const std::string filePath = trainFile;

    const std::string tempPath = tempTrainDatabase;

    const std::string backupPath = backupTrainDatabase;
    
    // Validate user input

    if (trackID.empty())
    {
        std::cerr << "Invalid track ID.\n";
        return false;
    }

    // Validate existing database

    if (!validateDatabaseTrack(filePath))
    {
        std::cerr << "Existing database is invalid.\n";
        return false;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(filePath);
        YAML::Node track;

        bool trackFound = false;

        //Find and update the requested track

        for (auto track : root["tracks"])
        {
            if (track["id"].as<std::string>() == trackID)
            {
                trackFound = true;
                break;
            }
        }

        if (trackFound)
        {
            std::cerr << "Track ID already exist.\n";
            return false;
        }
        else if(!trackFound)// Save changes to a temporary file
        {
            std::ofstream fout(tempPath);

            if (!fout.is_open())
            {
                std::cerr << "Cannot create temporary database.\n";
                return false;
            }

            track["id"] = trackID;
            track["occupancy"] = occupancy;
            track["trainId"] = trainId;
            track["signal"] = signal;

            root["tracks"].push_back(track);
            
            fout << root;
            fout.flush();

            if (!fout.good())
            {
                fout.close();
                fs::remove(tempPath);

                std::cerr << "Failed to write temporary database.\n";
                return false;
            }
        }

        // Validate the modified database
        if (!validateDatabaseTrack(tempPath))
        {
            fs::remove(tempPath);

            std::cerr << "Updated database failed validation.\n";
            return false;
        }

        //Backup original database
        if (fs::exists(backupPath))
        {
            fs::remove(backupPath);
        }

        fs::rename(filePath, backupPath);

        //Replace original database
        try
        {
            fs::rename(tempPath, filePath);
        }
        catch (...)
        {
            // Restore original database if replacement fails.
            fs::rename(backupPath, filePath);
            throw;
        }

        //Remove backup after successful replacement
        fs::remove(backupPath);

        std::cout << "\nDatabase updated successfully!\n";

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: "
                  << e.what() << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File operation error: "
                  << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database update error: "
                  << e.what() << '\n';
    }

    // Remove an incomplete temporary file.
    std::error_code error;
    fs::remove(tempPath, error);

    return false;
}




//////////////////////////////////////////////////////////////////////////////////////////////
//                                              TAINS
//////////////////////////////////////////////////////////////////////////////////////////////

// ------------------------------------------
//  TAINS:  DATABASE VALIDATION
// ------------------------------------------
/*
This function checks whether the existing YAML database contains valid and consistent train information.
The purpose is to prevent your application from working with malformed or inconsistent railway data.
*/
bool Database::validateDatabaseTrain(
    const std::string& filePath)
{
    try
    {
        YAML::Node root = YAML::LoadFile(filePath);

        if (!root["trains"] ||
            !root["trains"].IsSequence())
        {
            std::cerr << "Invalid database structure.\n";
            return false;
        }

        /*
        std::unordered_set: 
        Key properties:
        No duplicates: If you insert the same string twice, only one copy is stored.
        Unordered: The elements are not stored in any particular order; iteration order is not guaranteed.
        Fast lookups: Uses hashing internally, so find() and insert() are typically O(1) on average.
        */
        std::unordered_set<std::string> trainIDs;

        for (const auto& train : root["trains"])
        {
            if (!train.IsMap() ||
                !train["id"] ||
                !train["name"] ||
                !train["destination"])
            {
                std::cerr << "Missing train information.\n";
                return false;
            }

            std::string id =
                train["id"].as<std::string>();

            // Prevent duplicate track IDs.
            if (id.empty() ||
                !trainIDs.insert(id).second)
            {
                std::cerr << "Invalid or duplicate train ID.\n";
                return false;
            }
        }

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Database validation error: "
                  << e.what() << '\n';

        return false;
    }
}


// ------------------------------------------
// TAINS:  SECURE DATABASE UPDATE
// ------------------------------------------

bool Database::updateTraindetails(const std::string& trainID)
{
    //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to update data to database.\n";
        return false;
    }
    std::string trainFile = Configuration::trainDatabase;
    std::string tempTrainDatabase = Configuration::tempTrainDatabase;
    std::string backupTrainDatabase = Configuration::backupTrainDatabase;

    std::string enteredValue_item;
    std::string newValue;

    /*  std::mutex:
        A synchronization object that allows only one thread at a time to hold its lock
        std::lock_guard:
        A C++ utility that automatically locks a mutex when constructed and unlocks it when destroyed.
    */
    std::lock_guard<std::mutex> lock(databaseMutex);

    const std::string filePath = trainFile;

    const std::string tempPath = tempTrainDatabase;

    const std::string backupPath = backupTrainDatabase;
    
    // Validate user input

    if (trainID.empty())
    {
        std::cerr << "Invalid train ID.\n";
        return false;
    }

    // Validate existing database

    if (!validateDatabaseTrack(filePath))
    {
        std::cerr << "Existing database is invalid.\n";
        return false;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(filePath);

        bool trainFound = false;

        //Find and update the requested train

        for (auto train : root["trains"])
        {
            {
                trainFound = true;

                std::cout << "\n------ What do you want to update? ------\n";
                std::cout << "1. Train Name (Enter N)\n";
                std::cout << "2. Train Destination (Enter D)\n";
                std::cout << "-> ";

                std::getline(std::cin >> std::ws, enteredValue_item);

                if (enteredValue_item == "N")
                {
                    std::cout << "Old Train Name: "
                              << train["name"].as<std::string>() << "\n";

                    std::cout << "Enter New Train Name:\n-> ";
                    std::getline(std::cin >> std::ws, newValue);

                    train["name"] = newValue;
                }
                else if (enteredValue_item == "D")
                {
                    std::cout << "Old Train Destination: "
                              << train["destination"].as<std::string>() << "\n";

                    std::cout << "Enter New Train Destination:\n-> ";
                    std::getline(std::cin >> std::ws, newValue);

                    train["destination"] = newValue;
                }
                else
                {
                    std::cerr << "This option is not available.\n";
                    break;
                }
                std::ofstream fout(trainFile);

                fout << root;
                fout.close();

                std::cout << "\nTrain successfully updated!\n";

                break;
            }
        }

        if (!trainFound)
        {
            std::cerr << "Train not found.\n";
            return false;
        }

        // Save changes to a temporary file
        {
            std::ofstream fout(tempPath);

            if (!fout.is_open())
            {
                std::cerr << "Cannot create temporary database.\n";
                return false;
            }

            fout << root;
            fout.flush();

            if (!fout.good())
            {
                fout.close();
                fs::remove(tempPath);

                std::cerr << "Failed to write temporary database.\n";
                return false;
            }
        }

        // Validate the modified database
        if (!validateDatabaseTrack(tempPath))
        {
            fs::remove(tempPath);

            std::cerr << "Updated database failed validation.\n";
            return false;
        }

        //Backup original database
        if (fs::exists(backupPath))
        {
            fs::remove(backupPath);
        }

        fs::rename(filePath, backupPath);

        //Replace original database
        try
        {
            fs::rename(tempPath, filePath);
        }
        catch (...)
        {
            // Restore original database if replacement fails.
            fs::rename(backupPath, filePath);
            throw;
        }

        //Remove backup after successful replacement
        fs::remove(backupPath);

        std::cout << "\nDatabase updated successfully!\n";

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: "
                  << e.what() << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File operation error: "
                  << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database update error: "
                  << e.what() << '\n';
    }

    // Remove an incomplete temporary file.
    std::error_code error;
    fs::remove(tempPath, error);

    return false;
}


bool Database::deleteTrain(const std::string& trainID){
    
    //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to delete data to database.\n";
        return false;
    }
    std::string trainFile = Configuration::trainDatabase;
    std::string tempTrainDatabase = Configuration::tempTrainDatabase;
    std::string backupTrainDatabase = Configuration::backupTrainDatabase;
    
    std::lock_guard<std::mutex> lock(databaseMutex);

    const std::string filePath = trainFile;

    const std::string tempPath = tempTrainDatabase;

    const std::string backupPath = backupTrainDatabase;
    
    // Validate user input

    if (trainID.empty())
    {
        std::cerr << "Invalid train ID.\n";
        return false;
    }

    // Validate existing database

    if (!validateDatabaseTrack(filePath))
    {
        std::cerr << "Existing database is invalid.\n";
        return false;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(filePath);
        YAML::Node updated(YAML::NodeType::Sequence);

        bool trainFound = false;

        //Find and update the requested track
        for (auto train : root["trains"])
        {
            if (train["id"].as<std::string>() == trainID)
            {
                trainFound = true;

                for (const auto& train : root["trains"])
                {
                    if (train["id"].as<std::string>() != trainID)
                    {
                        updated.push_back(train);
                    }
                }
                break;
            }
        }

        if (!trainFound)
        {
            std::cerr << "Train not found.\n";
            return false;
        }

        // Save changes to a temporary file
        {
            std::ofstream fout(tempPath);

            if (!fout.is_open())
            {
                std::cerr << "Cannot create temporary database.\n";
                return false;
            }

            root["trains"] = updated;
            fout << root;
            fout.flush();

            if (!fout.good())
            {
                fout.close();
                fs::remove(tempPath);

                std::cerr << "Failed to write temporary database.\n";
                return false;
            }
        }

        // Validate the modified database
        if (!validateDatabaseTrack(tempPath))
        {
            fs::remove(tempPath);

            std::cerr << "Updated database failed validation.\n";
            return false;
        }

        //Backup original database
        if (fs::exists(backupPath))
        {
            fs::remove(backupPath);
        }

        fs::rename(filePath, backupPath);

        //Replace original database
        try
        {
            fs::rename(tempPath, filePath);
        }
        catch (...)
        {
            // Restore original database if replacement fails.
            fs::rename(backupPath, filePath);
            throw;
        }

        //Remove backup after successful replacement
        fs::remove(backupPath);

        std::cout << "\nDatabase updated successfully!\n";

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: "
                  << e.what() << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File operation error: "
                  << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database update error: "
                  << e.what() << '\n';
    }

    // Remove an incomplete temporary file.
    std::error_code error;
    fs::remove(tempPath, error);

    return false;
}


bool Database::addTrain(const std::string& trainID, const std::string& name, const std::string& destination){

     //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to add data to database.\n";
        return false;
    }
    std::string trainFile = Configuration::trainDatabase;
    std::string tempTrainDatabase = Configuration::tempTrainDatabase;
    std::string backupTrainDatabase = Configuration::backupTrainDatabase;

    
    std::lock_guard<std::mutex> lock(databaseMutex);

    const std::string filePath = trainFile;

    const std::string tempPath = tempTrainDatabase;

    const std::string backupPath = backupTrainDatabase;
    
    // Validate user input

    if (trainID.empty())
    {
        std::cerr << "Invalid track ID.\n";
        return false;
    }

    // Validate existing database

    if (!validateDatabaseTrack(filePath))
    {
        std::cerr << "Existing database is invalid.\n";
        return false;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(filePath);
        YAML::Node train;

        bool trainFound = false;

        //Find and update the requested train

        for (auto train : root["trains"])
        {
            if (train["id"].as<std::string>() == trainID)
            {
                trainFound = true;
                break;
            }
        }

        if (trainFound)
        {
            std::cerr << "Train ID already exist.\n";
            return false;
        }
        else if(!trainFound)// Save changes to a temporary file
        {
            std::ofstream fout(tempPath);

            if (!fout.is_open())
            {
                std::cerr << "Cannot create temporary database.\n";
                return false;
            }

            train["id"] = trainID;
            train["name"] = name;
            train["destination"] = destination;

            root["trains"].push_back(train);
            
            fout << root;
            fout.flush();

            if (!fout.good())
            {
                fout.close();
                fs::remove(tempPath);

                std::cerr << "Failed to write temporary database.\n";
                return false;
            }
        }

        // Validate the modified database
        if (!validateDatabaseTrack(tempPath))
        {
            fs::remove(tempPath);

            std::cerr << "Updated database failed validation.\n";
            return false;
        }

        //Backup original database
        if (fs::exists(backupPath))
        {
            fs::remove(backupPath);
        }

        fs::rename(filePath, backupPath);

        //Replace original database
        try
        {
            fs::rename(tempPath, filePath);
        }
        catch (...)
        {
            // Restore original database if replacement fails.
            fs::rename(backupPath, filePath);
            throw;
        }

        //Remove backup after successful replacement
        fs::remove(backupPath);

        std::cout << "\nDatabase updated successfully!\n";

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: "
                  << e.what() << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File operation error: "
                  << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database update error: "
                  << e.what() << '\n';
    }

    // Remove an incomplete temporary file.
    std::error_code error;
    fs::remove(tempPath, error);

    return false;
}


/////////////////////////////////////////////////////////////////////////////////////
// USER
////////////////////////////////////////////////////////////////////////////////////

// ------------------------------------------
//  TAINS:  DATABASE VALIDATION
// ------------------------------------------
/*
This function checks whether the existing YAML database contains valid and consistent train information.
The purpose is to prevent your application from working with malformed or inconsistent railway data.
*/
bool Database::validateDatabaseLogin(
    const std::string& filePath)
{
    try
    {
        YAML::Node root = YAML::LoadFile(filePath);

        if (!root["Users"] ||
            !root["Users"].IsSequence())
        {
            std::cerr << "Invalid database structure.\n";
            return false;
        }

        /*
        std::unordered_set: 
        Key properties:
        No duplicates: If you insert the same string twice, only one copy is stored.
        Unordered: The elements are not stored in any particular order; iteration order is not guaranteed.
        Fast lookups: Uses hashing internally, so find() and insert() are typically O(1) on average.
        */
        std::unordered_set<std::string> userNames;

        for (const auto& user : root["Users"])
        {
            if (!user.IsMap() ||
                !user["username"] ||
                !user["password"] )
            {
                std::cerr << "Missing user information.\n";
                return false;
            }

            std::string username =
                user["username"].as<std::string>();

            // Prevent duplicate track IDs.
            if (username.empty() ||
                !userNames.insert(username).second)
            {
                std::cerr << "Invalid or duplicate Username.\n";
                return false;
            }
        }

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Database validation error: "
                  << e.what() << '\n';

        return false;
    }
}

bool Database::deleteUser(const std::string& username){
    
    //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to delete data to database.\n";
        return false;
    }
    std::string loginFile = Configuration::loginDatabase;
    std::string tempLoginDatabase = Configuration::tempLoginDatabase;
    std::string backupLoginDatabase = Configuration::backupLoginDatabase;
    
    std::lock_guard<std::mutex> lock(databaseMutex);

    const std::string filePath = loginFile;

    const std::string tempPath = tempLoginDatabase;

    const std::string backupPath = backupLoginDatabase;
    
    // Validate user input

    if (username.empty())
    {
        std::cerr << "Invalid user Name.\n";
        return false;
    }

    // Validate existing database

    if (!validateDatabaseLogin(filePath))
    {
        std::cerr << "Existing database is invalid.\n";
        return false;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(filePath);
        YAML::Node updated(YAML::NodeType::Sequence);

        bool usernameFound = false;

        //Find and update the requested track
        for (auto user : root["Users"])
        {
            if (user["username"].as<std::string>() == username)
            {
                usernameFound = true;

                for (const auto& user : root["Users"])
                {
                    if (user["username"].as<std::string>() != username)
                    {
                        updated.push_back(user);
                    }
                }
                break;
            }
        }

        if (!usernameFound)
        {
            std::cerr << "Username not found.\n";
            return false;
        }

        // Save changes to a temporary file
        {
            std::ofstream fout(tempPath);

            if (!fout.is_open())
            {
                std::cerr << "Cannot create temporary database.\n";
                return false;
            }

            root["Users"] = updated;
            fout << root;
            fout.flush();

            if (!fout.good())
            {
                fout.close();
                fs::remove(tempPath);

                std::cerr << "Failed to write temporary database.\n";
                return false;
            }
        }

        // Validate the modified database
        if (!validateDatabaseLogin(tempPath))
        {
            fs::remove(tempPath);

            std::cerr << "Updated database failed validation.\n";
            return false;
        }

        //Backup original database
        if (fs::exists(backupPath))
        {
            fs::remove(backupPath);
        }

        fs::rename(filePath, backupPath);

        //Replace original database
        try
        {
            fs::rename(tempPath, filePath);
        }
        catch (...)
        {
            // Restore original database if replacement fails.
            fs::rename(backupPath, filePath);
            throw;
        }

        //Remove backup after successful replacement
        fs::remove(backupPath);

        std::cout << "\nDatabase updated successfully!\n";

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: "
                  << e.what() << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File operation error: "
                  << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database update error: "
                  << e.what() << '\n';
    }

    // Remove an incomplete temporary file.
    std::error_code error;
    fs::remove(tempPath, error);

    return false;
}


bool Database::addNewUser(const std::string& username, const std::string& userpassword){

     //set database file path
    if (!Configuration::loadConfig())
    {
        std::cerr << "Config Data Failure: Unable to add data to database.\n";
        return false;
    }
    std::string loginFile = Configuration::loginDatabase;
    std::string tempLoginDatabase = Configuration::tempLoginDatabase;
    std::string backupLoginDatabase = Configuration::backupLoginDatabase;

    
    std::lock_guard<std::mutex> lock(databaseMutex);

    const std::string filePath = loginFile;

    const std::string tempPath = tempLoginDatabase;

    const std::string backupPath = backupLoginDatabase;
    
    // Validate user input

    if (username.empty() || userpassword.empty())
    {
        std::cerr << "The username and password should not be empty.\n";
        return false;
    }

    // Validate existing database
    if (!validateDatabaseLogin(filePath))
    {
        std::cerr << "Existing database is invalid.\n";
        return false;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(filePath);
        YAML::Node user;

        bool usernameFound = false;

        //Find and update the requested username
        for (auto user : root["Users"])
        {
            if (user["username"].as<std::string>() == username)
            {
                usernameFound = true;
                break;
            }
        }

        if (usernameFound)
        {
            std::cerr << "Username already exist.\n";
            return false;
        }
        else if(!usernameFound)// Save changes to a temporary file
        {
            std::ofstream fout(tempPath);

            if (!fout.is_open())
            {
                std::cerr << "Cannot create temporary database.\n";
                return false;
            }

            user["username"] = username;
            user["password"] = userpassword;
            user["role"] = "user";

            root["Users"].push_back(user);
            
            fout << root;
            fout.flush();

            if (!fout.good())
            {
                fout.close();
                fs::remove(tempPath);

                std::cerr << "Failed to write temporary database.\n";
                return false;
            }
        }

        // Validate the modified database
        if (!validateDatabaseLogin(tempPath))
        {
            fs::remove(tempPath);

            std::cerr << "Updated database failed validation.\n";
            return false;
        }

        //Backup original database
        if (fs::exists(backupPath))
        {
            fs::remove(backupPath);
        }

        fs::rename(filePath, backupPath);

        //Replace original database
        try
        {
            fs::rename(tempPath, filePath);
        }
        catch (...)
        {
            // Restore original database if replacement fails.
            fs::rename(backupPath, filePath);
            throw;
        }

        //Remove backup after successful replacement
        fs::remove(backupPath);

        std::cout << "\nDatabase updated successfully!\n";

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: "
                  << e.what() << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File operation error: "
                  << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database update error: "
                  << e.what() << '\n';
    }

    // Remove an incomplete temporary file.
    std::error_code error;
    fs::remove(tempPath, error);

    return false;
}