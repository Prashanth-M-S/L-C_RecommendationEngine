#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

enum class RequestType
{
    LOGIN,
    GET_RECOMMENDED_FOOD,
    ADD_USER,
    DELETE_USER,
    ADD_MENU,
    DELETE_MENU,
    ROLLOUT_MENU,
    GET_DAILY_MENU,
    PLACE_ORDER,
    ADD_FEEDBACK,
    GET_NOTIFICATIONS,
    MARK_NOTIFICATIONS_VIEWED,
    UPDATE_PROFILE,
    VIEW_PROFILE,
    SET_DAILY_MENU_AVAILABILITY_ZERO,
    FETCH_FEEDBACK,
    ADD_FEEDBACK_QUESTION,
    FETCH_FEEDBACK_QUESTIONS,
    ADD_SUGGESTION_FOR_FOOD,
    FETCH_SUGGESTIONS_FOR_MENU,
    UNKNOWN
};

struct MenuData
{
    int menuId;
    std::string menuName;
    float price;
};

struct RecommendedMenuData : public MenuData
{
    float recommendationScore;

    RecommendedMenuData() {}
    RecommendedMenuData(int id, const std::string &name, float p, float score)
        : MenuData{id, name, p}, recommendationScore{score} {}
};

struct Feedback
{
    int feedbackId;
    int menuId;
    int userId;
    float rating;
    std::string comment;
    std::string date;
};

struct Menu
{
    int menuId;
    std::string menuName;
    float price;
    std::vector<Feedback> feedbacks;
};

class DailyMenuEntry
{
public:
    int dailyMenuId;
    std::string itemName;
    int availability;
    std::string mealCategory;
    double price;
};

struct Notification
{
    int notificationId;
    std::string message;
    std::string dateUpdated;
};

struct UserProfile
{
    int userId;
    std::string preferenceType;
    std::string spiceLevel;
    std::string cuisinePreference;
    std::string sweetTooth;
};

struct FeedbackQuestion
{
    int id;
    std::string text;
};

struct FeedbackAnswer
{
    int questionId;
    int foodId;
    int employeeId;
    std::string answerText;
};

#endif