#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

class Configuration
{
public:

    static std::string trainDatabase;
    static std::string loginDatabase;

    static std::string tempTrainDatabase;
    static std::string backupTrainDatabase;
    
    static std::string tempLoginDatabase;
    static std::string backupLoginDatabase;


    static bool loadConfig();
};

#endif