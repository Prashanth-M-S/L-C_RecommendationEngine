#ifndef RECOMMENDATIONENGINE_H
#define RECOMMENDATIONENGINE_H

#include "../../database/pub/databaseInterface.h"
#include "IRecommendationEngine.h"

#include <memory>

class RecommendationEngine : public IRecommendationEngine
{
public:
    RecommendationEngine(std::shared_ptr<DatabaseInterface> database);
    std::vector<Menu> getRecommendedFood() override;

private:
    float calculateSentimentScore(const std::string &feedback);

private:
    std::shared_ptr<DatabaseInterface> database;
};

#endif