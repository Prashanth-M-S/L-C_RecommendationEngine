#include "recommendationEngine.h"
#include "sentimentDictionary.h"
#include <string>
#include <algorithm>
#include <sstream>

std::vector<RecommendedMenuData> RecommendationEngine::getRecommendedFood()
{
    std::vector<RecommendedMenuData> recommendedMenus;
    std::vector<Menu> menus = database->fetchMenusWithFeedback();

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
        float averageSentimentScore = totalSentimentScore ? totalSentimentScore / feedbackCount : 0;
        float recommendationScore = averageRating + averageSentimentScore;

        recommendedMenus.push_back(RecommendedMenuData(menu.menuId, menu.menuName, menu.price, recommendationScore));
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

std::vector<DailyMenuAttributes> RecommendationEngine::getRecommendedFoodForUser(int userId)
{
    UserProfile userProfile = database->fetchUserProfile(userId);
    std::vector<DailyMenuAttributes> dailyMenus = database->getDailyMenuWithAttributes();
    std::vector<DailyMenuAttributes> sortedMenus = sortMenusByUserProfile(dailyMenus, userProfile);

    return sortedMenus;
}

std::vector<DailyMenuAttributes> RecommendationEngine::sortMenusByUserProfile(const std::vector<DailyMenuAttributes> &menus, const UserProfile &profile)
{
    std::vector<std::pair<DailyMenuAttributes, int>> scoredMenus;

    for (const auto &menu : menus)
    {
        int preferenceScore = 0;

        if (profile.preferenceType != "NO PREFERENCE" && profile.preferenceType == menu.dietType)
        {
            preferenceScore += 3;
        }

        if (profile.spiceLevel != "NO PREFERENCE" && profile.spiceLevel == menu.spiceLevel)
        {
            preferenceScore += 2;
        }

        if (profile.cuisinePreference != "NO PREFERENCE" && profile.cuisinePreference == menu.cuisineType)
        {
            preferenceScore += 2;
        }

        if (profile.sweetTooth != "NO PREFERENCE" &&
            ((profile.sweetTooth == "YES" && menu.sweetType == "Yes") || (profile.sweetTooth == "NO" && menu.sweetType == "No")))
        {
            preferenceScore += 1;
        }

        scoredMenus.push_back({menu, preferenceScore});
    }

    std::sort(scoredMenus.begin(), scoredMenus.end(),
              [](const std::pair<DailyMenuAttributes, int> &a, const std::pair<DailyMenuAttributes, int> &b)
              {
                  return a.second > b.second;
              });

    std::vector<DailyMenuAttributes> sortedMenus;
    for (const auto &scoredMenu : scoredMenus)
    {
        sortedMenus.push_back(scoredMenu.first);
    }

    return sortedMenus;
}