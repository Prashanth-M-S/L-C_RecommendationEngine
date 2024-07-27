#ifndef CHEF_H
#define CHEF_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "serverConnection.h"
#include "dataParser.h"
#include "menu.h"
#include "userInputHandler.h"

class Chef
{
public:
    Chef(int id, const std::string &password, ServerConnection &serverConnection);

    void mainMenu();
    void rolloutMenu();
    std::vector<DailyMenuEntry> viewMenu();
    void printRecommendedFood(const std::vector<RecommendedMenuData> &recommendedFood);
    std::vector<RecommendedMenuData>  fetchRecommendedFood();
    void printDailyMenu(const std::vector<DailyMenuEntry> &dailyMenu);
    void setMenuAvailabilityToZero();
    void fetchMenuFeedbacks();
    void deleteMenuItem();
    void writeSuggestionQuestion();

private:
    ServerConnection &serverConnection;
    std::shared_ptr<DataParser> dataParser;
    std::shared_ptr<UserInputHandler> userInputHandler;

    int id;
    std::string password;
    std::string role;
};

#endif
