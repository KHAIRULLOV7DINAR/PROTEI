#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
//nlohmann-json3-dev
#include <nlohmann/json.hpp>
#include <atomic>
#include <mutex>

#include "../../utils/Logger.h"

class Server
{
public:
    Server(short port, Logger& logger);
    ~Server();

    void main_cycle();
    void stop();

private:
    Logger& logger_;
    const short port_;
    int socket_ = -1;
    std::atomic<bool> running_{false};
    std::atomic<int> active_clients_{0};
    static constexpr int MAX_CLIENTS = 4;

    void set_up_server();
    static void handle_client(int client_socket, Logger& logger, int client_id, std::atomic<bool>& running, std::atomic<int>& active_clients);
};