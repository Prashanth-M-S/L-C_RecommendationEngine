#ifndef MOCKDATABASECONTROLLER_H
#define MOCKDATABASECONTROLLER_H

#include "IDatabaseController.h"
#include <gmock/gmock.h>

class MockDatabaseController : public IDatabaseController
{
public:
    MOCK_METHOD(std::vector<Menu>, fetchMenusWithFeedback, (), (override));
    MOCK_METHOD(std::vector<Menu>, fetchMenus, (), (override));
    MOCK_METHOD(std::string, authenticateUser, (int userId, const std::string &password), (override));
    MOCK_METHOD(bool, addUser, (const User &user), (override));
    MOCK_METHOD(bool, deleteUser, (int userId), (override));
    MOCK_METHOD(bool, addMenu, (const MenuAttributes &menu), (override));
    MOCK_METHOD(bool, deleteMenu, (int menuId), (override));
    MOCK_METHOD(bool, insertDailyMenuEntries, (const std::vector<DailyMenuEntry> &dailyMenuEntry), (override));
    MOCK_METHOD(std::vector<DailyMenuAttributes>, getDailyMenu, (), (override));
    MOCK_METHOD(bool, insertUserOrderEntries, (const std::vector<UserOrderEntry> &userOrderEntries), (override));
    MOCK_METHOD(bool, insertUserFeedback, (const Feedback &feedback), (override));
    MOCK_METHOD(bool, insertNotification, (const std::string &message), (override));
    MOCK_METHOD(bool, addNotification, (const std::string &message), (override));
    MOCK_METHOD(std::vector<Notification>, getNonViewedNotificationsForUser, (int userId), (override));
    MOCK_METHOD(bool, markNotificationsAsViewed, (int userId, const std::vector<int> &notificationIds), (override));
    MOCK_METHOD(UserProfile, fetchUserProfile, (int userId), (override));
    MOCK_METHOD(std::vector<DailyMenuAttributes>, getDailyMenuWithAttributes, (), (override));
    MOCK_METHOD(bool, updateUserProfile, (const UserProfile &profile), (override));
    MOCK_METHOD(UserProfile, getUserProfile, (int userId), (override));
    MOCK_METHOD(bool, setDailyMenuAvailabilityToZero, (int dailyMenuId), (override));
};

#endif
