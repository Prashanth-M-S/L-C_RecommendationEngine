#ifndef EMPLOYEEINTERFACE_H
#define EMPLOYEEINTERFACE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "serverConnection.h"
#include "dataParser.h"
#include "EmployeeService.h"
#include "menu.h"
#include "userInputHandler.h"

class Employee
{
public:
    Employee(int id, const std::string &password, ServerConnection &serverConnection);

    void mainMenu();
    void viewMenu();
    void printDailyMenu(const std::vector<DailyMenuEntry> &dailyMenu);
    void placeOrder();
    void giveFeedback();
    void checkNotifications();
    void updateProfile();
    void viewProfile();
    void printProfile(const UserProfile &userProfile);
    void printNotifications(const std::vector<Notification> &notifications);
    void giveSuggestionForFood();

private:
    int id;
    std::string password;
    std::shared_ptr<UserInputHandler> userInputHandler;
    std::shared_ptr<EmployeeService> employeeService;
};

#endif