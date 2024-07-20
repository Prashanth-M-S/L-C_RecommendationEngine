#include "dataParser.h"
#include <sstream>
#include <iostream>
#include <utility>
#include <iomanip>

std::pair<bool, LoginRequest> DataParser::deserializeLoginRequest(const std::string &request)
{
    LoginRequest loginRequest;
    size_t pos = request.find(',');
    if (pos != std::string::npos)
    {
        try
        {
            loginRequest.userId = std::stoi(request.substr(0, pos));
            loginRequest.password = request.substr(pos + 1);
            return std::make_pair(true, loginRequest);
        }
        catch (const std::exception &e)
        {
            return std::make_pair(false, loginRequest);
        }
    }
    return std::make_pair(false, loginRequest);
}

std::pair<bool, GeneralRequest> DataParser::deserializeRequest(const std::string &request)
{
    std::pair<bool, GeneralRequest> result;
    GeneralRequest generalRequest;

    try
    {
        size_t pos = request.find(',');
        if (pos != std::string::npos)
        {
            generalRequest.requestType = static_cast<RequestType>(std::stoi(request.substr(0, pos)));
            generalRequest.requestData = request.substr(pos + 1);
        }
        else
        {
            generalRequest.requestType = static_cast<RequestType>(std::stoi(request));
            generalRequest.requestData = "";
        }

        result = std::make_pair(true, generalRequest);
    }
    catch (const std::exception &e)
    {
        result = std::make_pair(false, generalRequest);
    }

    return result;
}

std::pair<bool, std::string> DataParser::deserializeRecommendedMenuData(const std::vector<RecommendedMenuData> &menus)
{
    std::string foodParameter;

    for (const auto &menu : menus)
    {
        foodParameter += std::to_string(menu.menuId) + "," + menu.menuName + "," + std::to_string(menu.price) + "," + std::to_string(menu.recommendationScore) + ",";
    }

    return std::make_pair(true, foodParameter);
}

std::pair<bool, User> DataParser::deserializeAddUserRequest(const std::string &data)
{
    std::istringstream iss(data);
    std::string userIdStr, username, password, role;

    getline(iss, userIdStr, ',');
    getline(iss, username, ',');
    getline(iss, password, ',');
    getline(iss, role, ',');

    int userId = std::stoi(userIdStr);

    return {true, User(userId, username, password, role)};
}

std::pair<bool, std::vector<std::string>> DataParser::deserializeData(const std::string &data)
{
    std::istringstream iss(data);
    std::string item;
    std::vector<std::string> tokens;

    while (getline(iss, item, ','))
    {
        tokens.push_back(item);
    }

    return {true, tokens};
}

std::string DataParser::serializeData(const std::vector<DailyMenuAttributes> &items)
{
    std::ostringstream oss;

    for (const auto &item : items)
    {
        oss << item.dailyMenuId << ","
            << item.menuName << ","
            << item.availability << ","
            << item.mealCategory << ","
            << std::fixed << std::setprecision(2) << item.price << ",";
    }

    std::string result = oss.str();
    if (!result.empty())
    {
        result.pop_back();
    }

    return result;
}

std::pair<bool, Feedback> DataParser::deserializeUserFeedbackRequest(const std::string &data)
{
    std::istringstream iss(data);
    std::string item;
    std::vector<std::string> tokens;
    Feedback feedback;

    while (getline(iss, item, ','))
    {
        tokens.push_back(item);
    }

    if (tokens.size() < 4)
    {
        return {false, feedback};
    }

    try
    {
        feedback.menuId = std::stoi(tokens.at(0));
        feedback.userId = std::stoi(tokens.at(1));
        feedback.rating = std::stof(tokens.at(2));

        std::string comment;
        for (size_t i = 3; i < tokens.size(); ++i)
        {
            if (i > 3)
                comment += ",";
            comment += tokens[i];
        }

        feedback.comment = comment;
    }
    catch (const std::exception &e)
    {
        return {false, feedback};
    }

    return {true, feedback};
}

std::string DataParser::serializeData(const std::vector<Notification> &notifications)
{
    std::ostringstream oss;

    for (const auto &notification : notifications)
    {
        oss << notification.notificationId << ","
            << notification.message << ","
            << notification.dateUpdated << ";";
    }

    std::string serializedData = oss.str();
    if (!serializedData.empty())
    {
        serializedData.pop_back();
    }

    return serializedData;
}

std::pair<int, std::vector<int>> DataParser::deserializeMarkNotificationsViewedRequest(const std::string &data)
{
    std::vector<int> notificationIds;
    std::stringstream ss(data);
    std::string segment;

    std::getline(ss, segment, ',');
    int userId = std::stoi(segment);

    while (std::getline(ss, segment, ','))
    {
        notificationIds.push_back(std::stoi(segment));
    }

    return {userId, notificationIds};
}
