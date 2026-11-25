#ifndef TRAIN_H
#define TRAIN_H

#include <string>

class Train
{
public:
    std::string* trainServiceOption(); //pointer function


private:
    std::string selectedService;
    
    void addTrain(std::string enteredValue_id,std::string enteredValue_name,std::string enteredValue_destination);
    void viewTrain();
    void updateTrain(std::string enteredValue_id);
    void deleteTrain(std::string enteredValue_id);
    void search(std::string enteredValue_id, std::string searchParams);
    std::string getSelectedService();

};

#endif