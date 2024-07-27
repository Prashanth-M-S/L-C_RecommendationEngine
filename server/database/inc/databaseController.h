#ifndef DATABASECONTOLLER_H
#define DATABASECONTOLLER_H

#include "IDatabaseController.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <memory>
#include "IDatabaseController.h"

class DatabaseController : public IDatabaseController
{
public:
    DatabaseController(const std::string &dbHost = "tcp://127.0.0.1", const std::string &user = "prashanth",
                       const std::string &password = "myroot", const std::string &database = "recommendationEngine");
    std::vector<Menu> fetchMenusWithFeedback() override;
    std::vector<Menu> fetchMenus() override;
    void fetchFeedbacks(Menu &menu);
    std::string authenticateUser(int userId, const std::string &password);
    bool addUser(const User &user);
    bool deleteUser(int userId);
    bool addMenu(const MenuAttributes &menu);
    bool deleteMenu(int menuId);
    bool insertDailyMenuEntries(const std::vector<DailyMenuEntry> &dailyMenuEntry);
    std::vector<DailyMenuAttributes> getDailyMenu();
    bool insertUserOrderEntries(const std::vector<UserOrderEntry> &userOrderEntries);
    int getMenuIdFromDailyMenuId(int dailyMenuId);
    bool insertUserFeedback(const Feedback &feedback);
    bool insertNotification(const std::string &message);
    bool addNotification(const std::string &message);
    std::vector<Notification> getNonViewedNotificationsForUser(int userId);
    bool markNotificationsAsViewed(int userId, const std::vector<int> &notificationIds);
    UserProfile fetchUserProfile(int userId);
    std::vector<DailyMenuAttributes> getDailyMenuWithAttributes();
    bool updateUserProfile(const UserProfile &profile);
    UserProfile getUserProfile(int userId);
    bool setDailyMenuAvailabilityToZero(int dailyMenuId);
    bool addFeedbackQuestion(const std::string &questionText);

private:
    void connect();

    std::string dbHost;
    std::string user;
    std::string password;
    std::string database;
    std::unique_ptr<sql::Connection> connection;
};

#endif