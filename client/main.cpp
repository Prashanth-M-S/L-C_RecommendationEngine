#include <iostream>

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
    std::cout << "2. User Login\n";
    std::cout << "3. Chef Login\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter your choice: ";
}

int main()
{
    printMainMenu();
    return 0;
}
