#ifndef MENU_H
#define MENU_H

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
    std::string date;
};

struct Menu
{
    int menuId;
    std::string menuName;
    float price;
    std::vector<Feedback> feedbacks;
};

#endif