#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#include "../include/log.h"
#include "../include/AppSettings.h"


AppSettings::AppSettings(int argc, char** argv) :
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
    }
{
    read_console_string(argc, argv);
    parse_console_string();
}

const std::vector<unsigned char>& AppSettings::get_ip() const
{
    return ip_;
}

unsigned short AppSettings::get_port() const
{
    return port_;
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
    name_ = name;
}

void AppSettings::print_ip() const
{
    for (int i = 0; i < ip_.size(); ++i)
    {
        if (i > 0) std::cout << ".";
        std::cout << static_cast<int>(ip_[i]);
    }
    std::cout << std::endl;
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
    std::stringstream ss_ip(string_values_["-a"]);
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
            ip_.push_back((unsigned char)int_oct);
        }
        else
        {
            throw std::invalid_argument("Invalid ip-address octet value!");
        }
    }

    if(ip_.size() != 4)
    {
        throw std::invalid_argument("Invalid ip-address format!");
    }
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
        port_ = (unsigned short)int_port;
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
