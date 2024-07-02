#include "userInputHandler.h"

int UserInputHandler::getIntInput(const std::string &prompt)
{
    std::string inputStr;
    int input;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin >> std::ws, inputStr);
        try
        {
            input = std::stoi(inputStr);
            break;
        }
        catch (std::invalid_argument &)
        {
            std::cout << "Invalid input. Please enter a valid integer.\n";
        }
        catch (std::out_of_range &)
        {
            std::cout << "Input out of range for integer.\n";
        }
    }
    return input;
}

std::string UserInputHandler::getStringInput(const std::string &prompt)
{
    std::string password;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, password); 
    return password;
}


float UserInputHandler::getFoodRatingInput(const std::string &prompt)
{
    std::string inputStr;
    float rating;
    std::cout << prompt;
    
    while (true)
    {
        std::getline(std::cin >> std::ws, inputStr);
        try
        {
            rating = std::stof(inputStr);
            if (rating < 0.0 || rating > 5.0)
            {
                std::cout << "Invalid rating. Please enter a rating between 0.0 and 5.0.\n";
                continue;
            }
            break;
        }
        catch (std::invalid_argument &)
        {
            std::cout << "Invalid input. Please enter a valid floating-point number.\n";
        }
        catch (std::out_of_range &)
        {
            std::cout << "Input out of range for floating-point number.\n";
        }
    }
    return rating;
}