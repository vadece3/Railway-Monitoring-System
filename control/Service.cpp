#include <iostream>
#include <string>
#include "Service.h"

std::string Service::selectService(){
   
    Service::selectedService;
    int checker = 0;
    int invalidInputChecker = 1;
    std::string const serviceOptions[6] = {"T","Tr","S","O","U","exit"};//enter the letters which will be checked after user has enterd the letter
    do
    {
        std::cout << "\n\n***Please enter a letter from the list and enter \"exit\" to end.***\n\n";
        Service::selectedService = getSelectedService();
        for (int i = 0; i <  sizeof(serviceOptions) / sizeof(serviceOptions[0]); i++)
            {
                if(selectedService == serviceOptions[i]){

                    invalidInputChecker = 0;
    
                    checker = 1;
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

std::string Service::getSelectedService(){

    std::string enteredValue;
    std::cout << "Select the service you want to use by entering the coresponding letter: \n\n";
    
    // Enter and Print Train information
    std::cout << "1. Train Settings (Enter letter T) \n\n";
    std::cout << "2. Track Settings (Enter letters Tr) \n\n";
    std::cout << "3. Display signal status (RED or GREEN) (Enter letter S) \n\n";
    std::cout << "4. Occupy a track and Release a track (Enter letter O)\n\n";
    std::cout << "5. User Settings (Enter letter U)\n\n";
    std::cout << "6. Exit (Enter exit) \n\n";
    std::cout << "->";

    std::cin >> enteredValue;

    return enteredValue;

}
