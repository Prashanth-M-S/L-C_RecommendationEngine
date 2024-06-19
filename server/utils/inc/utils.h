#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

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

#endif