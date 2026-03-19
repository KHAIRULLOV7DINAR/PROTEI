#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

#include "../include/Client.h"

using json = nlohmann::json;

Client::Client(NetworkAddress& network_address) : network_address_(network_address)
{
    set_up_server_connection();
}

Client::~Client()
{
    if(server_socket_ != -1)
    {
        close(server_socket_);
    }
}

void Client::set_up_server_connection()
{
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1)
    {
        throw std::runtime_error("Socket creation failed!");
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(network_address_.get_port());;

    const char* str_ip = (network_address_.get_str_ip()).c_str();

    if (inet_pton(AF_INET, str_ip, &server_addr.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid address!");
    }

    if (connect(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        throw std::runtime_error("Connecttion failed!");
    }
}

void Client::get_vect(std::unique_ptr<BaseVector> vect)
{
    if (dynamic_cast<TypedVector<int>*>(vect.get()))
    {
        auto ptr = std::unique_ptr<TypedVector<int>>(static_cast<TypedVector<int>*>(vect.release()));
        send_vector<int>(std::move(ptr));
    }
    else if (dynamic_cast<TypedVector<float>*>(vect.get()))
    {
        auto ptr = std::unique_ptr<TypedVector<float>>(static_cast<TypedVector<float>*>(vect.release()));
        send_vector<float>(std::move(ptr));
    }
    else if (dynamic_cast<TypedVector<double>*>(vect.get()))
    {
        auto ptr = std::unique_ptr<TypedVector<double>>(static_cast<TypedVector<double>*>(vect.release()));
        send_vector<double>(std::move(ptr));
    }
    else
    {
        throw std::runtime_error("Unknown vector type!");
    }
}

template <typename T>
void Client::send_vector(std::unique_ptr<TypedVector<T>> vect)
{
    nlohmann::json request;
    request["type"] = vect->get_type();
    request["vector"] = vect->get_data();

    std::string request_str = request.dump() + "\n";
    send_raw_data(request_str.data(), request_str.size());
}

template <typename T>
std::unique_ptr<TypedVector<T>> Client::receive_response()
{
    std::string response_str = receive_raw_data();
    json response = json::parse(response_str);

    return std::make_unique<TypedVector<T>>(response["type"], response["vector"]);
}

void Client::send_raw_data(const void* data, size_t length)
{
    const char* ptr = static_cast<const char*>(data);
    size_t remaining = length;
    while (remaining > 0)
    {
        ssize_t sent = send(server_socket_, ptr, remaining, 0);
        if (sent == -1)
        {
            throw std::runtime_error(std::string("Sending error: ") + strerror(errno));
        }
        ptr += sent;
        remaining -= sent;
    }
}

std::string Client::receive_raw_data()
{
    std::string result;
    char ch;
    while (recv(server_socket_, &ch, 1, 0) == 1)
    {
        if (ch == '\n')
        {
            break;
        }
        result += ch;
    }
    return result;
}

template void Client::send_vector<int>(std::unique_ptr<TypedVector<int>>);
template void Client::send_vector<float>(std::unique_ptr<TypedVector<float>>);
template void Client::send_vector<double>(std::unique_ptr<TypedVector<double>>);

template std::unique_ptr<TypedVector<int>> Client::receive_response<int>();
template std::unique_ptr<TypedVector<float>> Client::receive_response<float>();
template std::unique_ptr<TypedVector<double>> Client::receive_response<double>();