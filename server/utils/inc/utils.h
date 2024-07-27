#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

enum class RequestType
{
    LOGIN,
    GET_RECOMMENDED_FOOD,
    ADD_USER,
    DELETE_USER,
    ADD_MENU,
    DELETE_MENU,
    ROLLOUT_MENU,
    GET_DAILY_MENU,
    PLACE_ORDER,
    ADD_FEEDBACK,
    GET_NOTIFICATIONS,
    MARK_NOTIFICATIONS_VIEWED,
    UPDATE_PROFILE,
    VIEW_PROFILE,
    SET_DAILY_MENU_AVAILABILITY_ZERO,
    FETCH_FEEDBACK,
    ADD_FEEDBACK_QUESTION,
    UNKNOWN
};

struct MenuData
{
    int menuId;
    std::string menuName;
    float price;
};

struct MenuAttributes : public MenuData
{
    std::string dietType;
    std::string spiceLevel;
    std::string cuisineType;
    std::string sweetType;
};

struct RecommendedMenuData : public MenuData
{
    float recommendationScore;

    RecommendedMenuData(int id, const std::string &name, float p, float score)
        : MenuData{id, name, p}, recommendationScore{score} {}
};

struct Feedback
{
    int feedbackId;
    int menuId;
    int userId;
    float rating;
    std::string comment;
    std::string feedbackDate;
};

struct Menu
{
    int menuId;
    std::string menuName;
    float price;
    std::vector<Feedback> feedbacks;
};

struct User
{
    int userId;
    std::string username;
    std::string password;
    std::string role;

    User(int id, const std::string &name, const std::string &password, const std::string &roll)
        : userId(id), username(name), password(password), role(roll) {}
};

struct UserProfile
{
    int userId;
    std::string preferenceType;
    std::string spiceLevel;
    std::string cuisinePreference;
    std::string sweetTooth;
};

struct DailyMenuEntry
{
    int menuId;
    int availability;
    std::string mealCategory;
};

class DailyMenuAttributes : public MenuAttributes
{
public:
    int dailyMenuId;
    std::string itemName;
    int availability;
    std::string mealCategory;
    double price;
};

struct UserOrderEntry
{
    int userId;
    int dailyMenuId;
};

struct Notification
{
    int notificationId;
    std::string message;
    std::string dateUpdated;
};

#endif