#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <vector>
#include <string>
#include <memory>
#include "menuUtils.h"
#include "IRecommendationEngine.h"

class Server
{
public:
    Server(int port, std::shared_ptr<IRecommendationEngine> rEngine);
    ~Server();
    void start();

private:
    int server_fd;
    int port;
    struct sockaddr_in address;
    std::shared_ptr<IRecommendationEngine> recommendationEngine;

    void setupSocket();
    void bindSocket();
    void startListening();
    void handleClient(int client_socket);
    void processClient(int client_socket);
};

#endif
