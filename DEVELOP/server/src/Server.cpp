#include <thread>
#include <chrono>
#include <cstring>
#include <string>

#include "../include/Server.h"


Server::Server(short port, Logger& logger) : logger_(logger), port_(port)
{
    set_up_server();
    logger_.simple_console_log("Server was created!");
    logger_.info_file_log("Server was created!");
}

Server::~Server()
{
    stop();
    if (socket_ != -1)
    {
        close(socket_);
    }
}

void Server::stop()
{
    running_ = false;  
}

void Server::set_up_server()
{
    socket_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == -1)
    {
        throw std::runtime_error("Creating server socket failed!");
    }

    int opt = 1;
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in sock_addr{};
    sock_addr.sin_family = PF_INET;
    sock_addr.sin_port = htons(port_);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1)
    {
        close(socket_);
        socket_ = -1;
        throw std::runtime_error("Binding socket failed!");
    }

    if (listen(socket_, 10) == -1)
    {
        close(socket_);
        socket_ = -1;
        throw std::runtime_error("Listening socket failed!");
    }

    running_ = true;
}

void Server::main_cycle()
{
    int next_client_id = 0;

    while (running_)
    {
        while (running_ && active_clients_ >= MAX_CLIENTS)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (!running_)
        {
            break;
        }

        int client_socket = accept(socket_, nullptr, nullptr);
        if (client_socket < 0)
        {
            if (running_)
                logger_.simple_console_log("Accepting failed!");
            continue;
        }

        active_clients_++;
        int client_id = ++next_client_id;

        logger_.simple_console_log("New client connected, ID: " + std::to_string(client_id) + '\n');

        try
        {
            std::thread client_thread(handle_client, client_socket, std::ref(logger_), client_id, std::ref(running_), std::ref(active_clients_));
            client_thread.detach();   
        }
        catch (const std::exception& ex)
        {
            active_clients_--;
            close(client_socket);
            logger_.simple_console_log("Failed to create thread: " + std::string(ex.what()));
        }
    }
}

void Server::handle_client(int client_socket, Logger& logger, int client_id, std::atomic<bool>& running, std::atomic<int>& active_clients)
{
    std::string buffer;

    constexpr size_t CHUNK_SIZE = 1024;
    char chunk[CHUNK_SIZE];

    try
    {
        while (running)
        {
            size_t pos = buffer.find('\n');
            if (pos != std::string::npos)
            {
                std::string message = buffer.substr(0, pos);
                buffer.erase(0, pos + 1);

                nlohmann::json request = nlohmann::json::parse(message);
                std::string type = request["type"];
                std::vector<double> vec = request["vector"];

                logger.simple_console_log("Received from client " + std::to_string(client_id));
                logger.info_file_log("Client " + std::to_string(client_id) + ": Type " + type + '\n');

                if (vec.size() != 4)
                {
                    throw std::runtime_error("Vector size does not equal 4!");
                }

                nlohmann::json response;
                response["type"] = type;

                // *2
                if (type == "int")
                {
                    std::vector<int> int_vec(vec.begin(), vec.end());
                    for (int& v : int_vec) v *= 2;
                    response["vector"] = int_vec;
                }
                else if (type == "float")
                {
                    std::vector<float> float_vec(vec.begin(), vec.end());
                    for (float& v : float_vec) v *= 2.0f;
                    response["vector"] = float_vec;
                }
                else if (type == "double")
                {
                    std::vector<double> double_vec = vec;
                    for (double& v : double_vec) v *= 2.0;
                    response["vector"] = double_vec;
                }
                else
                {
                    throw std::runtime_error("Unknown type!");
                }

                std::string response_str = response.dump() + "\n";
                const char* data_ptr = response_str.data();
                size_t remaining = response_str.size();

                while (remaining > 0)
                {
                    ssize_t sent = send(client_socket, data_ptr, remaining, 0);
                    if (sent == -1)
                    {
                        throw std::runtime_error("Sending error!");
                    }
                    data_ptr += sent;
                    remaining -= sent;
                }

                logger.info_file_log("Response sent to client " + std::to_string(client_id));
            }
            else
            {
                ssize_t bytes = recv(client_socket, chunk, CHUNK_SIZE, 0);
                if (bytes == 0)
                {
                    logger.simple_console_log("Client " + std::to_string(client_id) + " closed connection!");
                    break;
                }
                if (bytes < 0)
                {
                    throw std::runtime_error("Receiving error!");
                }
                buffer.append(chunk, bytes);
            }
        }
    }
    catch (const std::exception& ex)
    {
        logger.simple_console_log("Error in client " + std::to_string(client_id) + ": " + ex.what() + '\n');
        logger.file_log(ex);
    }

    close(client_socket);
    active_clients--;
    logger.simple_console_log("Client " + std::to_string(client_id) + " disconnected!\n");
}