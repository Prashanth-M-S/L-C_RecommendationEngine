#include "admin.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

Admin::Admin(int id, const std::string &password, ServerConnection &serverConnection)
    : id(id), password(password), role("admin"), serverConnection(serverConnection)
{
    userInputHandler = std::make_shared<UserInputHandler>();
    dataParser = std::make_shared<DataParser>();
}

void Admin::mainMenu()
{
    int choice;
    do
    {
        std::cout << "\n-------Menu---------\n";
        std::cout << "\n1. Add User\n";
        std::cout << "2. Delete User\n";
        std::cout << "3. Add Menu Item\n";
        std::cout << "4. Delete Menu Item\n";
        std::cout << "5. Update Menu Item\n";
        std::cout << "6. Logout\n\n";
        choice = userInputHandler->getIntInput("Enter your choice: ");

        switch (choice)
        {
        case 1:
            addUser();
            break;
        case 2:
            deleteUser();
            break;
        case 3:
            addMenu();
            break;
        case 4:
            deleteMenu();
            break;
        case 5:
            updateMenu();
            break;
        case 6:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);
}

void Admin::addUser()
{
    int userId = userInputHandler->getIntInput("Enter user ID: ");
    std::string username = userInputHandler->getStringInput("Enter username: ");
    std::string password = userInputHandler->getStringInput("Enter password: ");
    std::string role = userInputHandler->getStringInput("Select role: ");

    std::string request = "ADD_USER," + std::to_string(userId) + "," + username + "," + password + "," + role;

    if (!serverConnection.connectToServer())
    {
        std::cerr << "Failed to connect to server." << std::endl;
        return;
    }

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Send request failed" << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();
    std::cout << "Server response: " << response << std::endl;
}

void Admin::deleteUser()
{
    int userIdToDelet = userInputHandler->getIntInput("Enter user ID that you want to delete: ");

    if (!serverConnection.connectToServer())
    {
        std::cerr << "Failed to connect to server." << std::endl;
        return;
    }
    std::string request = "DELETE_USER," + std::to_string(id) + "," + std::to_string(userIdToDelet);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Send request failed" << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();

    std::cout << "Server response: " << response << std::endl;
}

void Admin::addMenu()
{
    std::cout << "Adding a menu item...\n";
}

void Admin::deleteMenu()
{
    std::cout << "Deleting a menu item...\n";
}

void Admin::updateMenu()
{
    std::cout << "Updating a menu item...\n";
}
