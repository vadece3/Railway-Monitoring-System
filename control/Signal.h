#ifndef SIGNAL_H
#define SIGNAL_H

#include <string>

class Signal
{
public:
    std::string* signalServiceOption(); //pointer function

private:std::
    string selectedService;
    
    std::string signalgetSelectedService();
    void viewAllSignalOccupancy();
    void viewOccupiedSignals();
    void viewfreeSignals();
    void viewsearchedIDSignals(std::string enteredValue_id);
};

#endif