#ifndef RECOMMENDATIONENGINE_H
#define RECOMMENDATIONENGINE_H

#include "IDatabaseController.h"
#include "IRecommendationEngine.h"

#include <memory>

class RecommendationEngine : public IRecommendationEngine
{
public:
    RecommendationEngine(std::shared_ptr<IDatabaseController> database):  database(database) {}
    std::vector<RecommendedMenuData> getRecommendedFood() override;
    std::vector<DailyMenuAttributes> getRecommendedFoodForUser(int userId);

private:
    float calculateSentimentScore(const std::string &feedback);
    std::vector<DailyMenuAttributes> sortMenusByUserProfile(const std::vector<DailyMenuAttributes> &menus, const UserProfile &profile);

private:
    std::shared_ptr<IDatabaseController> database;
};

#endif