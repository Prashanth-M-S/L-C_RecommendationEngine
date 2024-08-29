#include <gtest/gtest.h>
#include <memory>
#include "../inc/IRecommendationEngine.h"
#include "../inc/recommendationEngine.h"
#include "MockDatabaseController.h"

class RecommendationEngineTest : public ::testing::TestWithParam<std::pair<std::string, float>>
{
};

TEST_P(RecommendationEngineTest, CalculateSentimentScore)
{
    std::shared_ptr<MockDatabaseController> databaseConstroller = std::make_shared<MockDatabaseController>();
    RecommendationEngine engine(databaseConstroller);
    auto param = GetParam();
    std::string feedback = param.first;
    float expectedScore = param.second;

    float score = engine.calculateSentimentScore(feedback);

    std::cout << "Feedback: " << feedback << std::endl;
    std::cout << "recommendation engine calculated Score: " << score << std::endl;
    std::cout << "Expected Score: " << expectedScore << std::endl;

    EXPECT_NEAR(score, expectedScore, 0.01);
}

INSTANTIATE_TEST_SUITE_P(
    CalculateSentimentScoreTests,
    RecommendationEngineTest,
    ::testing::Values(
        std::make_pair("The food was amazing and wonderful", 5.0f),
        std::make_pair("The food was horrible and disgusting", -4.5f),
        std::make_pair("The food was amazing but the service was bad", 1.0f),
        std::make_pair("The food was really amazing and very wonderful", 6.0f),
        std::make_pair("The food was good", 3.0f),
        std::make_pair("The service was fantastic", 5.0f),
        std::make_pair("The food was not good", -3.6f),
        std::make_pair("The meal was mediocre", -2.5f),
        std::make_pair("The dessert was heavenly", 5.0f),
        std::make_pair("The pasta was bland", -3.0f),
        std::make_pair("The burger was tasty", 4.0f),
        std::make_pair("The food was okay", 0.0f),
        std::make_pair("Absolutely fantastic meal", 5.0f),
        std::make_pair("Really bad experience", -3.6f),
        std::make_pair("Not tasty and not flavorful", -4.5f),
        std::make_pair("The food was not really good", -4.32f),
        std::make_pair("The dinner was very, very delicious", 7.2f)));

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
