#ifndef ADMIN_H
#define ADMIN_H

#include "serverConnection.h"
#include "userInputHandler.h"
#include "dataParser.h"
#include <memory>
#include <string>

class Admin
{
public:
    Admin(int id, const std::string &password, ServerConnection &serverConnection);

    void mainMenu();

private:
    ServerConnection &serverConnection;
    std::shared_ptr<UserInputHandler> userInputHandler;
    std::shared_ptr<DataParser> dataParser;
    int id;
    std::string password;
    std::string role;

    void addUser();
    void deleteUser();
    void addMenu();
    void deleteMenu();
    void updateMenu();
};

#endif
