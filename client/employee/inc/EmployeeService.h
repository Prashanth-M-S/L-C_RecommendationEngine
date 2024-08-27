#ifndef EMPLOYEESERVICE_H
#define EMPLOYEESERVICE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "serverConnection.h"
#include "dataParser.h"
#include "menu.h"
#include "userInputHandler.h"

class EmployeeService
{
public:
    EmployeeService(int id, ServerConnection &serverConnection);

    std::vector<DailyMenuEntry> fetchDailyMenu();
    bool isMenuItemAvailable(const std::vector<DailyMenuEntry> &dailyMenu, int dailyMenuId);
    void placeOrder(int dailyMenuId);
    int promptForDailyMenuId(const std::vector<DailyMenuEntry> &dailyMenu);
    void submitFeedback(int dailyMenuId, float rating, const std::string &comment);
    std::vector<Notification> checkNotifications();
    void updateProfile(ProfileOptions profileOptions);
    UserProfile getProfile();
    std::vector<FeedbackQuestion> fetchFeedbackQuestions();
    void giveSuggestionForFood(const std::vector<FeedbackAnswer> &feedbackAnswers);

private:
    int id;
    std::shared_ptr<DataParser> dataParser;
    std::shared_ptr<UserInputHandler> userInputHandler;
    ServerConnection &serverConnection;
};

#endif