#ifndef SERVICE_H
#define SERVICE_H

#include <string>

class Service{
    public:  
        std::string selectService();
        std::string getSelectedService();
        std::string selectedService;

};
#endif