#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <vector>
#include <string>
#include "menu.h"

class DataParser
{
public:
    std::pair<std::string, std::vector<RecommendedMenuData>> parseRecommendedFood(const std::string &data);
    std::pair<bool, std::vector<std::string>> deserializeData(const std::string &data);
};

#endif
