#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <vector>
#include <string>
#include "menu.h"

class DataParser
{
public:
    std::pair<std::string, std::vector<Menu>> parseRecommendedFood(const std::string &data);
};

#endif
