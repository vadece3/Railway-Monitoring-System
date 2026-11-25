
#include <iostream>
#include <string>
#include "control/Service.h"
#include "control/Train.h"
#include "control/Track.h"
#include "control/Signal.h"
#include "control/User.h"

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
    Track trackService;
    Signal signalService;
    User userService;
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
        else if (selectedService == "Tr")
        {
            std::string* trackResult = trackService.trackServiceOption();
            if(*trackResult == "exit"){
                continue;
            }
        }
        else if (selectedService == "S")
        {
            std::string* signalResult = signalService.signalServiceOption();
            if(*signalResult == "exit"){
                continue;
            }
        }
        else if (selectedService == "O")
        {
            std::string trackResult = trackService.trackStatusChange();
            if(trackResult == "exit"){
                continue;
            }
        }
        else if (selectedService == "U")
        {
            std::string userUpdates = userService.userUpdates();
            if(userUpdates == "exit"){
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
