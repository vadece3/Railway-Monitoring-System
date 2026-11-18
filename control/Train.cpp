#include <iostream>
#include <string>
#include "Train.h"

void Train::enterNewTrainDetails()
{

    std::string trainId;
    std::string trainName;
    std::string trainDestination;

    std::cout << "\nEnter Train ID: ";
    std::cin >> trainId;

    std::cin.ignore();

    std::cout << "Enter Train Name: ";
    std::getline(std::cin, trainName);

    std::cout << "Enter Train Destination: ";
    std::getline(std::cin, trainDestination);

    // Display registered train
    std::cout << "\n\nTRAIN REGISTERED SUCCESSFULLY!!\n\n";

    std::cout << "Train ID: " << trainId << "\n";
    std::cout << "Train Name: " << trainName << "\n";
    std::cout << "Destination: " << trainDestination << "\n";
}
