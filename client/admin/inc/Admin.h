#ifndef ADMIN_H
#define ADMIN_H

#include "serverConnection.h"
#include "userInputHandler.h"
#include "AdminService.h"
#include "dataParser.h"
#include <memory>
#include <string>

class Admin
{
public:
    Admin(int id, const std::string &password, ServerConnection &serverConnection);

    void mainMenu();
    void addUser();
    void deleteUser();
    void addMenu();
    void deleteMenu();
    void viewRecommendedmenu();
    void viewMenu();
    void printDailyMenu(const std::vector<DailyMenuEntry> &dailyMenu);

private:
    ServerConnection &serverConnection;
    std::shared_ptr<UserInputHandler> userInputHandler;
    std::shared_ptr<AdminService> adminService;
    int id;
    std::string password;
    std::string role;

};

#endif
