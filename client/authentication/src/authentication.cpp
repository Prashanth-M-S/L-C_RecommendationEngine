#include "authentication.h"
#include "dataParser.h"
#include <iostream>

Authentication::Authentication(ServerConnection &serverConn) : serverConn(serverConn) {}

std::string Authentication::authenticateUser(int userId, const std::string &password)
{
    std::string role;
    std::string loginRequest = "LOGIN," + std::to_string(userId) + "," + password;

    if (!serverConn.connectToServer())
    {
        std::cout << "Failed to connect to server." << std::endl;
        role = "";
    }

    if (!serverConn.sendRequest(loginRequest))
    {
        std::cout << "Send request failed" << std::endl;
        role = "";
    }

    std::string response = serverConn.readResponse();
    DataParser dataparser;
    std::pair<bool, std::vector<std::string>> data = dataparser.deserializeData(response);
    if (data.second.at(0) == "STATUS_OK") 
    {
        role = data.second.at(1);
    }
    
    return role;
}
