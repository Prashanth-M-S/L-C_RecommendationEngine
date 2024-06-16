#include "recommendationEngine.h"
#include "sentimentDictionary.h"
#include <string>
#include <algorithm>
#include <sstream>

RecommendationEngine::RecommendationEngine(std::shared_ptr<DatabaseInterface> database): database(database) 
{}

std::vector<Menu> RecommendationEngine::getRecommendedFood()
{
    std::vector<Menu> menus = database->fetchMenusWithFeedback();
    std::vector<Menu> recommendedMenus;

    for (auto &menu : menus)
    {
        float totalRating = 0;
        float totalSentimentScore = 0;
        int feedbackCount = menu.feedbacks.size();

        for (const auto &feedback : menu.feedbacks)
        {
            totalRating += feedback.rating;
            totalSentimentScore += calculateSentimentScore(feedback.comment);
        }

        float averageRating = feedbackCount ? totalRating / feedbackCount : 0;
        float averageSentimentScore = feedbackCount ? totalSentimentScore / feedbackCount : 0;

        if (averageRating > 3.5 && averageSentimentScore > 0.5)
        {
            recommendedMenus.push_back(menu);
        }
    }

    return recommendedMenus;
}


float RecommendationEngine::calculateSentimentScore(const std::string &feedback)
{
    std::istringstream inputStream(feedback);
    std::string word;

    int totalWords = 0;
    float totalSentimentScore = 0;
    bool shouldIntensifyNextWord = false;
    float intensifierValue = 1;

    while (inputStream >> word)
    {
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (sentimentDictionary.find(word) != sentimentDictionary.end())
        {
            const SentimentEntry &entry = sentimentDictionary.at(word);

            if (entry.flip)
            {
                shouldIntensifyNextWord = true;
                intensifierValue *= entry.score;
            }
            else
            {
                float scoreToAdd = entry.score;
                if (shouldIntensifyNextWord)
                {
                    scoreToAdd *= intensifierValue;
                    shouldIntensifyNextWord = false;
                    intensifierValue = 1;
                }
                totalSentimentScore += scoreToAdd;
                totalWords++;
            }
        }
    }

    return totalWords ? totalSentimentScore / totalWords : 0;
}
