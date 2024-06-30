#include "chef.h"
#include <iomanip>

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
        std::cout << "---------Main Menu---------\n";
        std::cout << "\n1. Fetch Recommended Food\n";
        std::cout << "2. Rollout Menu\n";
        std::cout << "3. View Current Menu\n";
        std::cout << "4. Logout\n\n";
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
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
}

void Chef::fetchRecommendedFood()
{
    if (!serverConnection.connectToServer())
    {
        std::cout << "Failed to connect to server." << std::endl;
        return;
    }

    std::string request = "GET_RECOMMENDED_FOOD";
    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();
    auto [status, recommendedFood] = dataParser->parseRecommendedFood(response);

    if (status == "STATUS_OK")
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
    else
    {
        std::cout << "Failed to get the food items: " << status << "\n";
    }
}

void Chef::rolloutMenu()
{
    fetchRecommendedFood();

    int menuId = userInputHandler->getIntInput("Enter menu ID to roll out: ");
    int available = 1;
    std::string category = userInputHandler->getStringInput("Enter category: ");

    std::string request = "ROLLOUT_MENU," + std::to_string(menuId) + "," + std::to_string(available) + "," + category;

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();

    std::cout << response << std::endl;
}

void Chef::viewMenu()
{
    std::cout << "Viewing current menu...\n";
}
