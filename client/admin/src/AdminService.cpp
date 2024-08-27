#include "AdminService.h"

AdminService::AdminService(int id, ServerConnection &serverConnection)
    : id(id), serverConnection(serverConnection)
{
    dataParser = std::make_shared<DataParser>();
}

std::string AdminService::addUser(UserData userData)
{
    std::string response;
    std::string request = std::to_string((int)RequestType::ADD_USER) + "," + std::to_string(userData.userId) + "," +
                          userData.name + "," + userData.password + "," + userData.role;

    if (!serverConnection.sendRequest(request))
    {
        response = "Send request failed";
    }
    else
    {
        response = serverConnection.readResponse();
    }

    return response;
}

std::string AdminService::deleteUser(int userIdToDelet)
{
    std::string response;
    std::string request = std::to_string((int)RequestType::DELETE_USER) + "," + std::to_string(id) + "," + std::to_string(userIdToDelet);

    if (!serverConnection.sendRequest(request))
    {
        response = "Send request failed";
    }
    else
    {
        response = serverConnection.readResponse();
    }

    return response;
}

std::string AdminService::addMenu(std::string menuName, float menuPrice, ProfileOptions profileOptions)
{
    std::string response;
    std::string request = std::to_string((int)RequestType::ADD_MENU) + "," + menuName + "," + std::to_string(menuPrice) + "," +
                          profileOptions.dietType + "," + profileOptions.spiceLevel + "," + profileOptions.cuisineType + "," + profileOptions.sweetType;

    if (!serverConnection.sendRequest(request))
    {
        response = "Send request failed";
    }
    else
    {
        response = serverConnection.readResponse();
    }

    return response;
}

std::string AdminService::deleteMenu(int menuId)
{
    std::string response;
    std::string request = std::to_string((int)RequestType::DELETE_MENU) + "," + std::to_string(menuId);

    if (!serverConnection.sendRequest(request))
    {
        response = "Send request failed";
    }
    else
    {
        response = serverConnection.readResponse();
    }

    return response;
}

std::pair<std::string, std::vector<RecommendedMenuData>> AdminService::getRecommendedmenu()
{
    std::vector<RecommendedMenuData> recommendedFood;
    std::string status;
    std::string request = std::to_string((int)RequestType::GET_RECOMMENDED_FOOD);
    if (!serverConnection.sendRequest(request))
    {
        status = "Failed to send request to server";
    }
    else
    {
        std::string response = serverConnection.readResponse();
        auto [status, recommendedFood] = dataParser->parseRecommendedFood(response);
    }

    return {status, recommendedFood};
}

std::pair<std::string, std::vector<DailyMenuEntry>> AdminService::fetchDailyMenu()
{
    std::vector<DailyMenuEntry> dailyMenu;
    std::string status;
    std::string request = std::to_string((int)RequestType::GET_DAILY_MENU) + "," + std::to_string(id);
    if (!serverConnection.sendRequest(request))
    {
        status = "Failed to send request to server";
    }
    else
    {
        std::string response = serverConnection.readResponse();
        auto [status, dailyMenu] = dataParser->deserializeToDailyMenuEntries(response);
    }

    return {status, dailyMenu};
}