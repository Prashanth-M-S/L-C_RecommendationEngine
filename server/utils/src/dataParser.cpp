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
    GeneralRequest generalRequest;
    size_t pos = request.find(',');
    if (pos != std::string::npos)
    {
        generalRequest.requestType = request.substr(0, pos);
        generalRequest.requestData = request.substr(pos + 1);
    }
    else
    {
        generalRequest.requestType = request;
        generalRequest.requestData = "";
    }

    if (!generalRequest.requestType.empty())
    {
        return std::make_pair(true, generalRequest);
    }
    else
    {
        return std::make_pair(false, generalRequest);
    }
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

std::string DataParser::serializeData(const std::vector<GetDailyMenu> &items)
{
    std::ostringstream oss;

    for (const auto &item : items)
    {
        oss << item.dailyMenuId << ","
            << item.itemName << ","
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