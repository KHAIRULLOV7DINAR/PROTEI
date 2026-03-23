#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#include "../include/AppSettings.h"


AppSettings::AppSettings(int argc, char** argv, NetworkAddress& network_addreess) :
    string_values_{
        {"-a", ""},
        {"-p", ""},
        {"-r", ""},
        {"-i", ""},
        {"-L", ""}
    },
    default_values_{
        {"-a", "127.0.0.1"},
        {"-p", "5555"},
        {"-r", "Client"},
        {"-i", "0"},
        {"-L", "mylib"}
    },
    network_address_(network_addreess)
{
    read_console_string(argc, argv);
    parse_console_string();
}

const std::array<std::byte, 4>& AppSettings::get_ip() const
{
    return network_address_.get_ip();
}

unsigned short AppSettings::get_port() const
{
    return network_address_.get_port();
}

std::string AppSettings::get_role() const
{
    return role_;
}

int AppSettings::get_i() const
{
    return i_;
}

std::string AppSettings::get_library() const
{
    return library_;
}

std::string AppSettings::get_name() const
{
    return name_;
}

void AppSettings::set_name(std::string& name)
{
    //Нельзя установить пустое имя
    if(name.empty())
    {
        throw std::logic_error("Setting empty name is not allowed!");
    }
    name_ = name;
}

void AppSettings::print_address() const
{
    network_address_.print_address();
}

void AppSettings::read_console_string(int argc, char** argv)
{
    //Обработка консольной строки
    for(int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        auto pair_iter = string_values_.find(arg);

        if(pair_iter != string_values_.end() && i + 1 < argc)
        {
            if(pair_iter->second != "")
            {
                throw std::logic_error("Argument was given more than once!");
            }
            else
            {
                string_values_[arg] = argv[++i];
            }
        }
        else
        {
            throw std::invalid_argument("Invalid flag or missing value!");
        }
    }

    for (const auto& pair : string_values_)
    {
        if(pair.second == "")
        {
            if(pair.first == "-a" || pair.first == "-L")
            {
                throw std::logic_error("Arguments with -a, -L flags are obligatory!");
            }

            string_values_[pair.first] = default_values_[pair.first];
        }
    }
}

void AppSettings::parse_ip_string()
{
    network_address_.set_ip(string_values_["-a"]);
}

void AppSettings::parse_port()
{
    //exception could be thrown in stoi func. In that case it's gonna be caught in main func.
    size_t pos;
    int int_port = std::stoi(string_values_["-p"], &pos);
    if (pos != string_values_["-p"].length())
    {
        throw std::invalid_argument("Invalid symbols in port value!");
    }
    
    if(0 <= int_port && int_port <= 65535)
    {
        network_address_.set_port(static_cast<unsigned short>(int_port));
    }
    else
    {
        throw std::invalid_argument("Invalid port value!");
    }
}

void AppSettings::parse_i()
{
    //exception could be thrown in stoi func. In that case it's gonna be caught in main func.
    size_t pos;
    i_ = std::stoi(string_values_["-i"], &pos);
    if (pos != string_values_["-i"].length())
    {
        throw std::invalid_argument("Invalid symbols in i value!");
    }
}

void AppSettings::parse_console_string()
{
    //Ip-адрес
    parse_ip_string();

    //Порт
    parse_port();

    //Роль
    role_ = string_values_["-r"];

    //i
    parse_i();

    //Библиотека
    library_ = string_values_["-L"];
}
