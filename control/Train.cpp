#include <iostream>
#include <string>
#include "Train.h"
#include <iomanip>
#include <fstream>
#include <yaml-cpp/yaml.h>

std::string* Train::trainServiceOption()
{
    std::cout << "====================================\n";
    std::cout << "      RAILWAY MONITORING SYSTEM\n";
    std::cout << "====================================\n\n";
    
    std::cout << "SERVICE: Train Settings\n";
    std::cout << "============================================\n\n";
    std::string enteredValue_id;
    std::string enteredValue_name;
    std::string enteredValue_destination;
    std::string enteredValueSearchParams;
    std::string enteredValue;
    Train::selectedService;
    int checker = 0;
    std::string const trainOptions[6] = {"C", "R", "U", "D", "S", "exit"};//enter the letters which will be checked after user has enterd the letter
    
    do
    {
        viewTrain();
        std::cout << "\n\n***Please enter a letter from the list and enter \"exit\" to end.***\n\n";
        Train::selectedService = getSelectedService(); //print the selectable options
        for (int i = 0; i <  sizeof(trainOptions) / sizeof(trainOptions[0]); i++)
            {
                if(selectedService == trainOptions[i]){
                    if(selectedService == "C"){
                        std::cout << "\nEnter Train ID: ";
                        std::getline(std::cin >> std::ws, enteredValue_id);

                        std::cout << "Enter Train Name: ";
                        std::getline(std::cin >> std::ws, enteredValue_name);

                        std::cout << "Enter Train Destination: ";
                        std::getline(std::cin >> std::ws, enteredValue_destination);
                        addTrain(enteredValue_id,enteredValue_name,enteredValue_destination);
                    }
                    if(selectedService == "R"){
                        viewTrain();
                    }
                    if(selectedService == "U"){
                        std::cout << "\n------ Enter the ID of the train you want to update ------\n";
                        std::cout << "-> ";
                        std::getline(std::cin >> std::ws, enteredValue_id);
                        updateTrain(enteredValue_id);
                    }
                    if(selectedService == "D"){
                        std::cout << "\n------Enter the ID of the train you want to delete-------\n";
                        std::cout << "->";

                        std::getline(std::cin >> std::ws, enteredValue_id);

                        deleteTrain(enteredValue_id);
                    }
                    if(selectedService == "S"){
                        std::cout << "\n------ What do you want to search? ------\n";
                        std::cout << "1. Train ID (Enter I)\n";
                        std::cout << "2. Train Name (Enter N)\n";
                        std::cout << "3. Train Destination (Enter D)\n";
                        std::cout << "-> ";

                        std::getline(std::cin >> std::ws, enteredValueSearchParams);

                        if (enteredValueSearchParams == "I")
                        {
                            std::cout << "\n------Enter the ID of the train you want to search-------\n";
                            std::cout << "->";
                            std::getline(std::cin >> std::ws, enteredValue);

                            search(enteredValue,enteredValueSearchParams);
                        }
                        else if (enteredValueSearchParams == "N")
                        {
                            std::cout << "\n------Enter the Name of the train you want to search-------\n";
                            std::cout << "->";
                            std::getline(std::cin >> std::ws, enteredValue);

                            search(enteredValue,enteredValueSearchParams);
                        }
                        else if (enteredValueSearchParams == "D")
                        {
                            std::cout << "\n------Enter the Destination of the train you want to search-------\n";
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
std::string Train::getSelectedService(){

    std::string enteredValue;
    std::cout << "Select the service you want to use by entering the coresponding letter: \n\n";
    
    // Enter and Print Train information
    std::cout << "1. Add New Train (Enter letter C) \n\n";
    std::cout << "2. View Train List (Enter letter R) \n\n";
    std::cout << "3. Update Train Details (Enter letter U) \n\n";
    std::cout << "4. Delete Train (Enter letter D) \n\n";
    std::cout << "5. Search for a particular train (Enter letter S) \n\n";
    std::cout << "6. Exit (Enter exit) \n\n";
    std::cout << "->";

    std::cin >> enteredValue;

    return enteredValue;

}

//adding new train
void Train::addTrain(std::string id, std::string name, std::string destination){

    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");
        YAML::Node train;

        bool trainFound = false;

        for (auto train : root["trains"])
        {
            if (train["id"].as<std::string>() == id)
            {
                trainFound = true;
                break;
            }
        }

        if (trainFound)
        {
            std::cerr << "This ID alreay exist.\n";
            return;
        }
        else if(!trainFound){
            train["id"] = id;
            train["name"] = name;
            train["destination"] = destination;

            root["trains"].push_back(train);
                
            std::ofstream fout("database/trains.yaml");

            fout << root;
            fout.close();

            std::cout << "\n\nTRAIN REGISTERED SUCCESSFULLY!!\n\n";
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }
}

//viewing all trains
void Train::viewTrain(){
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        std::cout << "====================================\n";
        std::cout << "      TRAINS DETAILS\n";
        std::cout << "====================================\n";

        //Print table
        const int colWidth = 20;
        std::cout << std::left 
            << std::setw(colWidth) << "ID"
            << std::setw(colWidth) << "NAME"
            << std::setw(colWidth) << "DESTINATION" << "\n";

        for (const auto& train : root["trains"])
        {  
            std::cout << std::left 
                << std::setw(colWidth) << train["id"].as<std::string>()
                << std::setw(colWidth) << train["name"].as<std::string>()
                << std::setw(colWidth) << train["destination"].as<std::string>() << "\n";
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Error reading train database: "
                  << e.what() << "\n";
    }
}

//deleting train with respect to the train ID
void Train::deleteTrain(std::string enteredValue_id){

    YAML::Node root = YAML::LoadFile("database/trains.yaml");

    YAML::Node updated(YAML::NodeType::Sequence);

    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        bool trainFound = false;

        for (auto train : root["trains"])
        {
            if (train["id"].as<std::string>() == enteredValue_id)
            {
                trainFound = true;
                for (const auto& train : root["trains"])
                {
                    if (train["id"].as<std::string>() != enteredValue_id)
                    {
                        updated.push_back(train);
                    }
                }
                break;
            }
        }

        if (!trainFound)
        {
            std::cerr << "This ID does not exist.\n";
            return;
        }

        root["trains"] = updated;

        std::ofstream fout("database/trains.yaml");
        fout << root;
        fout.close();
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }
}

//updating train by searching the train ID
void Train::updateTrain(std::string enteredValue_id)
{
    std::string enteredValue_item;
    std::string newValue;

    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        bool trainFound = false;

        for (auto train : root["trains"])
        {
            if (train["id"].as<std::string>() == enteredValue_id)
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
                std::ofstream fout("database/trains.yaml");

                fout << root;
                fout.close();

                std::cout << "\nTrain successfully updated!\n";

                break;
            }
        }

        if (!trainFound)
        {
            std::cerr << "This ID does not exist.\n";
            return;
        }

    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }
}

//searching by ID or train name or train destination
void Train::search(std::string enteredValue, std::string searchParams){
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        bool trainFound = false;

        const int colWidth = 20;
        std::cout << std::left 
            << std::setw(colWidth) << "ID"
            << std::setw(colWidth) << "NAME"
            << std::setw(colWidth) << "DESTINATION" << "\n";

        for (auto train : root["trains"])
        {
            if(searchParams == "I"){
                if (train["id"].as<std::string>() == enteredValue)
                {
                    trainFound = true;

                    std::cout << std::left 
                        << std::setw(colWidth) << train["id"].as<std::string>()
                        << std::setw(colWidth) << train["name"].as<std::string>()
                        << std::setw(colWidth) << train["destination"].as<std::string>() << "\n";

                    std::ofstream fout("database/trains.yaml");

                    fout << root;
                    fout.close();
                }
            }
            if(searchParams == "N"){
                if (train["name"].as<std::string>() == enteredValue)
                {
                    trainFound = true;

                    std::cout << std::left 
                        << std::setw(colWidth) << train["id"].as<std::string>()
                        << std::setw(colWidth) << train["name"].as<std::string>()
                        << std::setw(colWidth) << train["destination"].as<std::string>() << "\n";

                    std::ofstream fout("database/trains.yaml");

                    fout << root;
                    fout.close();
                }
            }
            if(searchParams == "D"){
                if (train["destination"].as<std::string>() == enteredValue)
                {
                    trainFound = true;

                    std::cout << std::left 
                        << std::setw(colWidth) << train["id"].as<std::string>()
                        << std::setw(colWidth) << train["name"].as<std::string>()
                        << std::setw(colWidth) << train["destination"].as<std::string>() << "\n";

                    std::ofstream fout("database/trains.yaml");

                    fout << root;
                    fout.close();
                }
            }
        }

        if (!trainFound)
        {
            if(searchParams == "I"){
                std::cerr << "This ID does not exist.\n";
                return;
            }
            if(searchParams == "N"){
                std::cerr << "This Train Name does not exist.\n";
                return;
            }
            if(searchParams == "D"){
                std::cerr << "This Destination does not exist.\n";
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