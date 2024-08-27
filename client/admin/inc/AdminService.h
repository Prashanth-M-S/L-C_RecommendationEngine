#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "serverConnection.h"
#include "userInputHandler.h"
#include "dataParser.h"
#include <memory>
#include <string>

class AdminService
{
public:
    AdminService(int id, ServerConnection &serverConnection);
    std::string addUser(UserData userData);
    std::string AdminService::deleteUser(int userIdToDelet);
    std::string AdminService::addMenu(std::string menuName, float menuPrice, ProfileOptions profileOptions);
    std::string AdminService::deleteMenu(int menuId);
    std::pair<std::string, std::vector<RecommendedMenuData>>AdminService::getRecommendedmenu();
    std::pair<std::string, std::vector<DailyMenuEntry>> AdminService::fetchDailyMenu();

private:
    int id;
    std::shared_ptr<DataParser> dataParser;
    ServerConnection &serverConnection;
};

#endif