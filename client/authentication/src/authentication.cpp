#include "authentication.h"
#include <iostream>

Authentication::Authentication(ServerConnection &serverConn) : serverConn(serverConn) {}

bool Authentication::authenticateUser(int userId, const std::string &password)
{
    std::string loginRequest = "LOGIN," + std::to_string(userId) + "," + password;

    int sock = serverConn.getSocket();
    if (sock == -1)
    {
        std::cout << "Server connection is not established.\n";
        return false;
    }

    if (send(sock, loginRequest.c_str(), loginRequest.size(), 0) < 0)
    {
        perror("Send failed");
        return false;
    }

    char buffer[256];
    if (recv(sock, buffer, sizeof(buffer), 0) < 0)
    {
        perror("Receive failed");
        return false;
    }

    buffer[255] = '\0';

    return std::string(buffer).find("status ok") != std::string::npos;
}
