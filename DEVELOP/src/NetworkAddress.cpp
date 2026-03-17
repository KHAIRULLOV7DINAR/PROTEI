#include <vector>
#include <sstream>
#include <cstdint>
#include <iostream>

#include "../include/NetworkAddress.h"


const std::vector<unsigned char>& NetworkAddress::get_ip() const
{
    return ip_;
}

unsigned short NetworkAddress::get_port() const
{
    return port_;
}

void NetworkAddress::set_ip(const std::string& str_ip)
{
    std::vector<unsigned char> new_ip;

    std::stringstream ss_ip(str_ip);
    std::string oct;

    while(std::getline(ss_ip, oct, '.'))
    {
        //exception could be thrown in stoi func. In that case it's gonna be caught in main func.
        size_t pos;
        int int_oct = std::stoi(oct, &pos);
        if (pos != oct.length())
        {
            throw std::invalid_argument("Invalid symbols in ip-oct value!");
        }

        if(0 <= int_oct && int_oct <= 255)
        {
            new_ip.push_back(static_cast<unsigned char>(int_oct));
        }
        else
        {
            throw std::invalid_argument("Invalid ip-address octet value!");
        }
    }

    if(new_ip.size() != 4)
    {
        throw std::invalid_argument("Invalid ip-address format!");
    }

    ip_.clear();
    ip_ = new_ip;
}

void NetworkAddress::set_ip(const std::vector<int>& vec_ip)
{
    if(vec_ip.size() != 4)
    {
        throw std::invalid_argument("Invalid ip-address format!");
    }

    std::vector<unsigned char> new_ip;

    for(int int_oct : vec_ip)
    {
        if(0 > int_oct || int_oct > 255)
        {
            throw std::invalid_argument("Invalid ip-octect value!");
        }
        new_ip.push_back(static_cast<unsigned char>(int_oct));
    }

    ip_.clear();
    ip_ = new_ip;
}

void NetworkAddress::set_ip(unsigned int hex_ip)
{
    std::vector<unsigned char> new_ip;

    new_ip.push_back(static_cast<unsigned char>((hex_ip >> 24) & 0xFF));
    new_ip.push_back(static_cast<unsigned char>((hex_ip >> 16) & 0xFF));
    new_ip.push_back(static_cast<unsigned char>((hex_ip >> 8) & 0xFF));
    new_ip.push_back(static_cast<unsigned char>(hex_ip & 0xFF));

    ip_.clear();
    ip_ = new_ip;
}

void NetworkAddress::set_port(int port)
{
    if(0 > port || port > 65535)
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