#include "dataParser.h"
#include <sstream>
#include <iostream>

std::pair<std::string, std::vector<Menu>> DataParser::parseRecommendedFood(const std::string &data)
{
    std::cout << "parseRecommendedFood: " << data << std::endl;

    std::istringstream iss(data);
    std::string statusCode;
    std::getline(iss, statusCode, ',');

    std::vector<Menu> recommendedFood;
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

            recommendedFood.push_back({menuId, menuName, price});
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing menu data: " << e.what() << std::endl;
            return {statusCode, {}};
        }
    }

    return {statusCode, recommendedFood};
}