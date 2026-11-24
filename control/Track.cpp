#include <iostream>
#include <string>
#include "Track.h"
#include <iomanip>
#include <fstream>
#include <yaml-cpp/yaml.h>

std::string* Track::trackServiceOption()
{
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
                            std::cout << "This option is not available.\n";
                        }
                    }
                    if(selectedService == "exit"){
                        checker = 1;
                        break;
                    }
                }else {
                        std::cout << "\nInvalid input\n" ;
                    }
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

//adding new train
void Track::addTrack(std::string id){
    std::string occupancy = "free";
    std::string trainId = "no_train";
    std::string signal = "green";
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");
        YAML::Node track;

        bool trackFound = false;

        for (auto track : root["tracks"])
        {
            if (track["id"].as<std::string>() == id)
            {
                trackFound = true;
                break;
            }
        }

        if (trackFound)
        {
            std::cout << "This ID alreay exist.\n";
            return;
        }
        else if(!trackFound){
            
            track["id"] = id;
            track["occupancy"] = occupancy;
            track["trainId"] = trainId;
            track["signal"] = signal;

            root["tracks"].push_back(track);
                
            std::ofstream fout("database/trains.yaml");

            fout << root;
            fout.close();

            std::cout << "\n\nTRACK REGISTERED SUCCESSFULLY!!\n\n";
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
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

//deleting track with respect to the train ID
void Track::deleteTrack(std::string enteredValue_id){

    YAML::Node root = YAML::LoadFile("database/trains.yaml");

    YAML::Node updated(YAML::NodeType::Sequence);

    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        bool trackFound = false;

        for (auto track : root["tracks"])
        {
            if (track["id"].as<std::string>() == enteredValue_id)
            {
                trackFound = true;
                for (const auto& track : root["tracks"])
                {
                    if (track["id"].as<std::string>() != enteredValue_id)
                    {
                        updated.push_back(track);
                    }
                }
                break;
            }
        }

        if (!trackFound)
        {
            std::cout << "This ID does not exist.\n";
            return;
        }

        root["tracks"] = updated;

        std::ofstream fout("database/trains.yaml");
        fout << root;
        fout.close();
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }
}

//updating track by searching the train ID
void Track::updateTrack(std::string enteredValue_id)
{
    std::string enteredValue_item;
    std::string newValue_occupency;
    std::string newValue_trainId;
    std::string newValue_id;

    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        bool trackFound = false;

        for (auto track : root["tracks"])
        {
            if (track["id"].as<std::string>() == enteredValue_id)
            {
                trackFound = true;

                std::cout << "\n------ What do you want to update? ------\n";
                std::cout << "1. Track Occupency (Enter O)\n";
                std::cout << "2. Exit (Enter exit)\n";
                std::cout << "-> ";

                std::getline(std::cin >> std::ws, enteredValue_item);

                if (enteredValue_item == "O")
                {
                    

                    std::cout << "Set New occupency (Enter letter \"O\" for occupied and \"F\" for free):\n->  ";
                    std::getline(std::cin >> std::ws, newValue_occupency);

                    if(newValue_occupency == "F"){
                        track["occupency"] = "free";
                        track["trainId"] = "no_train";
                        track["signal"] = "green";
                        
                        std::ofstream fout("database/trains.yaml");

                        fout << root;
                        fout.close();

                        std::cout << "\nTrack successfully updated!\n";

                        break;     
                    }
                    if(newValue_occupency == "O"){
                        std::cout << "Enter Train Id:\n-> ";
                        std::getline(std::cin >> std::ws, newValue_trainId);

                        //check if the train id exist in train list
                        YAML::Node roottrain = YAML::LoadFile("database/trains.yaml");

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
                                        
                                        std::ofstream fout("database/trains.yaml");

                                        fout << root;
                                        fout.close();

                                        std::cout << "\nTrack successfully updated!\n";

                                        return;
                                    }
                                    else if(yesorno == "N"){
                                        return;
                                    }
                                    else{
                                        std::cout << "Wrong option \n->";
                                        return; 
                                    }
                                }
                            }
                            if(!trainTrackFound){
                                track["occupancy"] = "occupied";
                                track["trainId"] = newValue_trainId;
                                track["signal"] = "red";
                                
                                std::ofstream fout("database/trains.yaml");

                                fout << root;
                                fout.close();

                                std::cout << "\nTrack successfully updated!\n";

                                break;
                            }  
                        }
                        if(!trainFound){
                            std::cout << "\nTrain Id not found in Train list\n";
                            return;
                        }
                    }
                }
                else if (enteredValue_item == "exit")
                {
                    return;
                }
                else
                {
                    std::cout << "This option is not available.\n";
                    break;
                }
            }
        }

        if (!trackFound)
        {
            std::cout << "This ID does not exist.\n";
            return;
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
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

                    std::ofstream fout("database/trains.yaml");

                    fout << root;
                    fout.close();
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

                    std::ofstream fout("database/trains.yaml");

                    fout << root;
                    fout.close();
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

                    std::ofstream fout("database/trains.yaml");

                    fout << root;
                    fout.close();
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

                    std::ofstream fout("database/trains.yaml");

                    fout << root;
                    fout.close();
                }
            }
        }

        if (!trackFound)
        {
            if(searchParams == "I"){
                std::cout << "This ID does not exist.\n";
                return;
            }
            if(searchParams == "O"){
                std::cout << "This Track Occupation does not exist.\n";
                return;
            }
            if(searchParams == "S"){
                std::cout << "This Signal does not exist.\n";
                return;
            }
            if(searchParams == "T"){
                std::cout << "This Train Id does not exist.\n";
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