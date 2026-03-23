#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
//nlohmann-json3-dev
#include <nlohmann/json.hpp>

#include "../../utils/Logger.h"

class Server
{
public:
    Server(short port, Logger& logger);
    ~Server();
    void main_cycle();

private:
    Logger& logger_;
    bool cycle_flag_ = false;
    const short port_;
    int socket_ = -1;
    std::string buffer_;

    void set_up_server();
    void handle_request(int client_socket);
    void handle_response(int client_socket, const std::string& message);
    void send_response(int client_socket, const void* data, size_t length);
};