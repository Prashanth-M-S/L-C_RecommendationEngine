#ifndef IRECOMENDATION_ENGINE
#define IRECOMENDATION_ENGINE

#include "utils.h"
#include <string>
#include <vector>

class IRecommendationEngine
{
public:
    virtual ~IRecommendationEngine() {}
    virtual std::vector<RecommendedMenuData> getRecommendedFood() = 0;
};

#endif