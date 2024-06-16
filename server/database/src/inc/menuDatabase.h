#ifndef MENU_DATABASE_H
#define MENU_DATABASE_H

#include "../../pub/databaseInterface.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <memory>

class MenuDatabase : public DatabaseInterface
{
public:
    MenuDatabase(const std::string &dbHost = "tcp://127.0.0.1", const std::string &user = "prashanth",
                 const std::string &password = "myroot", const std::string &database = "recommendationEngine");
    std::vector<Menu> fetchMenusWithFeedback() override;
    std::vector<Menu> fetchMenus() override;
    void fetchFeedbacks(Menu &menu);

private:
    void connect();

    std::string dbHost;
    std::string user;
    std::string password;
    std::string database;
    std::unique_ptr<sql::Connection> connection;
};

#endif