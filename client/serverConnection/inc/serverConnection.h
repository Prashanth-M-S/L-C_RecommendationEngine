#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include <string>

class ServerConnection
{
public:
    ServerConnection(const std::string &serverIp, int serverPort);
    ~ServerConnection();

    bool connectToServer();
    int getSocket() const;
    bool sendRequest(const std::string &request);
    std::string readResponse();

private:
    std::string serverIp;
    int serverPort;
    int sock;
};

#endif
