#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <string>
#include <vector>

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