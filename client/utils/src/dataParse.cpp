#include "dataParser.h"
#include <sstream>
#include <iostream>

std::pair<std::string, std::vector<RecommendedMenuData>> DataParser::parseRecommendedFood(const std::string &data)
{
    std::istringstream iss(data);
    std::string statusCode;
    std::getline(iss, statusCode, ',');

    std::vector<RecommendedMenuData> recommendedFood;
    std::string menuData;

    while (std::getline(iss, menuData, ','))
    {
        try
        {
            int menuId = std::stoi(menuData);

            std::string menuName;
            if (!std::getline(iss, menuName, ','))
            {
                throw std::runtime_error("Missing menu name");
            }

            std::string priceStr;
            if (!std::getline(iss, priceStr, ','))
            {
                throw std::runtime_error("Missing price");
            }

            float price = std::stof(priceStr);

            std::string recommendationScoreStr;
            if (!std::getline(iss, recommendationScoreStr, ','))
            {
                throw std::runtime_error("Missing recommendation score");
            }

            float recommendationScore = std::stof(recommendationScoreStr);

            recommendedFood.push_back({menuId, menuName, price, recommendationScore});
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing menu data: " << e.what() << std::endl;
            return {statusCode, {}};
        }
    }

    return {statusCode, recommendedFood};
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

std::pair<std::string, std::vector<DailyMenuEntry>> DataParser::deserializeToDailyMenuEntries(const std::string &data)
{
    std::vector<DailyMenuEntry> items;
    std::istringstream iss(data);
    std::string token;
    std::string statusCode;
    std::getline(iss, statusCode, ',');

    if (statusCode != "STATUS_OK")
    {
        items.clear();
    }
    else
    {
        while (getline(iss, token, ','))
        {
            DailyMenuEntry entry;
            entry.dailyMenuId = std::stoi(token);

            getline(iss, token, ',');
            entry.itemName = token;

            getline(iss, token, ',');
            entry.availability = std::stoi(token);

            getline(iss, token, ',');
            entry.mealCategory = token;

            getline(iss, token, ',');
            entry.price = std::stod(token);

            items.push_back(entry);
        }
    }

    return {statusCode, items};
}