#include "EmployeeService.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

EmployeeService::EmployeeService(int id, ServerConnection &serverConnection)
    : id(id), serverConnection(serverConnection)
{
    dataParser = std::make_shared<DataParser>();
    userInputHandler = std::make_shared<UserInputHandler>();
}

std::vector<DailyMenuEntry> EmployeeService::fetchDailyMenu()
{
    std::vector<DailyMenuEntry> dailyMenu;
    std::string request = std::to_string((int)RequestType::GET_DAILY_MENU) + "," + std::to_string(id);
    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
    }
    else
    {
        std::string response = serverConnection.readResponse();
        auto [status, dailyMenu] = dataParser->deserializeToDailyMenuEntries(response);

        if (status != "STATUS_OK")
        {
            std::cerr << "Failed to fetch daily menu: " << status << std::endl;
        }
    }

    return dailyMenu;
}

bool EmployeeService::isMenuItemAvailable(const std::vector<DailyMenuEntry> &dailyMenu, int dailyMenuId)
{
    auto it = std::find_if(dailyMenu.begin(), dailyMenu.end(), [dailyMenuId](const DailyMenuEntry &entry)
                           { return entry.dailyMenuId == dailyMenuId; });

    if (it != dailyMenu.end())
    {
        if (it->availability > 0)
        {
            return true;
        }
        else
        {
            std::cout << "The selected food item is not available.\n";
        }
    }
    else
    {
        std::cout << "Invalid menu ID. Please enter a valid ID from the menu.\n";
    }

    return false;
}

void EmployeeService::placeOrder(int dailyMenuId)
{
    std::string request = std::to_string((int)RequestType::PLACE_ORDER) + "," + std::to_string(id) + "," + std::to_string(dailyMenuId);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();
    std::cout << response << std::endl;
}

int EmployeeService::promptForDailyMenuId(const std::vector<DailyMenuEntry> &dailyMenu)
{
    int dailyMenuId;
    while (true)
    {
        dailyMenuId = userInputHandler->getIntInput("Enter the ID of the menu item to give feedback for: ");
        auto it = std::find_if(dailyMenu.begin(), dailyMenu.end(), [dailyMenuId](const DailyMenuEntry &entry)
                               { return entry.dailyMenuId == dailyMenuId; });

        if (it != dailyMenu.end())
        {
            break;
        }
        else
        {
            std::cout << "Invalid ID. Please enter a valid ID from the menu.\n";
        }
    }
    return dailyMenuId;
}

void EmployeeService::submitFeedback(int dailyMenuId, float rating, const std::string &comment)
{
    std::string request = std::to_string((int)RequestType::ADD_FEEDBACK) + "," + std::to_string(id) + "," + std::to_string(dailyMenuId) + "," + std::to_string(rating) + "," + comment;

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send feedback to the server." << std::endl;
        return;
    }

    std::string response = serverConnection.readResponse();
    std::cout << response << std::endl;
}

std::vector<Notification> EmployeeService::checkNotifications()
{
    std::vector<Notification> notifications{};
    std::string request = std::to_string((int)RequestType::GET_NOTIFICATIONS) + "," + std::to_string(id);

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
            std::ostringstream oss;
            for (const auto &notification : notifications)
            {
                oss << notification.notificationId << ",";
            }
            std::string markViewedRequest = std::to_string((int)RequestType::MARK_NOTIFICATIONS_VIEWED) + "," + std::to_string(id) + "," + oss.str();
            serverConnection.sendRequest(markViewedRequest);
        }
    }
    else
    {
        std::cout << "Failed to get notifications: " << status << "\n";
    }

    return notifications;
}

void EmployeeService::updateProfile(ProfileOptions profileOptions)
{
    std::string request = std::to_string((int)RequestType::UPDATE_PROFILE) + "," + std::to_string(id) + "," + profileOptions.dietType + "," +
                          profileOptions.spiceLevel + "," + profileOptions.cuisineType + "," + profileOptions.sweetType;

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Send request failed" << std::endl;
    }
    else
    {
        std::string response = serverConnection.readResponse();
        std::cout << response << std::endl;
    }
}

UserProfile EmployeeService::getProfile()
{
    UserProfile userProfile;
    std::string request = std::to_string((int)RequestType::VIEW_PROFILE) + "," + std::to_string(id);

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Send request failed" << std::endl;
    }
    else
    {
        std::string response = serverConnection.readResponse();
        auto [status, userProfile] = dataParser->deserializeUserProfile(response);
    }

    return userProfile;
}

std::vector<FeedbackQuestion> EmployeeService::fetchFeedbackQuestions()
{
    std::vector<FeedbackQuestion> feedbackQuestions;
    std::string request = std::to_string(static_cast<int>(RequestType::FETCH_FEEDBACK_QUESTIONS));

    if (!serverConnection.sendRequest(request))
    {
        std::cerr << "Failed to send request to server." << std::endl;
        return feedbackQuestions;
    }

    std::string response = serverConnection.readResponse();
    std::istringstream responseStream(response);
    std::string status;
    std::getline(responseStream, status, '|');

    if (status != "STATUS_OK")
    {
        std::cerr << "Failed to fetch feedback questions: " << status << std::endl;
        return feedbackQuestions;
    }

    std::string questionId, questionText;

    while (std::getline(responseStream, questionId, '|') &&
           std::getline(responseStream, questionText, '|'))
    {
        FeedbackQuestion question;
        question.id = std::stoi(questionId);
        question.text = questionText;
        feedbackQuestions.push_back(question);
    }

    return feedbackQuestions;
}

void EmployeeService::giveSuggestionForFood(const std::vector<FeedbackAnswer> &feedbackAnswers)
{
    std::string request = std::to_string(static_cast<int>(RequestType::ADD_SUGGESTION_FOR_FOOD));
    std::string data;

    for (const auto &feedbackAnswer : feedbackAnswers)
    {
        data += std::to_string(feedbackAnswer.questionId) + "," +
                std::to_string(feedbackAnswer.foodId) + "," +
                std::to_string(feedbackAnswer.employeeId) + "," +
                feedbackAnswer.answerText + "|";
    }

    if (!serverConnection.sendRequest(request + "," + data))
    {
        std::cerr << "Failed to send request to server." << std::endl;
    }

    std::string response = serverConnection.readResponse();
    std::istringstream responseStream(response);
    std::string status;
    std::getline(responseStream, status, ',');

    if (status != "STATUS_OK")
    {
        std::cout << "\nFailed to add the suggestions\n";
    }
}
