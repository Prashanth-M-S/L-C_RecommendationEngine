#include "employee.h"
#include <iostream>
#include <iomanip>

Employee::Employee(int id, const std::string &password, ServerConnection &serverConnection)
    : id(id), password(password), role("employee"), serverConnection(serverConnection)
{
    userInputHandler = std::make_shared<UserInputHandler>();
    dataParser = std::make_shared<DataParser>();
}

void Employee::mainMenu()
{
    int choice;
    do
    {
        std::cout << "---------Main Menu---------\n";
        std::cout << "\n1. View Menu\n";

        choice = userInputHandler->getIntInput("Enter your choice: ");

        switch (choice)
        {
        case 1:
            viewMenu();
            break;
        case 2:
            viewMenu();
            break;
        case 3:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);
}

void Employee::viewMenu()
{
    if (!serverConnection.connectToServer())
    {
        std::cout << "Failed to connect to server." << std::endl;
        return;
    }

    std::string request = "GET_DAILY_MENU";
    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();
    auto [status, dailyMenu] = dataParser->deserializeToDailyMenuEntries(response);

    if (status == "STATUS_OK")
    {
        std::cout << "----- Daily Menu ------\n";
        std::cout << "------------------------------------------------------------------\n";
        std::cout << "| ID   | Name                | Availability | Category   | Price  |\n";
        std::cout << "------------------------------------------------------------------\n";

        for (const auto &menu : dailyMenu)
        {
            std::cout << "| "
                      << std::setw(4) << menu.dailyMenuId << " | "
                      << std::setw(18) << std::left << menu.itemName.substr(0, 17) << " | "
                      << std::setw(12) << (menu.availability ? "Available" : "Not Available") << " | "
                      << std::setw(10) << menu.mealCategory << " | "
                      << std::setw(6) << std::fixed << std::setprecision(2) << menu.price << " |\n";
        }

        std::cout << "------------------------------------------------------------------\n";
    }
    else
    {
        std::cout << "Failed to get the daily menu items: " << status << "\n";
    }
}
