
#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database
{
public:

    //Track
    bool authenticateUser(const std::string& username,const std::string& password);

    bool authorizeAdmin(const std::string& username,const std::string& password);

    bool updateTrackOccupancy(const std::string& trackID);

    bool deleteTrack(const std::string& trackID);

    bool addTrack(const std::string& trackID);

private:
    std::string trainsDatabaseFile;
    std::string loginDatabaseFile;
    bool validateDatabaseTrack(const std::string& filePath);

};

#endif
