#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "RequestType.h"

class Client {
public:
    Client(const std::string& server_ip, int port);
    ~Client();
    void connectToServer();
    void sendRequest(RequestType requestType);
    std::string receiveResponse();

private:
    std::string server_ip;
    int port;
    int sock;
    struct sockaddr_in server_address;

    std::string requestTypeToString(RequestType requestType);
};

#endif // CLIENT_H
