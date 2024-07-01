#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

struct MenuData
{
    int menuId;
    std::string menuName;
    float price;
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

struct DailyMenuEntry
{
    int menuId;
    int availability;
    std::string mealCategory;
};

class GetDailyMenu
{
public:
    int dailyMenuId;
    std::string itemName;
    int availability;
    std::string mealCategory;
    double price;
};

#endif