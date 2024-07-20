#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "serverConnection.h"
#include "dataParser.h"
#include "menu.h"
#include "userInputHandler.h"

class Employee
{
public:
    Employee(int id, const std::string &password, ServerConnection &serverConnection);

    void mainMenu();
    void placeOrder();
    void giveFeedback();
    void checkNotifications();
    void updateProfile();
    void viewProfile();
    std::vector<DailyMenuEntry> viewMenu();
    void printDailyMenu(const std::vector<DailyMenuEntry> &dailyMenu);
    void printNotifications(const std::vector<Notification>& notifications);
    void printProfile(const UserProfile &userProfile);
    std::pair<std::string, std::vector<DailyMenuEntry>> fetchDailyMenu();

private:
    ServerConnection &serverConnection;
    std::shared_ptr<DataParser> dataParser;
    std::shared_ptr<UserInputHandler> userInputHandler;

    int id;
    std::string password;
    std::string role;
};

#endif