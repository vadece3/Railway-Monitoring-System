#include "Configuration.h"

#include <iostream>
#include <yaml-cpp/yaml.h>

// Initialize static variables
std::string Configuration::trainDatabase;
std::string Configuration::loginDatabase;

std::string Configuration::tempTrainDatabase;
std::string Configuration::backupTrainDatabase;

std::string Configuration::tempLoginDatabase;
std::string Configuration::backupLoginDatabase;

// Load configuration file
bool Configuration::loadConfig()
{
    try
    {
        YAML::Node config = YAML::LoadFile("app.config");

        trainDatabase = config["DB_TRAINS"].as<std::string>();
        loginDatabase = config["DB_LOGIN"].as<std::string>();

        tempTrainDatabase = config["DB_TEMP_TRAINS"].as<std::string>();
        backupTrainDatabase = config["DB_BACKUP_TRAINS"].as<std::string>();

        tempLoginDatabase = config["DB_TEMP_LOGIN"].as<std::string>();
        backupLoginDatabase = config["DB_BACKUP_LOGIN"].as<std::string>();

        return true;
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Configuration error: "
                  << e.what() << '\n';

        return false;
    }
}