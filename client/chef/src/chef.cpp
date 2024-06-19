#include "chef.h"

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
        std::cout << "\n\n1. Fetch Recommended Food\n";
        std::cout << "2. Rollout Menu\n";
        std::cout << "3. View Current Menu\n";
        std::cout << "4. Logout\n\n";
        std::cout << "Enter your choice: ";
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
        std::cout << "Recommended food:\n";
        for (const auto &menu : recommendedFood)
        {
            std::cout << "ID: " << menu.menuId << ", Name: " << menu.menuName << ", Price: " << menu.price << "\n\n";
        }
    }
    else
    {
        std::cout << "Failed to get the food item " << status << "\n";
    }
}

void Chef::rolloutMenu()
{
    std::cout << "Rolling out a menu...\n";
}

void Chef::viewMenu()
{
    std::cout << "Viewing current menu...\n";
}
