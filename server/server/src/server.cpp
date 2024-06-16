#include "../inc/server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

Server::Server(int port, std::shared_ptr<IRecommendationEngine> rEngine) : port(port), recommendationEngine(rEngine)
{
    setupSocket();
}

Server::~Server()
{
    close(server_fd);
}

void Server::setupSocket()
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

void Server::bindSocket()
{
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

void Server::startListening()
{
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << port << "..." << std::endl;
}

void Server::start()
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

        if (fork() == 0)
        {
            close(server_fd);
            handleClient(new_socket);
            exit(0);
        }
        else
        {
            close(new_socket);
        }
    }
}

void Server::handleClient(int client_socket)
{
    processClient(client_socket);
    close(client_socket);
}

void Server::processClient(int client_socket)
{
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);

    std::string request(buffer);
    if (request == "GET /recommended-food")
    {
        std::vector<Menu> recommendedMenu = recommendationEngine->getRecommendedFood();
        // std::string jsonResponse = recommendationEngine->g
        // send(client_socket, jsonResponse.c_str(), jsonResponse.size(), 0);
    }
    else
    {
        const char *message = "Unknown request\n";
        send(client_socket, message, strlen(message), 0);
    }
}