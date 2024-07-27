#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <string>
#include <utility>
#include "utils.h"

struct LoginRequest
{
    int userId;
    std::string password;
};

struct GeneralRequest
{
    RequestType requestType;
    std::string requestData;
};

class DataParser
{
public:
    std::pair<bool, LoginRequest> deserializeLoginRequest(const std::string &request);
    std::pair<bool, GeneralRequest> deserializeRequest(const std::string &request);
    std::pair<bool, std::string> deserializeRecommendedMenuData(const std::vector<RecommendedMenuData> &menus);
    std::pair<bool, User> deserializeAddUserRequest(const std::string &data);
    std::pair<bool, std::vector<std::string>> deserializeData(const std::string &data);
    std::string serializeData(const std::vector<DailyMenuAttributes> &items);
    std::pair<bool, Feedback> deserializeUserFeedbackRequest(const std::string &data);
    std::string serializeData(const std::vector<Notification> &notifications);
    std::pair<int, std::vector<int>> deserializeMarkNotificationsViewedRequest(const std::string &data);
    std::pair<bool, std::vector<FeedbackAnswer>> deserializeFeedbackAnswers(const std::string &data);
};

#endif