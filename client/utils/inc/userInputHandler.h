#ifndef USER_INPUT_HANDLER_H
#define USER_INPUT_HANDLER_H

#include <iostream>
#include <limits>

class UserInputHandler
{
public:
    int getIntInput(const std::string &prompt);
    std::string getStringInput(const std::string &prompt);
};

#endif
