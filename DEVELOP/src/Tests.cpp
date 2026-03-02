#include "../include/Tests.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <sys/stat.h>
#include <stdexcept>

void ConnectionTest::set_params(const std::vector<std::string>& params)
{
    if (params.empty())
    {
        throw std::invalid_argument("ConnectionTest: no parameters provided!");
    }
    addresses_ = params;
}

void ConnectionTest::validate_ip(const std::string& ip) const
{
    std::stringstream ss(ip);
    std::string octet;
    int count = 0;
    
    while (std::getline(ss, octet, '.'))
    {
        count++;
        
        if (octet.empty())
        {
            throw std::invalid_argument("Invalid IP: empty octet!");
        }

        for (char c : octet)
        {
            if (!std::isdigit(static_cast<unsigned char>(c)))
            {
                throw std::invalid_argument("Invalid IP: non-digit character!");
            }
        }
        
        int value = std::stoi(octet);
        if (value < 0 || value > 255)
        {
            throw std::invalid_argument("Invalid IP: octet out of range!");
        }
    }
    
    if (count != 4)
    {
        throw std::invalid_argument("Invalid IP: wrong number of octets!");
    }
}

void ConnectionTest::validate_port(const std::string& port) const
{
    if (port.empty())
    {
        throw std::invalid_argument("Invalid port: empty!");
    }
    
    for (char c : port)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            throw std::invalid_argument("Invalid port: non-digit character!");
        }
    }
    
    int value = std::stoi(port);
    if (value < 0 || value > 65535)
    {
        throw std::invalid_argument("Invalid port: out of range!");
    }
}

void ConnectionTest::test()
{
    if (addresses_.empty())
    {
        throw std::logic_error("ConnectionTest: parameters not set!");
    }
    
    std::cout << "\nConnection Test Results:\n";
    
    for (const auto& addr : addresses_)
    {
        std::cout << "  " << addr << " -> ";
        
        size_t colon_pos = addr.find(':');
        
        if (colon_pos == std::string::npos)
        {
            try
            {
                validate_ip(addr);
                std::cout << "Valid IP\n";
            }
            catch (const std::exception& e)
            {
                std::cout << "Invalid IP\n";
                throw;
            }
        }
        else
        {
            std::string ip = addr.substr(0, colon_pos);
            std::string port = addr.substr(colon_pos + 1);
            
            try
            {
                validate_ip(ip);
                validate_port(port);
                std::cout << "Valid IP:port\n";
            }
            catch (const std::exception& e)
            {
                std::cout << "Invalid format\n";
                throw;
            }
        }
    }
}

void ResourceTest::set_params(const std::vector<std::string>& params)
{
    if (params.empty())
    {
        throw std::invalid_argument("ResourceTest: no parameters provided!");
    }
    paths_ = params;
}

void ResourceTest::validate_file_exists(const std::string& path) const
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0)
    {
        throw std::runtime_error("File not found: " + path);
    }
}

void ResourceTest::test()
{
    if (paths_.empty())
    {
        throw std::logic_error("ResourceTest: parameters not set!");
    }
    
    std::cout << "\nResource Test Results:\n";
    
    for (const auto& path : paths_)
    {
        std::cout << "  " << path << " -> ";
        
        try
        {
            validate_file_exists(path);
            std::cout << "Found\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "Not found\n";
            throw;
        }
    }
}