#include <vector>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "../include/NetworkAddress.h"

const std::string& NetworkAddress::get_str_ip() const
{
    return str_ip_;
}

const std::array<std::byte, 4>& NetworkAddress::get_ip() const
{
    return ip_;
}

unsigned short NetworkAddress::get_port() const
{
    return port_;
}

void NetworkAddress::set_ip(const std::string& str_ip)
{
    str_ip_ = str_ip;

    std::array<std::byte, 4> new_ip{};
    std::stringstream ss_ip(str_ip);
    std::string oct;
    int index = 0;

    while (std::getline(ss_ip, oct, '.'))
    {
        if (index >= 4)
        {
            throw std::invalid_argument("Invalid ip-address: too many octets!");
        }

        size_t pos;
        int int_oct = std::stoi(oct, &pos);
        if (pos != oct.length())
        {
            throw std::invalid_argument("Invalid symbols in ip-oct value!");
        }

        if (int_oct < 0 || int_oct > 255)
        {
            throw std::invalid_argument("Invalid ip-address octet value!");
        }

        new_ip[index] = static_cast<std::byte>(int_oct);
        ++index;
    }

    if (index != 4)
    {
        throw std::invalid_argument("Invalid ip-address: wrong number of octets!");
    }

    ip_ = new_ip;
}

void NetworkAddress::set_ip(const std::vector<int>& vec_ip)
{
    if (vec_ip.size() != 4)
    {
        throw std::invalid_argument("Invalid ip-address format!");
    }

    for (size_t i = 0; i < 4; ++i)
    {
        int int_oct = vec_ip[i];
        if (int_oct < 0 || int_oct > 255)
        {
            throw std::invalid_argument("Invalid ip-octet value!");
        }
        ip_[i] = static_cast<std::byte>(int_oct);
    }
}

void NetworkAddress::set_ip(unsigned int hex_ip)
{
    ip_[0] = static_cast<std::byte>((hex_ip >> 24) & 0xFF);
    ip_[1] = static_cast<std::byte>((hex_ip >> 16) & 0xFF);
    ip_[2] = static_cast<std::byte>((hex_ip >> 8) & 0xFF);
    ip_[3] = static_cast<std::byte>(hex_ip & 0xFF);
}

void NetworkAddress::set_port(int port)
{
    if (port < 0 || port > 65535)
    {
        throw std::invalid_argument("Invalid port!");
    }
    port_ = static_cast<unsigned short>(port);
}

void NetworkAddress::print_ip() const
{
    for (size_t i = 0; i < ip_.size(); ++i)
    {
        if (i > 0)
        {
            std::cout << ".";
        }
        std::cout << static_cast<int>(ip_[i]);
    }
}

void NetworkAddress::print_address() const
{
    print_ip();
    std::cout << ':' << port_ << std::endl;
}