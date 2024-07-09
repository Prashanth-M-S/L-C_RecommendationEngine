#include "employee.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

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
        std::cout << "2. Order Food\n";
        std::cout << "3. Give Feedback\n";
        std::cout << "4. Check Notifications\n";
        std::cout << "5. Update Profile\n";
        std::cout << "6. View Profile\n";
        std::cout << "7. Logout\n";
        std::cout << "-----------------------------\n";

        choice = userInputHandler->getIntInput("Enter your choice: ");

        switch (choice)
        {
        case 1:
            viewMenu();
            break;
        case 2:
            placeOrder();
            break;
        case 3:
            giveFeedback();
            break;
        case 4:
            checkNotifications();
            break;
        case 5:
            updateProfile();
            break;
        case 6:
            viewProfile();
            break;
        case 7:
            std::cout << "Logging out..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);
}

std::vector<DailyMenuEntry> Employee::viewMenu()
{
    if (!serverConnection.connectToServer())
    {
        std::cout << "Failed to connect to server." << std::endl;
        return {};
    }

    std::string request = "GET_DAILY_MENU," + std::to_string(id);
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

void Employee::placeOrder()
{
    auto dailyMenu = viewMenu();
    if (dailyMenu.empty()) return;

    int dailyMenuId;
    while (true)
    {
        dailyMenuId = userInputHandler->getIntInput("Enter ID to order: ");
        auto it = std::find_if(dailyMenu.begin(), dailyMenu.end(), [dailyMenuId](const DailyMenuEntry &entry) {
            return entry.dailyMenuId == dailyMenuId;
        });

        if (it != dailyMenu.end())
        {
            break;
        }
        else
        {
            std::cout << "Invalid menu ID. Please enter a valid ID from the menu.\n";
        }
    }

    if (!serverConnection.connectToServer())
    {
        std::cout << "Failed to connect to server." << std::endl;
        return;
    }

    std::string request = "PLACE_ORDER," + std::to_string(id) + "," + std::to_string(dailyMenuId);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();

    std::cout << response << std::endl;
}


void Employee::giveFeedback()
{
    auto dailyMenu = viewMenu();
    if (dailyMenu.empty()) return;

    int dailyMenuId;
    while (true)
    {
        dailyMenuId = userInputHandler->getIntInput("Enter the ID of the menu item to give feedback for: ");
        auto it = std::find_if(dailyMenu.begin(), dailyMenu.end(), [dailyMenuId](const DailyMenuEntry &entry) {
            return entry.dailyMenuId == dailyMenuId;
        });

        if (it != dailyMenu.end())
        {
            break;
        }
        else
        {
            std::cout << "Invalid menu ID. Please enter a valid ID from the menu.\n";
        }
    }

    float rating = userInputHandler->getFoodRatingInput("Enter your rating (0 to 5): ");
    std::string comment = userInputHandler->getStringInput("Enter your comment: ");

    if (!serverConnection.connectToServer())
    {
        std::cout << "Failed to connect to server." << std::endl;
        return;
    }

    std::string request = "ADD_FEEDBACK," + std::to_string(dailyMenuId) + "," + std::to_string(id) + "," + std::to_string(rating) + "," + comment;

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();

    std::cout << response << std::endl;
}

void Employee::checkNotifications()
{
    if (!serverConnection.connectToServer())
    {
        std::cout << "Failed to connect to server." << std::endl;
        return;
    }

    std::string request = "GET_NOTIFICATIONS," + std::to_string(id);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();
    auto [status, notifications] = dataParser->deserializeNotifications(response);

    if (status == "STATUS_OK")
    {
        if (notifications.empty())
        {
            std::cout << "No new notifications.\n";
        }
        else
        {
            printNotifications(notifications);

            std::ostringstream oss;
            for (const auto &notification : notifications)
            {
                oss << notification.notificationId << ",";
            }
            std::string markViewedRequest = "MARK_NOTIFICATIONS_VIEWED," + std::to_string(id) + "," + oss.str();

            if (!serverConnection.sendRequest(markViewedRequest))
            {
                std::cerr << "Failed to send request to mark notifications as viewed." << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Failed to get notifications: " << status << "\n";
    }
}

void Employee::updateProfile()
{
    std::vector<std::string> dietOptions = {"Vegetarian", "Non Vegetarian", "Eggetarian"};
    std::vector<std::string> spiceOptions = {"High", "Medium", "Low"};
    std::vector<std::string> cuisineOptions = {"North Indian", "South Indian", "Other"};
    std::vector<std::string> sweetOptions = {"Yes", "No"};

    int dietChoice = userInputHandler->getChoiceInput("Select diet type:", dietOptions);
    int spiceChoice = userInputHandler->getChoiceInput("Select spice level:", spiceOptions);
    int cuisineChoice = userInputHandler->getChoiceInput("Select cuisine type:", cuisineOptions);
    int sweetChoice = userInputHandler->getChoiceInput("Do you like sweet:", sweetOptions);

    std::string dietType = dietOptions[dietChoice - 1];
    std::string spiceLevel = spiceOptions[spiceChoice - 1];
    std::string cuisineType = cuisineOptions[cuisineChoice - 1];
    std::string sweetType = sweetOptions[sweetChoice - 1];

    std::string request = "UPDATE_PROFILE," + std::to_string(id) + "," + dietType + "," + spiceLevel + "," + cuisineType + "," + sweetType;

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

void Employee::viewProfile()
{
    std::string request = "VIEW_PROFILE," + std::to_string(id);

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

    auto [status, userProfile] = dataParser->deserializeUserProfile(response);

    if (status == "STATUS_OK")
    {
        printProfile(userProfile);
    }
    else
    {
        std::cerr << "Failed to retrieve profile information." << std::endl;
    }
}

void Employee::printDailyMenu(const std::vector<DailyMenuEntry> &dailyMenu)
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

void Employee::printProfile(const UserProfile &userProfile)
{
    std::cout << "------------------- Profile Information -------------------\n";
    std::cout << "-----------------------------------------------------------\n";
    std::cout << "| UserID          | " << std::setw(20) << userProfile.userId << " |\n";
    std::cout << "| Preference Type | " << std::setw(20) << userProfile.preferenceType << " |\n";
    std::cout << "| Spice Level     | " << std::setw(20) << userProfile.spiceLevel << " |\n";
    std::cout << "| Cuisine Type    | " << std::setw(20) << userProfile.cuisinePreference << " |\n";
    std::cout << "| Likes Sweet     | " << std::setw(20) << userProfile.sweetTooth << " |\n";
    std::cout << "-----------------------------------------------------------\n";
}

void Employee::printNotifications(const std::vector<Notification> &notifications)
{
    std::cout << "------- Notifications -------\n";
    for (const auto &notification : notifications)
    {
        std::cout << notification.message << "\n";
    }
    std::cout << "------------------------------\n";
}