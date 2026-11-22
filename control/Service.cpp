#include <iostream>
#include <string>
#include "Service.h"

std::string Service::selectService(){
   
    Service::selectedService;
    int checker = 0;
    std::string const serviceOptions[3] = {"T","Tr","exit"};//enter the letters which will be checked after user has enterd the letter
    do
    {
        std::cout << "\n\n***Please enter a letter from the list and enter \"exit\" to end.***\n\n";
        Service::selectedService = getSelectedService();
        for (int i = 0; i <  sizeof(serviceOptions) / sizeof(serviceOptions[0]); i++)
            {
                if(selectedService == serviceOptions[i]){
                    checker = 1;
                    break;
                }
            }
    } while (checker == 0);
    
   return selectedService;
   
}

std::string Service::getSelectedService(){

    std::string enteredValue;
    std::cout << "Select the service you want to use by entering the coresponding letter: \n\n";
    
    // Enter and Print Train information
    std::cout << "1. Train Settings (Enter letter T) \n\n";
    std::cout << "2. Track Settings (Enter letter Tr) \n\n";
    std::cout << "3. Exit (Enter exit) \n\n";
    std::cout << "->";

    std::cin >> enteredValue;

    return enteredValue;

}
