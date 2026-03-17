#include <nlohmann/json.hpp>

#include "../include/Server.h"


Server::Server(short port) : port_(port)
{
    set_up_server();
}

Server::~Server()
{
    if(socket_ != -1)
    {
        close(socket_);
    }
}

void Server::main_cycle()
{
    while(cycle_flag_)
    {
        int client = accept ( socket_, nullptr, nullptr );
        
        handle_request(client);
        
        shutdown ( client, SHUT_RDWR );
        close ( client );
    }
}

void Server::set_up_server()
{
    socket_ = socket ( PF_INET, SOCK_STREAM, IPPROTO_TCP );
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

    if(bind( socket_, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) == -1)
    {
        close(socket_);

        throw std::runtime_error("Binding socket failed!");
    };

    if(listen( socket_, 10 ) == -1)
    {
        close(socket_);

        throw std::runtime_error("Listening socket failed!");
    };

    cycle_flag_ = true;
}

void Server::handle_request(int client_socket)
{
    try
    {
        std::string received_data;
        char buffer[1024];

        while (true)
        {
            ssize_t bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if(bytes <= 0)
            {
                throw std::runtime_error("Receiving data failed!");
            }

            buffer[bytes] = '\0';
            received_data.append(buffer, bytes);
            
            // '\n' - конец форматированного json-а в строку
            size_t position = received_data.find('\n');
            if (position != std::string::npos)
            {
                std::string message = received_data.substr(0, position);
                handle_response(client_socket, message);
                break;
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error handling client: " << ex.what() << std::endl;
    }
}

void Server::handle_response(int client_socket, const std::string& message)
{
    nlohmann::json request = nlohmann::json::parse(message);

    std::string type = request["type"];
    std::vector<double> vec = request["vector"];
    //хоть клиент и должен блокировать отправку не 4-мерного вектора, но все же
    if (vec.size() != 4)
    {
        throw std::runtime_error("Vector must have 4 elements");
    }

    nlohmann::json response;
    response["type"] = type;

    //нужны шаблоны
    //пока просто на 2 умножение
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
        //клиент должен блокировать отправку несуществующего типа
        throw std::runtime_error("Unknown type!");
    }

    // '\n' - конец форматированного json-а в строку
    std::string response_str = response.dump() + "\n";
    send_response(client_socket, response_str.data(), response_str.size());
}

void Server::send_response(int client_socket, const void* data, size_t length)
{
    const char* data_ptr = static_cast<const char*>(data);
    size_t remaining_size = length;

    while (remaining_size > 0)
    {
        ssize_t sent_size = send(client_socket, data_ptr, remaining_size, 0);
        if (sent_size == -1)
        {
            throw std::runtime_error("Sending response error!");
        }
        data_ptr += sent_size;
        remaining_size -= sent_size;
    }
}