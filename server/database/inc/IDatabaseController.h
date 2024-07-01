#ifndef IDATABASECONTOLLER_H
#define IDATABASECONTOLLER_H

#include "utils.h"
#include <vector>
#include <string>

class IDatabaseController
{
public:
    virtual ~IDatabaseController(){};
    virtual std::vector<Menu> fetchMenusWithFeedback() = 0;
    virtual std::vector<Menu> fetchMenus() = 0;
    virtual std::string authenticateUser(int userId, const std::string &password) = 0;
    virtual bool addUser(const User& user) = 0;
    virtual bool deleteUser(int userId) = 0;
    virtual bool addMenu(const MenuData& menu) = 0;
    virtual bool deleteMenu(int menuId) = 0;
    virtual bool insertDailyMenuEntries(const std::vector<DailyMenuEntry> &dailyMenuEntry) = 0;
    virtual std::vector<GetDailyMenu> getDailyMenu() = 0;
};

#endif
