#pragma once

#include <string>
#include <array>


class NetworkAddress
{
public:

    const std::string& get_str_ip() const;
    
    const std::array<std::byte, 4>& get_ip() const;
    unsigned short get_port() const;

    void set_ip(const std::string& str_ip);
    void set_ip(const std::vector<int>& vec_ip);
    void set_ip(unsigned int hex_ip);

    void set_port(int port);

    void print_ip() const;
    void print_address() const;

private:
    std::array<std::byte, 4> ip_;
    std::string str_ip_;
    unsigned short port_ = 0;
};