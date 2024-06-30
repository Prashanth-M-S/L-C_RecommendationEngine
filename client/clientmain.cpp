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

#define SERVER_PORT 1234
#define SERVER_IP "127.0.0.1"

void printMainMenu()
{
    std::cout << "\033[1;32m" << R"(
   __________________________________________________
  |                                                  |
  |                  Cafeteria                       |
  |__________________________________________________|
)" << "\033[0m"
              << std::endl;
    std::cout << "Main Menu:\n";
    std::cout << "1. Admin Login\n";
    std::cout << "2. Chef Login\n";
    std::cout << "3. Employee Login\n";
    std::cout << "4. Exit\n";
}

int main()
{
    ServerConnection serverConnection(SERVER_IP, SERVER_PORT);

    while (true)
    {
        if (!serverConnection.connectToServer())
        {
            std::cout << "Failed to connect to server. trying to reconnect......" << std::endl;
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
        std::string role = auth.authenticateUser(id, password);

        switch (choice)
        {
        case 1:
        {
            if (role == "ADMIN")
            {
                Admin admin(id, password, serverConnection);
                admin.mainMenu();
            }
            else
            {
                std::cout << "Admin athentication failed\n";
            }
            break;
        }
        case 2:
        {
            if (role == "CHEF")
            {
                Chef chef(id, password, serverConnection);
                chef.mainMenu();
            }
            else
            {
                std::cout << "Chef athentication failedn";
            }
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