#include <iostream>
#include <string>
#include "Signal.h"
#include <iomanip>
#include <fstream>
#include <yaml-cpp/yaml.h>

std::string* Signal::signalServiceOption()
{
    std::string enteredValue_id;
    Signal::selectedService;
    int checker = 0;
    std::string const signalOptions[5] = {"A", "R", "G","S" , "exit"};//enter the letters which will be checked after user has enterd the letter
    
    do
    {
        std::cout << "\n\n***Please enter a letter from the list and enter \"exit\" to end.***\n\n";
        Signal::selectedService = signalgetSelectedService(); //print the selectable options
        for (int i = 0; i <  sizeof(signalOptions) / sizeof(signalOptions[0]); i++)
            {
                if(selectedService == signalOptions[i]){
                    if(selectedService == "A"){
                        viewAllSignalOccupancy();
                    }
                    if(selectedService == "R"){
                        viewOccupiedSignals();
                    }
                    if(selectedService == "G"){
                        viewfreeSignals();
                    }
                    if(selectedService == "S"){
                        std::cout << "\nEnter Track ID: ";
                        std::getline(std::cin >> std::ws, enteredValue_id);
                        viewsearchedIDSignals(enteredValue_id);
                    }
                    if(selectedService == "exit"){
                        checker = 1;
                        break;
                    }
                }
            }
    } while (checker == 0);
  
   std::string* pointerOfSelectedService = &selectedService; //points to the memory address of the value of selectedService
   return pointerOfSelectedService; //sends the value of the variable selectedService
   
}

//selection menu
std::string Signal::signalgetSelectedService(){

    std::string enteredValue;
    std::cout << "Select the service you want to use by entering the coresponding letter: \n\n";
    
    // Enter and Print Track information
    std::cout << "1. View all Track signal (Enter letter A) \n\n";
    std::cout << "2. View Red signal Track List (Free Tracks) (Enter letter R) \n\n";
    std::cout << "3. View Green signal Track List (Occupied Tracks) (Enter letter G) \n\n";
    std::cout << "4. Search signal of a particular Track (Enter letter S) \n\n";
    std::cout << "5. Exit (Enter exit) \n\n";
    std::cout << "->";

    std::cin >> enteredValue;

    return enteredValue;

}

//viewing all tracks signals
void Signal::viewAllSignalOccupancy(){
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        std::cout << "====================================\n";
        std::cout << "      TRACKS SIGNAL DETAILS\n";
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

//viewing tracks with Red signals
void Signal::viewOccupiedSignals(){
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        const int colWidth = 20;
        std::cout << std::left 
            << std::setw(colWidth) << "ID"
            << std::setw(colWidth) << "OCCUPENCY"
            << std::setw(colWidth) << "TRAIN_ID"
            << std::setw(colWidth) << "SIGNAL" << "\n";

        for (auto track : root["tracks"])
        {
            
            if (track["signal"].as<std::string>() == "red")
            {
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
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }

}

//viewing tracks with green signals
void Signal::viewfreeSignals(){
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        const int colWidth = 20;
        std::cout << std::left 
            << std::setw(colWidth) << "ID"
            << std::setw(colWidth) << "OCCUPENCY"
            << std::setw(colWidth) << "TRAIN_ID"
            << std::setw(colWidth) << "SIGNAL" << "\n";

        for (auto track : root["tracks"])
        {
            
            if (track["signal"].as<std::string>() == "green")
            {
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
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }

}

//viewing particular track signals
void Signal::viewsearchedIDSignals(std::string enteredValue_id){
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
            
            if (track["id"].as<std::string>() == enteredValue_id)
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

        if(!trackFound){
            std::cout << "Track ID was not found" << '\n';
            return;
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }

}