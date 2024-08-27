#include "Employee.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

Employee::Employee(int id, const std::string &password, ServerConnection &serverConnection)
    : id(id), password(password)
{
    userInputHandler = std::make_shared<UserInputHandler>();
    employeeService = std::make_shared<EmployeeService>(id, serverConnection);
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
        std::cout << "7. Give Suggestion for Food\n";
        std::cout << "8. Logout\n";
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
            giveSuggestionForFood();
            break;
        case 8:
            std::cout << "Logging out..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);
}

void Employee::viewMenu()
{
    auto dailyMenu = employeeService->fetchDailyMenu();
    printDailyMenu(dailyMenu);
}

void Employee::printDailyMenu(const std::vector<DailyMenuEntry> &dailyMenu)
{
    if (dailyMenu.empty())
    {
        std::cout << "\nNo daily menu items available.\n\n";
        return;
    }
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

void Employee::placeOrder()
{
    auto dailyMenu = employeeService->fetchDailyMenu();
    printDailyMenu(dailyMenu);

    if (dailyMenu.empty())
    {
        int dailyMenuId;
        while (true)
        {
            dailyMenuId = userInputHandler->getIntInput("Enter ID to order: ");
            if (employeeService->isMenuItemAvailable(dailyMenu, dailyMenuId))
            {
                break;
            }
        }

        employeeService->placeOrder(dailyMenuId);
    }
}

void Employee::giveFeedback()
{
    auto dailyMenu = employeeService->fetchDailyMenu();
    if (dailyMenu.empty())
        return;

    int dailyMenuId = employeeService->promptForDailyMenuId(dailyMenu);
    float rating = userInputHandler->getFoodRatingInput("Enter your rating (0 to 5): ");
    std::string comment = userInputHandler->getStringInput("Enter your comment: ");

    employeeService->submitFeedback(dailyMenuId, rating, comment);
}

void Employee::printDailyMenu(const std::vector<DailyMenuEntry> &dailyMenu)
{
    if (dailyMenu.empty())
    {
        std::cout << "\nNo daily menu items available.\n\n";
        return;
    }
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

void Employee::checkNotifications()
{
    std::vector<Notification> &notifications = employeeService->checkNotifications();
    printNotifications(notifications);
}

void Employee::printNotifications(const std::vector<Notification> &notifications)
{
    if (notifications.empty())
    {
        std::cout << "------- Notifications -------\n";
        for (const auto &notification : notifications)
        {
            std::cout << notification.message << "\n";
        }
        std::cout << "------------------------------\n";
    }
}

void Employee::updateProfile()
{
    int dietChoice = userInputHandler->getChoiceInput("Select diet type:", dietOptions);
    int spiceChoice = userInputHandler->getChoiceInput("Select spice level:", spiceOptions);
    int cuisineChoice = userInputHandler->getChoiceInput("Select cuisine type:", cuisineOptions);
    int sweetChoice = userInputHandler->getChoiceInput("Do you like sweet:", sweetOptions);

    ProfileOptions profileOptions;
    profileOptions.dietType = dietOptions[dietChoice - 1];
    profileOptions.spiceLevel = spiceOptions[spiceChoice - 1];
    profileOptions.cuisineType = cuisineOptions[cuisineChoice - 1];
    profileOptions.sweetType = sweetOptions[sweetChoice - 1];

    std::string request = std::to_string((int)RequestType::UPDATE_PROFILE) + "," + std::to_string(id) + "," + dietType + "," + spiceLevel + "," + cuisineType + "," + sweetType;

    employeeService->updateProfile(profileOptions);
}

void Employee::viewProfile()
{
    UserProfile userProfile = employeeService->getProfile();
    printProfile(userProfile);
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

void Employee::giveSuggestionForFood()
{
    auto dailyMenu = employeeService->fetchDailyMenu();
    if (!dailyMenu.empty())
    {
        std::vector<FeedbackQuestion> feedbackQuestions = employeeService->fetchFeedbackQuestions();
        std::vector<FeedbackAnswer> feedbackAnswers;

        if (feedbackQuestions.empty())
        {
            std::cerr << "No feedback questions available." << std::endl;
        }
        else
        {
            int foodId = employeeService->promptForDailyMenuId(dailyMenu);
            std::cout << "\nPlease answers the below question :\n\n";
            for (auto &feedbackQuestion : feedbackQuestions)
            {
                std::string answer = userInputHandler->getStringInput(feedbackQuestion.text + "\n ->  ");
                FeedbackAnswer feedbackAnswer;
                feedbackAnswer.questionId = feedbackQuestion.id;
                feedbackAnswer.foodId = foodId;
                feedbackAnswer.employeeId = this->id;
                feedbackAnswer.answerText = answer;
                feedbackAnswers.push_back(feedbackAnswer);
            }

            employeeService->giveSuggestionForFood(feedbackAnswers);
        }
    }
}
