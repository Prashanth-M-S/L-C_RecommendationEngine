#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

#include "menuUtils.h"
#include <vector>
#include <string>

class DatabaseInterface
{
public:
    virtual ~DatabaseInterface(){};
    virtual std::vector<Menu> fetchMenusWithFeedback() = 0;
    virtual std::vector<Menu> fetchMenus() = 0;
};

#endif
