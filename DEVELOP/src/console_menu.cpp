#include <algorithm>
#include <limits>
#include <sstream>

#include "../include/console_menu.h"

int read_console_string(int argc, char** argv, Config & config)
{
    auto pmap = config.string_values;
    auto default_values = config.default_values;

    //Handling given arguments
    for(int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        auto pair_iter = pmap->find(arg);

        if(pair_iter != pmap->end() && i + 1 < argc)
        {
            if(pair_iter->second != "")
            {
                std::cout << "Argument was given more than once!" << std::endl;

                return 1;
            }
            else
            {
                (*pmap)[arg] = argv[++i];
            }
        }
        else
        {
            std::cout << "Invalid flag or missing value!" << std::endl;

            return 1;
        }
    }

    for (const auto& pair : *pmap)
    {
        if(pair.second == "")
        {
            (*pmap)[pair.first] = (*default_values)[pair.first];
        }
    }

    return 0;
}

int parse_console_string(Config& config)
{

    //Ip-address
    std::stringstream ss_ip((*config.string_values)["-a"]);
    std::string oct;

    while(std::getline(ss_ip, oct, '.'))
    {
        try
        {
            int int_oct = std::stoi(oct);
            if(0 <= int_oct && int_oct <= 255)
            {
                config.ip.push_back((unsigned char)int_oct);
            }
            else
            {
                std::cout << "Invalid ip-address octet value!\n" << std::endl;

                return 1;
            }
        }
        catch(std::exception & ex)
        {
            std::cout << ex.what() << std::endl;
            std::cout << "Invalid ip-address format!\n" << std::endl;

            return 1;
        }
    }

    if(config.ip.size() != 4)
    {
        std::cout << "Invalid ip-address format!\n" << std::endl;

        return 1;
    }

    //Port
    try
    {
        int int_port;
        int_port = std::stoi((*config.string_values)["-p"]);
        if(0 <= int_port && int_port <= 65535)
        {
            config.port = (unsigned short)int_port;
        }
        else
        {
            std::cout << "Invalid port value!\n" << std::endl;

            return 1;
        }
    }
    catch(std::exception & ex)
    {
        std::cout << (*config.string_values)["-p"];
        std::cout << ex.what() << std::endl;
        std::cout << "Invalid port format!\n" << std::endl;

        return 1;
    }

    //Role
    config.role = (*config.string_values)["-r"];

    //i
    try
    {
        config.i = std::stoi((*config.string_values)["-i"]);
    }
    catch(std::exception & ex)
    {
        std::cout << ex.what() << std::endl;
        std::cout << "Invalid i format!\n" << std::endl;

        return 1;
    }

    //Library
    config.library = (*config.string_values)["-L"];

    return 0;
}

int show_menu(Config& config)
{
    std::string& name = config.program_name;
    std::string& type = config.type;
    // std::vector<int>& vect = config.vect;

    bool menu_flag = true;

    std::string new_command;

    std::cout << "\nEnter one of the following commands:\nName - name for the program;\nType - type of the vector;\nVector - 4-d int vector;\nExit - exit from the menu;\nConsole - show enetered console string values;\nMenu - show entered menu values\n" << std::endl;
    std::cout << "Warning! No multiple values are allowed in one string except for 4-d vector values! (cin.ignore used).\n" << std::endl;

    while(menu_flag)
    {

        std::cout << "Enter new command:\n" << std::endl;

        std::cin >> new_command;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::transform(new_command.begin(), new_command.end(), new_command.begin(), ::tolower);

        if(new_command == "name")
        {
            std::cout << "\nEnter name for program:\n" << std::endl;

            std::cin >> name;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "\nName for the program was entered.\n" << std::endl;
        }
        else if(new_command == "type")
        {
            std::cout << "\nEnter type for vector:\n" << std::endl;

            std::string new_type;

            std::cin >> new_type;

            std::transform(new_type.begin(), new_type.end(), new_type.begin(), ::tolower);

            if(std::find(config.vector_types->begin(), config.vector_types->end(), new_type) == config.vector_types->end())
            {
                std::cout << "\nInvalid type value!\n" << std::endl;

                continue;
            }
            else
            {
                if(type != new_type)
                {
                    config.createTypedVector(new_type); 

                    type = new_type;
                }
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "\nType for the vector was entered.\n" << std::endl; 
        }
        else if(new_command == "vector")
        {
            if(config.type == "")
            {
                std::cout << "\nEnter type of vector first!\n" << std::endl;

                continue;
            }

            std::cout << "\nEnter 4-d " << type << " vector:\n" << std::endl;

            config.vect->clear();

            std::string str_num;

            for(int i = 0; i < 4; i++)
            {
                std::cout << "Enter remaining " << 4 - i << " values for vector.\n" << std::endl;

                //No ignore for entering 4 numbers in one string
                std::cin >> str_num;
                try
                {
                    if(type == "int")
                    {
                        auto* vec = dynamic_cast<Config::TypedVector<int>*>(config.vect.get());
                        if(vec)
                        {
                            vec->data.push_back(std::stoi(str_num));
                        }
                    }
                    else if(type == "float")
                    {
                        auto* vec = dynamic_cast<Config::TypedVector<float>*>(config.vect.get());
                        if(vec)
                        {
                            vec->data.push_back(std::stof(str_num));
                        }
                    }
                    else if(type == "double")
                    {
                        auto* vec = dynamic_cast<Config::TypedVector<double>*>(config.vect.get());
                        if(vec)
                        {
                            vec->data.push_back(std::stod(str_num));
                        }
                    }
                }
                catch(const std::exception & ex)
                {
                    i--;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << ex.what() << std::endl;
                    std::cout << "Invalid number for vector!\n" << std::endl;
                }
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "\nVector was entered.\n" <<std::endl;
        }
        else if(new_command == "menu")
        {
            std::cout << "\nEntered menu values: \n";
            std::cout << "Name - " << name << std::endl;
            std::cout << "Type - " << type << std::endl;
            std::cout << "Vector values: ";

            if(type == "int")
            {
                auto* vec = dynamic_cast<Config::TypedVector<int>*>(config.vect.get());
                if(vec)
                {
                    for(int val : vec->data) std::cout << val << ' ';
                }
            }
            else if(type == "float")
            {
                auto* vec = dynamic_cast<Config::TypedVector<float>*>(config.vect.get());
                if(vec)
                {
                    for(float val : vec->data) std::cout << val << ' ';
                }
            }
            else if(type == "double")
            {
                auto* vec = dynamic_cast<Config::TypedVector<double>*>(config.vect.get());
                if(vec)
                {
                    for(double val : vec->data) std::cout << val << ' ';
                }
            }

            std::cout << '\n' << std::endl;
        }
        else if(new_command == "console")
        {
            std::cout << "\nEntered console values \n";
            std::cout << "Ip-address - ";
            for(int i = 0; i < config.ip.size() - 1; i++)
            {
                std::cout << (unsigned short int)config.ip[i] << '.';
            }
            std::cout << (unsigned short int)config.ip[config.ip.size() - 1] << std::endl;

            std::cout << "Port - " << (unsigned short int)config.port << std::endl;
            std::cout << "Role - " << config.role << std::endl;
            std::cout << "i - " << config.i << std::endl;
            std::cout << "Library - " << config.library << std::endl;
        }
        else if(new_command == "exit")
        {
            std::cout << "\nExit from the menu\n" << std::endl;

            menu_flag = false;
        }

        else
        {
            std::cout << "\nInvalid command!\n" << std::endl;
        }
    }
    return 0;
}