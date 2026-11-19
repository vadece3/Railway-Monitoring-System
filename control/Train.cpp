#include <iostream>
#include <string>
#include "Train.h"
#include <iomanip>
#include <fstream>
#include <yaml-cpp/yaml.h>

std::string* Train::trainServiceOption()
{
    viewTrain();
    Train::selectedService;
    int checker = 0;
    std::string const trainOptions[5] = {"C", "R", "U", "D", "exit"};//enter the letters which will be checked after user has enterd the letter
    do
    {
        viewTrain();
        std::cout << "\n\n***Please enter a letter from the list and enter \"exit\" to end.***\n\n";
        Train::selectedService = getSelectedService();
        for (int i = 0; i <  sizeof(trainOptions) / sizeof(trainOptions[0]); i++)
            {
                if(selectedService == trainOptions[i]){
                    if(selectedService == "C"){
                        addTrain();
                    }
                    if(selectedService == "R"){
                        viewTrain();
                    }
                    if(selectedService == "U"){
                        updateTrain();
                    }
                    if(selectedService == "D"){
                        deleteTrain();
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

std::string Train::getSelectedService(){

    std::string enteredValue;
    std::cout << "Select the service you want to use by entering the coresponding letter: \n\n";
    
    // Enter and Print Train information
    std::cout << "1. Add New Train (Enter letter C) \n\n";
    std::cout << "2. View Train List (Enter letter R) \n\n";
    std::cout << "3. Update Train Details (Enter letter U) \n\n";
    std::cout << "4. Delete Train (Enter letter D) \n\n";
    std::cout << "->";

    std::cin >> enteredValue;

    return enteredValue;

}

void Train::addTrain(){

    std::string id;
    std::string name;
    std::string destination;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nEnter Train ID: ";
    std::getline(std::cin, id);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter Train Name: ";
    std::getline(std::cin, name);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter Train Destination: ";
    std::getline(std::cin, destination);

    // Display registered train
    std::cout << "\n\nTRAIN REGISTERED SUCCESSFULLY!!\n\n";
    try
    {
        

        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        YAML::Node train;
        train["id"] = id;
        train["name"] = name;
        train["destination"] = destination;

        root["trains"].push_back(train);

        std::ofstream fout("database/trains.yaml");
        fout << root;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Error reading train database: "
                  << e.what() << "\n";
    }
    viewTrain();
}

void Train::viewTrain(){
    try
    {
        YAML::Node root = YAML::LoadFile("database/trains.yaml");

        std::cout << "====================================\n";
        std::cout << "      TRAIN DETAILS\n";
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

void Train::deleteTrain(){

    viewTrain();

    std::string enteredValue_id;

    std::cout << "\n------Enter the ID of the train you want to delete-------\n";
    std::cout << "->";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, enteredValue_id);

    YAML::Node root = YAML::LoadFile("database/trains.yaml");

    YAML::Node updated(YAML::NodeType::Sequence);

    for (const auto& train : root["trains"])
    {
        if (train["id"].as<std::string>() != enteredValue_id)
        {
            updated.push_back(train);
        }
    }

    root["trains"] = updated;

    std::ofstream fout("database/trains.yaml");
    fout << root;

}

void Train::updateTrain()
{
    std::string enteredValue_id;
    std::string enteredValue_item;
    std::string newValue;

    std::cout << "\n------ Enter the ID of the train you want to update ------\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_id);

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
                    std::cout << "This option is not available.\n";
                    return;
                }

                break;
            }
        }

        if (!trainFound)
        {
            std::cout << "This ID does not exist.\n";
            return;
        }

        std::ofstream fout("database/trains.yaml");

        if (!fout)
        {
            std::cerr << "Error: Could not open train database for writing.\n";
            return;
        }

        fout << root;
        fout.close();

        std::cout << "\nTrain successfully updated!\n";
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }
}