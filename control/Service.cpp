#include <iostream>
#include <string>
#include "Service.h"

std::string Service::selectService(){
   
    Service::selectedService;
    int checker = 0;
    std::string const cars[2] = {"A","exit"};//enter the letters which will be checked after user has enterd the letter
    do
    {
        std::cout << "\n\n***Please enter a letter from the list of enter \"exit\" to end.***\n\n";
        Service::selectedService = getSelectedService();
        for (int i = 0; i <  sizeof(cars) / sizeof(cars[0]); i++)
            {
                if(selectedService == cars[i]){
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
    std::cout << "1. Enter New Train And Print Train List (Enter letter A) \n\n";

    std::cin >> enteredValue;

    return enteredValue;

}
