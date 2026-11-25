#ifndef TRACK_H
#define TRACK_H

#include <string>

class Track
{
public:
    std::string* trackServiceOption(); //pointer function
    std::string trackgetSelectedService();
    std::string trackStatusChange();

    void addTrack(std::string enteredValue_id);
    void viewTrack();
    void updateTrack(std::string enteredValue_id);
    void deleteTrack(std::string enteredValue_id);
    void search(std::string enteredValue_id, std::string searchParams);

private:
    std::string selectedService;
};

#endif