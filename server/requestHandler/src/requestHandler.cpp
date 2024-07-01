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
    MenuData menuData;

    menuData.menuName = addMenuParam.second.at(0);
    menuData.price = std::stod(addMenuParam.second.at(1));


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

    dailyMenuEntry.menuId  = std::stoi(addDailyMenuItemParam.second.at(0));
    dailyMenuEntry.availability = std::stoi(addDailyMenuItemParam.second.at(1));
    dailyMenuEntry.mealCategory = addDailyMenuItemParam.second.at(2);

    if (addDailyMenuItemParam.first)
    {
        if (database->insertDailyMenuEntries({dailyMenuEntry}))
        {
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
    std::vector<GetDailyMenu> items = database->getDailyMenu();

    if (items.empty())
    {
        return "STATUS_ERROR,NO_DAILY_MENU_ITEMS";
    }

    return "STATUS_OK," + dataParser->serializeData(items);
}