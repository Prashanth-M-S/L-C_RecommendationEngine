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
    virtual bool authenticateUser(int userId, const std::string &password) = 0;
    virtual bool addUser(const User& user) = 0;
    virtual bool deleteUser(int userId) = 0;
};

#endif
