#include "serverConnection.h"

ServerConnection::ServerConnection(const std::string &serverIp, int serverPort)
    : serverIp(serverIp), serverPort(serverPort), sock(-1) {}

ServerConnection::~ServerConnection()
{
    if (sock != -1)
    {
        close(sock);
    }
}

bool ServerConnection::connectToServer()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket creation error");
        return false;
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, '0', sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIp.c_str(), &serverAddress.sin_addr) <= 0)
    {
        perror("Invalid address or Address not supported");
        return false;
    }

    if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Connection failed");
        return false;
    }

    return true;
}

int ServerConnection::getSocket() const
{
    return sock;
}

bool ServerConnection::sendRequest(const std::string &request)
{
    if (send(sock, request.c_str(), request.size(), 0) < 0)
    {
        perror("Send request failed");
        return false;
    }
    return true;
}

std::string ServerConnection::readResponse()
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytesReceived = read(sock, buffer, sizeof(buffer) - 1);
    if (bytesReceived < 0)
    {
        perror("Failed to fetch data from the server");
        return "";
    }

    buffer[bytesReceived] = '\0';

    return std::string(buffer);
}
