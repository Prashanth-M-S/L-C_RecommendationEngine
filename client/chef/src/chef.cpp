#include "chef.h"
#include <iomanip>
#include <sstream>
#include <algorithm>

Chef::Chef(int id, const std::string &password, ServerConnection &serverConnection)
    : id(id), password(password), role("chef"), serverConnection(serverConnection)
{
    userInputHandler = std::make_shared<UserInputHandler>();
    dataParser = std::make_shared<DataParser>();
}

void Chef::mainMenu()
{
    int choice;
    do
    {
        std::cout << "\n---------Main Menu---------\n";
        std::cout << "\n1. Fetch Recommended Food\n";
        std::cout << "2. Rollout Menu\n";
        std::cout << "3. View Current Menu\n";
        std::cout << "4. set menu availability to Zero\n";
        std::cout << "5. get menu feedback\n";
        std::cout << "6. delete menu\n";
        std::cout << "7. write the suggestion questions\n";
        std::cout << "8. see Suggestions For Menu\n";
        std::cout << "9. Logout\n\n";
        choice = userInputHandler->getIntInput("Enter your choice: ");

        switch (choice)
        {
        case 1:
            fetchRecommendedFood();
            break;
        case 2:
            rolloutMenu();
            break;
        case 3:
            viewMenu();
            break;
        case 4:
            setMenuAvailabilityToZero();
            break;
        case 5:
            fetchMenuFeedbacks();
            break;
        case 6:
            deleteMenuItem();
            break;
        case 7:
            writeSuggestionQuestion();
            break;
        case 8:
            FoodSuggestionsForMenu();
            break;
        case 9:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);
}

std::vector<RecommendedMenuData> Chef::fetchRecommendedFood()
{
    std::vector<RecommendedMenuData> recommendedMenuData;

    std::string request = std::to_string((int)RequestType::GET_RECOMMENDED_FOOD);
    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return {};
    }

    std::string response = serverConnection.readResponse();
    auto [status, recommendedFood] = dataParser->parseRecommendedFood(response);
    recommendedMenuData = recommendedFood;
    if (status == "STATUS_OK")
    {
        printRecommendedFood(recommendedFood);
    }
    else
    {
        std::cout << "Failed to get the food items: " << status << "\n";
    }

    return recommendedMenuData;
}

void Chef::rolloutMenu()
{
    auto recommendedFood = fetchRecommendedFood();
    if (recommendedFood.empty())
        return;

    int menuId;
    while (true)
    {
        menuId = userInputHandler->getIntInput("Enter menu ID to roll out: ");
        auto it = std::find_if(recommendedFood.begin(), recommendedFood.end(), [menuId](const RecommendedMenuData &menu)
                               { return menu.menuId == menuId; });

        if (it != recommendedFood.end())
        {
            break;
        }
        else
        {
            std::cerr << "Invalid menu ID. Please enter a valid menu ID from the recommended food list." << std::endl;
        }
    }

    int available = 1;
    std::vector<std::string> categoryOptions = {"breakfast", "lunch", "dinner"};
    int selectedCategory = userInputHandler->getChoiceInput("Select diet type:", categoryOptions);
    std::string category = categoryOptions[selectedCategory - 1];

    std::string request = std::to_string((int)RequestType::ROLLOUT_MENU) + "," + std::to_string(menuId) + "," + std::to_string(available) + "," + category;

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();

    std::cout << response << std::endl;
}

std::vector<DailyMenuEntry> Chef::viewMenu()
{
    std::string request = std::to_string((int)RequestType::GET_DAILY_MENU) + "," + std::to_string(id);
    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return {};
    }

    std::string response = serverConnection.readResponse();
    auto [status, dailyMenu] = dataParser->deserializeToDailyMenuEntries(response);

    if (status == "STATUS_OK")
    {
        printDailyMenu(dailyMenu);
        return dailyMenu;
    }
    else
    {
        std::cout << "Failed to get the daily menu items: " << status << "\n";
        return {};
    }
}

void Chef::printRecommendedFood(const std::vector<RecommendedMenuData> &recommendedFood)
{
    std::cout << "-----Recommended food------\n";
    std::cout << "-------------------------------------------------\n";
    std::cout << "| ID   | Name            | Price  | Rating      |\n";
    std::cout << "-------------------------------------------------\n";

    for (const auto &menu : recommendedFood)
    {
        std::cout << "| "
                  << std::setw(4) << menu.menuId << " | "
                  << std::setw(15) << std::left << menu.menuName.substr(0, 14) << " | "
                  << std::setw(6) << std::fixed << std::setprecision(2) << menu.price << " | "
                  << std::setw(10) << std::fixed << std::setprecision(2) << menu.recommendationScore << " |\n";
    }

    std::cout << "-------------------------------------------------\n";
}

void Chef::printDailyMenu(const std::vector<DailyMenuEntry> &dailyMenu)
{
    std::cout << "----- Daily Menu ------\n";
    std::cout << "------------------------------------------------------------------\n";
    std::cout << "| ID   | Name                | Availability | Category   | Price  |\n";
    std::cout << "------------------------------------------------------------------\n";

    for (const auto &menu : dailyMenu)
    {
        std::cout << "| "
                  << std::setw(4) << menu.dailyMenuId << " | "
                  << std::setw(19) << menu.itemName << " | "
                  << std::setw(12) << menu.availability << " | "
                  << std::setw(10) << menu.mealCategory << " | "
                  << std::setw(6) << std::fixed << std::setprecision(2) << menu.price << " |\n";
    }

    std::cout << "------------------------------------------------------------------\n";
}

void Chef::setMenuAvailabilityToZero()
{
    auto dailyMenu = viewMenu();
    if (dailyMenu.empty())
        return;

    int dailyMenuId;
    while (true)
    {
        dailyMenuId = userInputHandler->getIntInput("Enter ID to set availability to zero: ");
        auto it = std::find_if(dailyMenu.begin(), dailyMenu.end(), [dailyMenuId](const DailyMenuEntry &entry)
                               { return entry.dailyMenuId == dailyMenuId; });

        if (it != dailyMenu.end())
        {
            break;
        }
        else
        {
            std::cout << "Invalid menu ID. Please enter a valid ID from the menu.\n";
        }
    }

    std::string request = std::to_string((int)RequestType::SET_DAILY_MENU_AVAILABILITY_ZERO) + "," + std::to_string(dailyMenuId);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();

    std::cout << response << std::endl;
}

void Chef::fetchMenuFeedbacks()
{
    auto recommendedFood = fetchRecommendedFood();
    if (recommendedFood.empty())
        return;

    int menuId;
    while (true)
    {
        menuId = userInputHandler->getIntInput("Enter menu ID to see the feedback: ");
        auto it = std::find_if(recommendedFood.begin(), recommendedFood.end(), [menuId](const RecommendedMenuData &menu)
                               { return menu.menuId == menuId; });

        if (it != recommendedFood.end())
        {
            break;
        }
        else
        {
            std::cerr << "Invalid menu ID. Please enter a valid menu ID from the recommended food list." << std::endl;
        }
    }

    std::string request = std::to_string(static_cast<int>(RequestType::FETCH_FEEDBACK)) + "," + std::to_string(menuId);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();
    std::istringstream responseStream(response);
    std::string status;
    std::getline(responseStream, status, '|');

    if (status != "STATUS_OK")
    {
        std::cerr << "Failed to fetch feedbacks: " << status << std::endl;
        return;
    }

    std::cout << "\n-----------------------------------------" << std::endl;
    std::string date, rating, comment;
    bool hasFeedback = false;
    while (std::getline(responseStream, date, '|') &&
           std::getline(responseStream, rating, '|') &&
           std::getline(responseStream, comment, '|'))
    {
        hasFeedback = true;
        std::cout << "Date: " << date << ", Rating: " << rating
                  << ", Comment: " << comment << std::endl;
    }

    if (!hasFeedback)
    {
        std::cout << "No feedback available for this menu item." << std::endl;
    }
    std::cout << "-----------------------------------------\n\n";
}

void Chef::deleteMenuItem()
{
    auto recommendedFood = fetchRecommendedFood();
    if (recommendedFood.empty())
        return;

    int menuId;
    while (true)
    {
        menuId = userInputHandler->getIntInput("Enter menu ID to see the feedback: ");
        auto it = std::find_if(recommendedFood.begin(), recommendedFood.end(), [menuId](const RecommendedMenuData &menu)
                               { return menu.menuId == menuId; });

        if (it != recommendedFood.end())
        {
            break;
        }
        else
        {
            std::cerr << "Invalid menu ID. Please enter a valid menu ID from the recommended food list." << std::endl;
        }
    }

    std::string request = std::to_string((int)RequestType::DELETE_MENU) + "," + std::to_string(menuId);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Send request failed" << std::endl;
        return;
    }
    std::string response = serverConnection.readResponse();

    std::cout << "server response: " << response << std::endl;
}

void Chef::writeSuggestionQuestion()
{
    std::string question = userInputHandler->getStringInput("Enter the question you would like to ask the customer: ");

    std::string request = std::to_string((int)RequestType::ADD_FEEDBACK_QUESTION) + "," + question;

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Send request failed" << std::endl;
        return;
    }
    std::string response = serverConnection.readResponse();

    std::cout << "server response: " << response << std::endl;
}

void Chef::FoodSuggestionsForMenu()
{    
    auto recommendedFood = fetchRecommendedFood();
    if (recommendedFood.empty())
        return;

    int menuId;
    while (true)
    {
        menuId = userInputHandler->getIntInput("Enter menu ID which you want to see the suggeston: ");
        auto it = std::find_if(recommendedFood.begin(), recommendedFood.end(), [menuId](const RecommendedMenuData &menu)
                               { return menu.menuId == menuId; });

        if (it != recommendedFood.end())
        {
            break;
        }
        else
        {
            std::cerr << "Invalid menu ID. Please enter a valid menu ID from the recommended food list." << std::endl;
        }
    }

    std::string request = std::to_string((int)RequestType::FETCH_SUGGESTIONS_FOR_MENU) + "," + std::to_string(menuId);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();

    std::cout << std::endl << response << std::endl;
}
