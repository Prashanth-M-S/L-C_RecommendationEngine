#ifndef USER_INPUT_HANDLER_H
#define USER_INPUT_HANDLER_H

#include <iostream>
#include <vector>
#include <limits>

class UserInputHandler
{
public:
    int getIntInput(const std::string &prompt);
    std::string getStringInput(const std::string &prompt);
    float getFoodRatingInput(const std::string &prompt);
    int getChoiceInput(const std::string &prompt, const std::vector<std::string> &choices);
};

#endif
