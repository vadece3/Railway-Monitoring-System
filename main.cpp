
#include <iostream>
#include <string>
#include "control/Service.cpp"
#include "control/Train.cpp"

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
    int mainChecker = 0;

    do
    {
        std::string selectedService = selectOptiog.selectService();

        if (selectedService == "A")
        {
            Train addTrain;
            addTrain.enterNewTrainDetails();
        }
        else if (selectedService == "exit")
        {
            int mainChecker = 1;
            break;
        }

    } while (mainChecker == 0);
    
    
    std::cout << "BYE BYE \n";

    return 0;
}
