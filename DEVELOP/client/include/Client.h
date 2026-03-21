#pragma once

#include "NetworkAddress.h"
#include "DataPool.h"

class Client
{
public:
    Client(NetworkAddress& network_address);
    ~Client();
    void get_vect(std::unique_ptr<BaseVector> vec);
    template <typename T>
    void send_vector(std::unique_ptr<TypedVector<T>> vect);
    template <typename T>
    std::unique_ptr<TypedVector<T>> receive_response();
private:
    NetworkAddress& network_address_;
    int server_socket_;

    void set_up_server_connection();
    void send_raw_data(const void* data, size_t length);
    std::string receive_raw_data();
};