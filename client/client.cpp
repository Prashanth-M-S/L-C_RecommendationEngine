#include "../client/client.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

Client::Client(const std::string& server_ip, int port) 
    : server_ip(server_ip), port(port), sock(0) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Client::~Client() {
    close(sock);
}

void Client::connectToServer() {
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Client::sendRequest(RequestType requestType) {
    std::string request = requestTypeToString(requestType);
    send(sock, request.c_str(), request.size(), 0);
}

std::string Client::receiveResponse() {
    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    return std::string(buffer, valread);
}

std::string Client::requestTypeToString(RequestType requestType) {
    switch (requestType) {
        case RequestType::GET_RECOMMENDED_FOOD:
            return "GET /recommended-food";
        // Add other request types as needed
        default:
            return "UNKNOWN_REQUEST";
    }
}
