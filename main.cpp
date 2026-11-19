
#include <iostream>
#include <string>
#include "control/Service.h"
#include "control/Train.h"

int main()
{
    std::cout << "====================================\n";
    std::cout << "      RAILWAY MONITORING SYSTEM\n";
    std::cout << "====================================\n";

    std::cout << "\nWelcome to the Railway Monitoring System!\n";

    std::string railwayName = "Railway Network";

    std::cout << "System: " << railwayName << "\n\n";

    // List of available services
    std::cout << "Service List:\n";
    std::cout << "====================================\n";
    Service selectOptiog;
    Train trainService;
    int mainChecker = 0;

    do
    {
        std::string selectedService = selectOptiog.selectService();

        if (selectedService == "T")
        {
            std::string* trainResult = trainService.trainServiceOption();
            if(*trainResult == "exit"){
                continue;
            }
        }
        else if (selectedService == "exit")
        {
            mainChecker = 1;
            break;
        }

    } while (mainChecker == 0);
    
    
    std::cout << "BYE BYE \n";

    return 0;
}
