#include <iostream>
#include <string>
#include "Track.h"
#include "Database.h"
#include <iomanip>
#include <fstream>
#include <yaml-cpp/yaml.h>


std::string* Track::trackServiceOption()
{
    std::cout << "====================================\n";
    std::cout << "      RAILWAY MONITORING SYSTEM\n";
    std::cout << "====================================\n\n";
    
    std::cout << "SERVICE: Track Settings\n";
    std::cout << "============================================\n\n";
    std::string enteredValue_id;
    std::string enteredValue_name;
    std::string enteredValue_destination;
    std::string enteredValueSearchParams;
    std::string enteredValue;
    Track::selectedService;
    int checker = 0;
    std::string const trackOptions[6] = {"C", "R", "U", "D", "S", "exit"};//enter the letters which will be checked after user has enterd the letter
    
    do
    {
        viewTrack();
        std::cout << "\n\n***Please enter a letter from the list and enter \"exit\" to end.***\n\n";
        Track::selectedService = trackgetSelectedService(); //print the selectable options
        for (int i = 0; i <  sizeof(trackOptions) / sizeof(trackOptions[0]); i++)
            {
                if(selectedService == trackOptions[i]){
                    if(selectedService == "C"){
                        std::cout << "\nEnter Track ID: ";
                        std::getline(std::cin >> std::ws, enteredValue_id);

                        addTrack(enteredValue_id);
                    }
                    if(selectedService == "R"){
                        viewTrack();
                    }
                    if(selectedService == "U"){
                        std::cout << "\n------ Enter the ID of the track you want to update ------\n";
                        std::cout << "-> ";
                        std::getline(std::cin >> std::ws, enteredValue_id);
                        updateTrack(enteredValue_id);
                    }
                    if(selectedService == "D"){
                        std::cout << "\n------Enter the ID of the track you want to delete-------\n";
                        std::cout << "->";

                        std::getline(std::cin >> std::ws, enteredValue_id);

                        deleteTrack(enteredValue_id);
                    }
                    if(selectedService == "S"){
                        std::cout << "\n------ What do you want to search? ------\n";
                        std::cout << "1. Track ID (Enter I)\n";
                        std::cout << "2. Track Occupancy (Enter O)\n";
                        std::cout << "3. Track Signal (Enter S)\n";
                        std::cout << "4. Train Id (Enter T)\n";
                        std::cout << "-> ";

                        std::getline(std::cin >> std::ws, enteredValueSearchParams);

                        if (enteredValueSearchParams == "I")
                        {
                            std::cout << "\n------Enter the ID of the track you want to search-------\n";
                            std::cout << "->";
                            std::getline(std::cin >> std::ws, enteredValue);

                            search(enteredValue,enteredValueSearchParams);
                        }
                        else if (enteredValueSearchParams == "O")
                        {
                            std::cout << "\n------Enter the Occupancy of the track you want to search-------\n";
                            std::cout << "->";
                            std::getline(std::cin >> std::ws, enteredValue);

                            search(enteredValue,enteredValueSearchParams);
                        }
                        else if (enteredValueSearchParams == "S")
                        {
                            std::cout << "\n------Enter the Destination of the track you want to search-------\n";
                            std::cout << "->";
                            std::getline(std::cin >> std::ws, enteredValue);

                            search(enteredValue,enteredValueSearchParams);
                        }
                        else if (enteredValueSearchParams == "T")
                        {
                            std::cout << "\n------Enter the train Id you want to search-------\n";
                            std::cout << "->";
                            std::getline(std::cin >> std::ws, enteredValue);

                            search(enteredValue,enteredValueSearchParams);
                        }
                        else
                        {
                            std::cerr << "This option is not available.\n";
                        }
                    }
                    if(selectedService == "exit"){
                        checker = 1;
                        break;
                    }
                }
            }
        if(checker == 0){
            std::cerr << "\nInvalid input\n";
        } 
    } while (checker == 0);
  
   std::string* pointerOfSelectedService = &selectedService; //points to the memory address of the value of selectedService
   return pointerOfSelectedService; //sends the value of the variable selectedService
   
}

//selection menu
std::string Track::trackgetSelectedService(){

    std::string enteredValue;
    std::cout << "Select the service you want to use by entering the coresponding letter: \n\n";
    
    // Enter and Print Track information
    std::cout << "1. Add New Track (Enter letter C) \n\n";
    std::cout << "2. View Track List (Enter letter R) \n\n";
    std::cout << "3. Update Track Details (Enter letter U) \n\n";
    std::cout << "4. Delete Track (Enter letter D) \n\n";
    std::cout << "5. Search for a particular track (Enter letter S) \n\n";
    std::cout << "6. Exit (Enter exit) \n\n";
    std::cout << "->";

    std::cin >> enteredValue;

    return enteredValue;

}

//adding new track
void Track::addTrack(std::string id){
    std::string enteredValue_username;
    std::string enteredValue_password;

    std::cout << "You must login to add a Track\n";
    std::cout << "Enter username\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_username);

    std::cout << "Enter password\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_password);

    Database database;
    bool logedin = database.authenticateUser(enteredValue_username, enteredValue_password);

    if (!logedin)
    {
        std::cerr << "Track details update rejected.\n";
        return;
    }

    std::cout << "Login Successfull.\n";
    bool addTrack = database.addTrack(id);
    
    if (addTrack)
    {
        std::cout << "Track was successfully added\n";
        return;
    }
    if (!addTrack)
    {
        std::cerr << "Failed to add Track\n";
        return;
    }  
}

//viewing all tracks
void Track::viewTrack(){
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        std::cout << "====================================\n";
        std::cout << "      TRACKS DETAILS\n";
        std::cout << "====================================\n";

        //Print table
        const int colWidth = 20;
        std::cout << std::left 
            << std::setw(colWidth) << "ID"
            << std::setw(colWidth) << "OCCUPENCY"
            << std::setw(colWidth) << "TRAIN_ID"
            << std::setw(colWidth) << "SIGNAL" << "\n";

        for (const auto& track : root["tracks"])
        {  
            std::cout << std::left 
                << std::setw(colWidth) << track["id"].as<std::string>()
                << std::setw(colWidth) << track["occupancy"].as<std::string>()
                << std::setw(colWidth) << track["trainId"].as<std::string>()
                << std::setw(colWidth) << track["signal"].as<std::string>() << "\n";
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Error reading track database: "
                  << e.what() << "\n";
    }
}

//deleting track with respect to the track ID
void Track::deleteTrack(std::string enteredValue_id){

    std::string enteredValue_username;
    std::string enteredValue_password;

    std::cout << "You must login to delete a Track\n";
    std::cout << "Enter username\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_username);

    std::cout << "Enter password\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_password);

    Database database;
    bool logedin = database.authenticateUser(enteredValue_username, enteredValue_password);

    if (!logedin)
    {
        std::cerr << "Track details update rejected.\n";
        return;
    }

    std::cout << "Login Successfull.\n";
    bool deleteTrack = database.deleteTrack(enteredValue_id);
    
    if (deleteTrack)
    {
        std::cout << "Track was successfully deleted\n";
        return;
    }
    if (!deleteTrack)
    {
        std::cerr << "Failed to delete Track\n";
        return;
    }
}

//updating track by searching the track ID
void Track::updateTrack(std::string enteredValue_id)
{
    std::string enteredValue_username;
    std::string enteredValue_password;

    std::cout << "You must login to update\n";
    std::cout << "Enter username\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_username);

    std::cout << "Enter password\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_password);

    Database database;
    bool logedin = database.authenticateUser(enteredValue_username, enteredValue_password);

    if (!logedin)
    {
        std::cerr << "Track details update rejected.\n";
        return;
    }

    std::cout << "Login Successfull.\n";
    bool updateTrackOccupancy = database.updateTrackOccupancy(enteredValue_id);
    
    if (updateTrackOccupancy)
    {
        std::cout << "Track details was successfully updated\n";
        return;
    }
    if (!updateTrackOccupancy)
    {
        std::cerr << "Track details failed to update\n";
        return;
    }
}

//searching by ID or track occupancy or track signal or trainId
void Track::search(std::string enteredValue, std::string searchParams){
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        bool trackFound = false;

        const int colWidth = 20;
        std::cout << std::left 
            << std::setw(colWidth) << "ID"
            << std::setw(colWidth) << "OCCUPENCY"
            << std::setw(colWidth) << "TRAIN_ID"
            << std::setw(colWidth) << "SIGNAL" << "\n";

        for (auto track : root["tracks"])
        {
            if(searchParams == "I"){
                if (track["id"].as<std::string>() == enteredValue)
                {
                    trackFound = true;

                    std::cout << std::left 
                        << std::setw(colWidth) << track["id"].as<std::string>()
                        << std::setw(colWidth) << track["occupancy"].as<std::string>()
                        << std::setw(colWidth) << track["trainId"].as<std::string>()
                        << std::setw(colWidth) << track["signal"].as<std::string>() << "\n";
                }
            }
            if(searchParams == "O"){
                if (track["name"].as<std::string>() == enteredValue)
                {
                    trackFound = true;

                    std::cout << std::left 
                        << std::setw(colWidth) << track["id"].as<std::string>()
                        << std::setw(colWidth) << track["occupancy"].as<std::string>()
                        << std::setw(colWidth) << track["trainId"].as<std::string>()
                        << std::setw(colWidth) << track["signal"].as<std::string>() << "\n";
                }
            }
            if(searchParams == "S"){
                if (track["destination"].as<std::string>() == enteredValue)
                {
                    trackFound = true;

                    std::cout << std::left 
                        << std::setw(colWidth) << track["id"].as<std::string>()
                        << std::setw(colWidth) << track["occupancy"].as<std::string>()
                        << std::setw(colWidth) << track["trainId"].as<std::string>()
                        << std::setw(colWidth) << track["signal"].as<std::string>() << "\n";
                }
            }
            if(searchParams == "T"){
                if (track["destination"].as<std::string>() == enteredValue)
                {
                    trackFound = true;

                    std::cout << std::left 
                        << std::setw(colWidth) << track["id"].as<std::string>()
                        << std::setw(colWidth) << track["occupancy"].as<std::string>()
                        << std::setw(colWidth) << track["trainId"].as<std::string>()
                        << std::setw(colWidth) << track["signal"].as<std::string>() << "\n";
                }
            }
        }

        if (!trackFound)
        {
            if(searchParams == "I"){
                std::cerr << "This ID does not exist.\n";
                return;
            }
            if(searchParams == "O"){
                std::cerr << "This Track Occupation does not exist.\n";
                return;
            }
            if(searchParams == "S"){
                std::cerr << "This Signal does not exist.\n";
                return;
            }
            if(searchParams == "T"){
                std::cerr << "This Train Id does not exist.\n";
                return;
            }
            return;  
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }

}

std::string Track::trackStatusChange(){
    std::cout << "====================================\n";
    std::cout << "      RAILWAY MONITORING SYSTEM\n";
    std::cout << "====================================\n\n";
    
    std::cout << "SERVICE: Occupy a track and Release a track\n";
    std::cout << "============================================\n\n";
    std::string enteredValue_id;
    int checker = 0;
    
    do
    {
        viewTrack();

        std::cout << "\n------ Enter Track ID or enter exit to quit------\n";
        std::cout << "-> ";
        std::getline(std::cin >> std::ws, enteredValue_id);
        
        if(enteredValue_id == "exit"){
                checker = 1;
                break;
        }
        else {
            updateTrack(enteredValue_id);
        }

    }while(checker == 0);
    
    return enteredValue_id;

}