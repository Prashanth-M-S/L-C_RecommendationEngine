#include "requestHandler.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE (1024)

RequestHandler::RequestHandler(std::shared_ptr<IDatabaseController> database, std::shared_ptr<IRecommendationEngine> recommendationEngine)
    : database(database), recommendationEngine(recommendationEngine), dataParser(std::make_shared<DataParser>()) {}

void RequestHandler::handleRequest(int new_socket)
{
    while (true)
    {
        std::string request = readFromSocket(new_socket);
        if (request.empty())
        {
            return;
        }

        std::pair<bool, GeneralRequest> requestAndData = dataParser->deserializeRequest(request);

        if (requestAndData.first)
        {
            std::string response = processRequest(requestAndData.second);
            int bytesSent = write(new_socket, response.c_str(), response.length());
            if (bytesSent < 0)
            {
                perror("handleRequest: Error writing to socket");
            }
        }
        else
        {
            std::string response = "parse error";
            int bytesSent = write(new_socket, response.c_str(), response.length());
            if (bytesSent < 0)
            {
                perror("Error writing to socket");
            }
        }
    }
}

std::string RequestHandler::readFromSocket(int new_socket)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    int bytesReceived = read(new_socket, buffer, BUFFER_SIZE - 1);
    if (bytesReceived < 0)
    {
        return "";
    }

    return std::string(buffer);
}

std::string RequestHandler::processRequest(const GeneralRequest &request)
{
    if (request.requestType == "LOGIN")
    {
        return handleLoginRequest(request.requestData);
    }
    else if (request.requestType == "GET_RECOMMENDED_FOOD")
    {
        return handleGetRecommendedFoodRequest();
    }
    else if (request.requestType == "ADD_USER")
    {
        return handleAddUserRequest(request.requestData);
    }
    else if (request.requestType == "DELETE_USER")
    {
        return handleDelUserRequest(request.requestData);
    }
    else if (request.requestType == "ADD_MENU")
    {
        return handleAddMenuRequest(request.requestData);
    }
    else if (request.requestType == "DELETE_MENU")
    {
        return handleDelMenuRequest(request.requestData);
    }
    else if (request.requestType == "ROLLOUT_MENU")
    {
        return handleAddDailyMenuItemRequest(request.requestData);
    }
    else if (request.requestType == "GET_DAILY_MENU")
    {
        return handleGetDailyMenuRequest(request.requestData);
    }
    else if (request.requestType == "PLACE_ORDER")
    {
        return handlePlaceOrderRequest(request.requestData);
    }
    else if (request.requestType == "ADD_FEEDBACK")
    {
        return handleAddUserFeedbackRequest(request.requestData);
    }
    else if (request.requestType == "GET_NOTIFICATIONS")
    {
        return handleGetNotifications(request.requestData);
    }
    else if (request.requestType == "MARK_NOTIFICATIONS_VIEWED")
    {
        return handleMarkNotificationsViewed(request.requestData);
    }
    else if (request.requestType == "UPDATE_PROFILE")
    {
        return handleUpdateProfile(request.requestData);
    }
    else if (request.requestType == "VIEW_PROFILE")
    {
        return handleViewProfile(request.requestData);
    }
    else if (request.requestType == "SET_DAILY_MENU_AVAILABILITY_ZERO")
    {
        return handleSetDailyMenuAvailabilityToZeroRequest(request.requestData);
    }

    return "UNKNOWN_REQUEST";
}

std::string RequestHandler::handleLoginRequest(const std::string &data)
{
    std::pair<bool, LoginRequest> loginParam = dataParser->deserializeLoginRequest(data);

    if (loginParam.first)
    {
        return database->authenticateUser(loginParam.second.userId, loginParam.second.password);
    }
    else
    {
        return "INVALID_PARAMETER";
    }
}

std::string RequestHandler::handleGetRecommendedFoodRequest()
{
    std::vector<RecommendedMenuData> recommendedFood = recommendationEngine->getRecommendedFood();

    if (recommendedFood.empty())
    {
        return "STATUS_NO_RECOMMENDATIONS";
    }

    std::pair<bool, std::string> parseData = dataParser->deserializeRecommendedMenuData(recommendedFood);

    if (parseData.first)
    {
        return "STATUS_OK," + parseData.second;
    }
    else
    {
        return "STATUS_ERROR";
    }
}

std::string RequestHandler::handleAddUserRequest(const std::string &data)
{
    std::pair<bool, User> addUserParam = dataParser->deserializeAddUserRequest(data);

    if (addUserParam.first)
    {
        if (database->addUser(addUserParam.second))
        {
            return "STATUS_OK,User added successfully";
        }
        else
        {
            return "STATUS_ERROR,Failed to add user";
        }
    }
    else
    {
        return "STATUS_ERROR,Invalid request format";
    }
}

std::string RequestHandler::handleDelUserRequest(const std::string &data)
{
    std::pair<bool, std::vector<std::string>> parsedData = dataParser->deserializeData(data);

    if (parsedData.first && parsedData.second.size() == 2)
    {
        try
        {
            int currentAdminId = std::stoi(parsedData.second[0]);
            int userIdToDelete = std::stoi(parsedData.second[1]);

            if (currentAdminId == userIdToDelete)
            {
                return "STATUS_ERROR,You cannot delete your own account";
            }

            if (database->deleteUser(userIdToDelete))
            {
                return "STATUS_OK,User deleted successfully";
            }
            else
            {
                return "STATUS_ERROR,Failed to delete user";
            }
        }
        catch (const std::invalid_argument &e)
        {
            return "STATUS_ERROR,Invalid user ID format";
        }
        catch (const std::out_of_range &e)
        {
            return "STATUS_ERROR,User ID out of range";
        }
    }
    else
    {
        return "STATUS_ERROR,Invalid request format";
    }
}

std::string RequestHandler::handleAddMenuRequest(const std::string &data)
{
    std::pair<bool, std::vector<std::string>> addMenuParam = dataParser->deserializeData(data);
    MenuAttributes menuData;

    menuData.menuName = addMenuParam.second.at(0);
    menuData.price = std::stod(addMenuParam.second.at(1));
    menuData.dietType = addMenuParam.second.at(2);
    menuData.spiceLevel = addMenuParam.second.at(3);
    menuData.cuisineType = addMenuParam.second.at(4);
    menuData.sweetType = addMenuParam.second.at(5);

    if (addMenuParam.first)
    {
        if (database->addMenu(menuData))
        {
            return "STATUS_OK,Menu added successfully";
        }
        else
        {
            return "STATUS_ERROR,Failed to add menu";
        }
    }
    else
    {
        return "STATUS_ERROR,Invalid request format";
    }
}

std::string RequestHandler::handleDelMenuRequest(const std::string &data)
{
    int menuId = std::stoi(data);
    if (database->deleteMenu(menuId))
    {
        return "STATUS_OK,Menu deleted successfully";
    }

    return "STATUS_ERROR,Failed to delete menu";
};

std::string RequestHandler::handleAddDailyMenuItemRequest(const std::string &data)
{
    std::pair<bool, std::vector<std::string>> addDailyMenuItemParam = dataParser->deserializeData(data);
    DailyMenuEntry dailyMenuEntry;

    dailyMenuEntry.menuId = std::stoi(addDailyMenuItemParam.second.at(0));
    dailyMenuEntry.availability = std::stoi(addDailyMenuItemParam.second.at(1));
    dailyMenuEntry.mealCategory = addDailyMenuItemParam.second.at(2);

    if (addDailyMenuItemParam.first)
    {
        if (database->insertDailyMenuEntries({dailyMenuEntry}))
        {
            handleNotification("Menu is rolled out");
            return "STATUS_OK,Daily menu item added successfully";
        }
        else
        {
            return "STATUS_ERROR,Failed to add daily menu item";
        }
    }
    else
    {
        return "STATUS_ERROR,Invalid request format";
    }
}

std::string RequestHandler::handleGetDailyMenuRequest(const std::string &data)
{
    int userId = std::stoi(data);
    std::vector<DailyMenuAttributes> items = recommendationEngine->getRecommendedFoodForUser(userId);

    if (items.empty())
    {
        return "STATUS_OK,";
    }

    return "STATUS_OK," + dataParser->serializeData(items);
}

std::string RequestHandler::handlePlaceOrderRequest(const std::string &data)
{
    std::pair<bool, std::vector<std::string>> placeOrderParam = dataParser->deserializeData(data);
    UserOrderEntry userOrderEntry;

    userOrderEntry.userId = std::stoi(placeOrderParam.second.at(0));
    userOrderEntry.dailyMenuId = std::stoi(placeOrderParam.second.at(1));

    if (placeOrderParam.first)
    {
        if (database->insertUserOrderEntries({userOrderEntry}))
        {
            return "STATUS_OK,User order placed successfully";
        }
        else
        {
            return "STATUS_ERROR,Failed to place user order";
        }
    }
    else
    {
        return "STATUS_ERROR,Invalid request format";
    }
}

std::string RequestHandler::handleAddUserFeedbackRequest(const std::string &data)
{
    std::pair<bool, Feedback> addFeedbackParam = dataParser->deserializeUserFeedbackRequest(data);

    if (addFeedbackParam.first)
    {
        if (database->insertUserFeedback(addFeedbackParam.second))
        {
            return "STATUS_OK,User feedback added successfully";
        }
        else
        {
            return "STATUS_ERROR,Failed to add user feedback";
        }
    }
    else
    {
        return "STATUS_ERROR,Invalid request format";
    }
}

std::string RequestHandler::handleNotification(const std::string &data)
{
    if (database->addNotification(data))
    {
        return "STATUS_OK,Notification sent successfully";
    }
}

std::string RequestHandler::handleGetNotifications(const std::string &data)
{
    int userId = std::stoi(data);
    std::vector<Notification> notifications = database->getNonViewedNotificationsForUser(userId);

    if (notifications.empty())
    {
        return "STATUS_OK,NO_NOTIFICATIONS";
    }

    return "STATUS_OK," + dataParser->serializeData(notifications);
}

std::string RequestHandler::handleMarkNotificationsViewed(const std::string &data)
{
    auto [userId, notificationIds] = dataParser->deserializeMarkNotificationsViewedRequest(data);

    if (database->markNotificationsAsViewed(userId, notificationIds))
    {
        return "STATUS_OK,Notifications marked as viewed";
    }
    else
    {
        return "STATUS_ERROR,Failed to mark notifications as viewed";
    }
}

std::string RequestHandler::handleUpdateProfile(const std::string &data)
{
    std::pair<bool, std::vector<std::string>> updateProfileParam = dataParser->deserializeData(data);

    if (updateProfileParam.first)
    {
        UserProfile profile;
        profile.userId = std::stoi(updateProfileParam.second[0]);
        profile.preferenceType = updateProfileParam.second[1];
        profile.spiceLevel = updateProfileParam.second[2];
        profile.cuisinePreference = updateProfileParam.second[3];
        profile.sweetTooth = updateProfileParam.second[4];

        if (database->updateUserProfile(profile))
        {
            return "STATUS_OK,Profile updated successfully";
        }
        else
        {
            return "STATUS_ERROR,Failed to update profile";
        }
    }
    else
    {
        return "STATUS_ERROR,Invalid request format";
    }
}

std::string RequestHandler::handleViewProfile(const std::string &data)
{
    int userId = std::stoi(data);

    UserProfile userProfile = database->getUserProfile(userId);

    std::string response = "STATUS_OK," +
                           std::to_string(userProfile.userId) + "," +
                           userProfile.preferenceType + "," +
                           userProfile.spiceLevel + "," +
                           userProfile.cuisinePreference + "," +
                           userProfile.sweetTooth;

    return response;
}

std::string RequestHandler::handleSetDailyMenuAvailabilityToZeroRequest(const std::string &data)
{
    std::pair<bool, std::vector<std::string>> setAvailabilityParam = dataParser->deserializeData(data);

    if (setAvailabilityParam.first && setAvailabilityParam.second.size() == 1)
    {
        int dailyMenuId = std::stoi(setAvailabilityParam.second.at(0));

        if (database->setDailyMenuAvailabilityToZero(dailyMenuId))
        {
            return "STATUS_OK,Daily menu availability set to zero";
        }
        else
        {
            return "STATUS_ERROR,Failed to set daily menu availability to zero";
        }
    }
    else
    {
        return "STATUS_ERROR,Invalid request format";
    }
}