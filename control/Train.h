#ifndef TRAIN_H
#define TRAIN_H

#include <string>

class Train
{
public:
    std::string* trainServiceOption(); //pointer function
    std::string getSelectedService();

    void addTrain();
    void viewTrain();
    void updateTrain();
    void deleteTrain();

private:
    std::string selectedService;
};

#endif