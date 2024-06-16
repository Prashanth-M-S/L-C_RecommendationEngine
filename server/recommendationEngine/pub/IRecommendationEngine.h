#ifndef IRECOMENDATION_ENGINE
#define IRECOMENDATION_ENGINE

#include "menuUtils.h"
#include <string>
#include <vector>

class IRecommendationEngine
{
public:
    virtual ~IRecommendationEngine() {}
    virtual std::vector<Menu> getRecommendedFood() = 0;
};

#endif