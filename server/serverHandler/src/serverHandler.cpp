#include "serverHandler.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

serverHandler::serverHandler(int port, std::shared_ptr<IRequestHandler> requestHandler) : port(port), requestHandler(requestHandler)
{
    setupSocket();
}

serverHandler::~serverHandler()
{
    close(server_fd);
}

void serverHandler::setupSocket()
{
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}

void serverHandler::bindSocket()
{
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

void serverHandler::startListening()
{
    if (listen(server_fd, 5) < 0)
    {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "serverHandler is listening on port " << port << "..." << std::endl;
}

void serverHandler::start()
{
    bindSocket();
    startListening();

    int addrlen = sizeof(address);
    while (true)
    {
        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept");
            exit(EXIT_FAILURE);
        }
        std::thread handleClientThread(&serverHandler::handleClient, this, new_socket);
        handleClientThread.detach();
    }
}

void serverHandler::handleClient(int new_socket)
{
    requestHandler->handleRequest(new_socket);
    close(new_socket);
}