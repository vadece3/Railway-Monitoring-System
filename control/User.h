#ifndef USER_H
#define USER_H

#include <string>

class User
{
public:
    std::string userUpdates();
    
private:
    std::string selectedService;

    void addUser(std::string enteredValue_username, std::string enteredValue_userpassword);
    void viewUser();
    void deleteUser(std::string enteredValue_username);
    void search(std::string enteredValue_username);
    std::string usergetSelectedService();
};

#endif