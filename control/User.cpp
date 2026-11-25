#include <iostream>
#include <string>
#include "User.h"
#include "Database.h"
#include "Configuration.h"
#include <iomanip>
#include <fstream>
#include <yaml-cpp/yaml.h>

std::string User::userUpdates()
{
    std::cout << "====================================\n";
    std::cout << "      RAILWAY MONITORING SYSTEM\n";
    std::cout << "====================================\n\n";
    
    std::cout << "SERVICE: User Settings\n";
    std::cout << "============================================\n\n";
    std::string enteredValue_username;
    std::string enteredValue_userpassword;
    std::string enteredValueSearchParams;
    
    std::string enteredValue_adminusername;
    std::string enteredValue_adminpassword;

    User::selectedService;
    int checker = 0;
    int invalidInputChecker = 1;
    std::string const trainOptions[6] = {"C", "R", "D", "S", "exit"};//enter the letters which will be checked after user has enterd the letter
    

    // only admin can access
    std::cout << "You must login to access user settings\n";
    std::cout << "Enter username\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_adminusername);

    std::cout << "Enter password\n";
    std::cout << "-> ";

    std::getline(std::cin >> std::ws, enteredValue_adminpassword);

    Database database;
    bool logedin = database.authorizeAdmin(enteredValue_adminusername, enteredValue_adminpassword);

    if (!logedin)
    {
        std::cerr << "User rejected.\n";
        return "exit";
    }
    std::cout << "Admin Login Successfull.\n";

    //show the page after admin login successfull
    do
    {
        viewUser();
        std::cout << "\n\n***Please enter a letter from the list and enter \"exit\" to end.***\n\n";
        User::selectedService = usergetSelectedService(); //print the selectable options
        for (int i = 0; i <  sizeof(trainOptions) / sizeof(trainOptions[0]); i++)
            {
                if(selectedService == trainOptions[i]){

                    invalidInputChecker = 0;
                        
                    if(selectedService == "C"){
                        std::cout << "\nEnter User name: ";
                        std::getline(std::cin >> std::ws, enteredValue_username);

                        std::cout << "Enter User password: ";
                        std::getline(std::cin >> std::ws, enteredValue_userpassword);

                        addUser(enteredValue_username ,enteredValue_userpassword);
                    }
                    if(selectedService == "R"){
                        viewUser();
                    }
                    if(selectedService == "D"){
                        std::cout << "\n------Enter the User's name to delete-------\n";
                        std::cout << "->";

                        std::getline(std::cin >> std::ws, enteredValue_username);

                        deleteUser(enteredValue_username);
                    }
                    if(selectedService == "S"){
                        std::cout << "\n------Enter the Username you want to search-------\n";
                        std::cout << "->";
                        std::getline(std::cin >> std::ws, enteredValue_username);

                        search(enteredValue_username);
                    }   
                    if(selectedService == "exit"){
                        checker = 1;
                        break;
                    }
                    break;
                }
            }
        if(checker == 0 && invalidInputChecker == 1){
            std::cerr << "\nInvalid input\n";
        }
        invalidInputChecker = 1;
    } while (checker == 0);

   return selectedService;
   
}

//selection menu
std::string User::usergetSelectedService(){

    std::string enteredValue;
    std::cout << "Select the service you want to use by entering the coresponding letter: \n\n";
    
    // Enter and Print Train information
    std::cout << "1. Add New User (Enter letter C) \n\n";
    std::cout << "2. View User List (Enter letter R) \n\n";
    std::cout << "3. Delete User (Enter letter D) \n\n";
    std::cout << "4. Search for a particular User (Enter letter S) \n\n";
    std::cout << "5. Exit (Enter exit) \n\n";
    std::cout << "->";

    std::cin >> enteredValue;

    return enteredValue;

}

//adding new user
void User::addUser(std::string enteredValue_username, std::string enteredValue_password){
    Database database;
    bool addUser = database.addNewUser(enteredValue_username,enteredValue_password);
    
    if (addUser)
    {
        std::cout << "User was successfully added\n";
        return;
    }
    if (!addUser)
    {
        std::cerr << "Failed to add User\n";
        return;
    }  
}

//viewing all users
void User::viewUser(){
    try
    {
        //set database file path
        if (!Configuration::loadConfig())
        {
            std::cerr << "Config Data Failure: Unable to view data from database.\n";
            return;
        }
        std::string loginFile = Configuration::loginDatabase;

        YAML::Node root = YAML::LoadFile(loginFile);

        std::cout << "====================================\n";
        std::cout << "      USERS DETAILS\n";
        std::cout << "====================================\n";

        //Print table
        const int colWidth = 20;
        std::cout << std::left 
            << std::setw(colWidth) << "USER NAME"
            << std::setw(colWidth) << "PASSWORD" << "\n";

        for (const auto& user : root["Users"])
        {  
            std::cout << std::left 
                << std::setw(colWidth) << user["username"].as<std::string>()
                << std::setw(colWidth) << user["password"].as<std::string>() << "\n";
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Error reading user database: "
                  << e.what() << "\n";
    }
}

//deleting train with respect to the username
void User::deleteUser(std::string enteredValue_username){

    Database database;
    bool deleteUser = database.deleteUser(enteredValue_username);
    
    if (deleteUser)
    {
        std::cout << "User was successfully deleted\n";
        return;
    }
    if (!deleteUser)
    {
        std::cerr << "Failed to delete User\n";
        return;
    }
}

//searching by username
void User::search(std::string enteredValue_username){
    try
    {
        //set database file path
        if (!Configuration::loadConfig())
        {
            std::cerr << "Config Data Failure: Unable to view data from database.\n";
            return;
        }
        std::string loginFile = Configuration::trainDatabase;

        YAML::Node root = YAML::LoadFile(loginFile);

        bool usernameFound = false;

        const int colWidth = 20;
        std::cout << std::left 
            << std::setw(colWidth) << "USER NAME"
            << std::setw(colWidth) << "PASSWORD" << "\n";

        for (auto user : root["Users"])
        {
            if (user["username"].as<std::string>() == enteredValue_username)
                {
                    usernameFound = true;

                    std::cout << std::left 
                        << std::setw(colWidth) << user["username"].as<std::string>()
                        << std::setw(colWidth) << user["password"].as<std::string>() << "\n";

                    std::ofstream fout(loginFile);

                    fout << root;
                    fout.close();
                }
        }

        if (!usernameFound)
        {
            std::cerr << "This Username does not exist.\n";
            return;  
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "YAML error: " << e.what() << '\n';
    }

}