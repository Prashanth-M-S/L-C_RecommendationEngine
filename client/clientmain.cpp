#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "serverConnection.h"
#include "userInputHandler.h"
#include "authentication.h"
#include "chef.h"
#include "admin.h"

#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1"

void printMainMenu()
{
    std::cout << "\033[1;32m" << R"(
   __________________________________________________
  |                                                  |
  |                  Cafeteria                       |
  |__________________________________________________|
)" << "\033[0m" << std::endl;
    std::cout << "Main Menu:\n";
    std::cout << "1. Admin Login\n";
    std::cout << "2. Chef Login\n";
    // std::cout << "2. User Login\n";
    std::cout << "4. Exit\n";
}

int main()
{
    ServerConnection serverConnection(SERVER_IP, SERVER_PORT);

    while (true)
    {
        if (!serverConnection.connectToServer())
        {
            std::cout << "Failed to connect to server. trying to connect......" << std::endl;
            sleep(2);
            system("clear");
            continue;
        }
        UserInputHandler userInputHandler;
        Authentication auth(serverConnection);

        printMainMenu();
        int choice = userInputHandler.getIntInput("\nEnter Choice: ");
        if (choice == 4)
        {
            return 0;
        }
        int id = userInputHandler.getIntInput("enter your Id: ");
        std::string password = userInputHandler.getStringInput("enter your password: ");

        if (!auth.authenticateUser(id, password))
        {
            std::cout << "\033[1;31m" << "\nAuthentication failed please check your password or ID\n" << "\033[0m" << "\n";
            continue;
        }
        else
        {
            std::cout << "\033[1;32m" << "\nAuthentication successful\n" << "\033[0m" << "\n";
        }

        switch (choice)
        {
        case 1:
        {
            Admin admin(id, password, serverConnection);
            admin.mainMenu();
            break;
        }
        case 2:
        {
            Chef chef(id, password, serverConnection);
            chef.mainMenu();
            break;
        }
        case 3:
        {
            break;
        }
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}