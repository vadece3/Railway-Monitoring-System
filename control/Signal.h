#ifndef SIGNAL_H
#define SIGNAL_H

#include <string>

class Signal
{
public:
    std::string* signalServiceOption(); //pointer function
    std::string signalgetSelectedService();

    void viewAllSignalOccupancy();
    void viewOccupiedSignals();
    void viewfreeSignals();
    void viewsearchedIDSignals(std::string enteredValue_id);

private:
    std::string selectedService;
};

#endif