
#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database
{
public:

    
    bool authenticateUser(const std::string& username,const std::string& password);
    bool authorizeAdmin(const std::string& username,const std::string& password);

    //Track
    bool updateTrackOccupancy(const std::string& trackID);
    bool deleteTrack(const std::string& trackID);
    bool addTrack(const std::string& trackID);

    //Train
    bool updateTraindetails(const std::string& trainID);
    bool deleteTrain(const std::string& trainID);
    bool addTrain(const std::string& trainID, const std::string& name, const std::string& destination);

private:
    std::string trainsDatabaseFile;
    std::string loginDatabaseFile;
    bool validateDatabaseTrack(const std::string& filePath);
    bool validateDatabaseTrain(const std::string& filePath);

};

#endif
