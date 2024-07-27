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
    virtual void fetchFeedbacks(Menu &menu) = 0;
    virtual std::string authenticateUser(int userId, const std::string &password) = 0;
    virtual bool addUser(const User &user) = 0;
    virtual bool deleteUser(int userId) = 0;
    virtual bool addMenu(const MenuAttributes &menu) = 0;
    virtual bool deleteMenu(int menuId) = 0;
    virtual bool insertDailyMenuEntries(const std::vector<DailyMenuEntry> &dailyMenuEntry) = 0;
    virtual std::vector<DailyMenuAttributes> getDailyMenu() = 0;
    virtual bool insertUserOrderEntries(const std::vector<UserOrderEntry> &userOrderEntries) = 0;
    virtual bool insertUserFeedback(const Feedback &feedback) = 0;
    virtual bool insertNotification(const std::string &message) = 0;
    virtual bool addNotification(const std::string &message) = 0;
    virtual std::vector<Notification> getNonViewedNotificationsForUser(int userId) = 0;
    virtual bool markNotificationsAsViewed(int userId, const std::vector<int> &notificationIds) = 0;
    virtual UserProfile fetchUserProfile(int userId) = 0;
    virtual std::vector<DailyMenuAttributes> getDailyMenuWithAttributes() = 0;
    virtual bool updateUserProfile(const UserProfile &profile) = 0;
    virtual UserProfile getUserProfile(int userId) = 0;
    virtual bool setDailyMenuAvailabilityToZero(int dailyMenuId) = 0;
    virtual bool addFeedbackQuestion(const std::string &questionText) = 0;
};

#endif
