#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <netinet/in.h>
#include <vector>
#include <string>
#include <memory>
#include "utils.h"
#include "IRequestHandler.h"

class serverHandler
{
public:
    serverHandler(int port, std::shared_ptr<IRequestHandler> requestHandler);
    ~serverHandler();
    void start();

private:
    int server_fd;
    int port;
    struct sockaddr_in address;
    std::shared_ptr<IRequestHandler> requestHandler;

    void setupSocket();
    void bindSocket();
    void startListening();
    void handleClient(int new_socket);
};

#endif
