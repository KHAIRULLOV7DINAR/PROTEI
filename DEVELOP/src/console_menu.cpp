#include <algorithm>
#include <limits>
#include <sstream>

#include "../include/console_menu.h"
#include "../include/log.h"


void read_console_string(int argc, char** argv, Config & config)
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
                throw std::logic_error("Argument was given more than once!");
            }
            else
            {
                (*pmap)[arg] = argv[++i];
            }
        }
        else
        {
            throw std::invalid_argument("Invalid flag or missing value!");
        }
    }

    for (const auto& pair : *pmap)
    {
        if(pair.second == "")
        {
            if(pair.first == "-a" || pair.first == "-L")
            {
                throw std::logic_error("Arguments with -a, -L flags are obligatory!\n");
            }

            (*pmap)[pair.first] = (*default_values)[pair.first];
        }
    }
}

void parse_ip_string(Config& config)
{
    std::stringstream ss_ip((*config.string_values)["-a"]);
    std::string oct;

    while(std::getline(ss_ip, oct, '.'))
    {
        //exception could be thrown in stoi func. In that case it's gonna be caught in main func.
        int int_oct = std::stoi(oct);
        if(0 <= int_oct && int_oct <= 255)
        {
            config.ip.push_back((unsigned char)int_oct);
        }
        else
        {
            throw std::invalid_argument("Invalid ip-address octet value!\n");
        }
    }

    if(config.ip.size() != 4)
    {
        throw std::invalid_argument("Invalid ip-address format!\n");
    }
}

void parse_port(Config& config)
{
    //exception could be thrown in stoi func. In that case it's gonna be caught in main func.
    int int_port = std::stoi((*config.string_values)["-p"]);
    if(0 <= int_port && int_port <= 65535)
    {
        config.port = (unsigned short)int_port;
    }
    else
    {
        throw std::invalid_argument("Invalid port value!\n");
    }
}

void parse_i(Config& config)
{
        //exception could be thrown in stoi func. In that case it's gonna be caught in main func.
        config.i = std::stoi((*config.string_values)["-i"]);
}

void parse_console_string(Config& config)
{
    //Ip-address
    parse_ip_string(config);

    //Port
    parse_port(config);

    //Role
    config.role = (*config.string_values)["-r"];

    //i
    parse_i(config);

    //Library
    config.library = (*config.string_values)["-L"];
}

//===============================================
//MENU FUNCTIONS
//===============================================

void input_name(std::string& name)
{
    std::cout << "\nEnter name for program:\n" << std::endl;

    std::cin >> name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nName for the program was entered.\n" << std::endl;
}

void input_type(Config& config)
{
    std::string new_type;

    std::cout << "\nEnter type for vector:\n" << std::endl;

    std::cin >> new_type;
    std::transform(new_type.begin(), new_type.end(), new_type.begin(), ::tolower);

    if(std::find(config.vector_types->begin(), config.vector_types->end(), new_type) == config.vector_types->end())
    {
        //Invalid type (couldn't find input_type in allowed types list)
        throw std::invalid_argument("\nInvalid type value!\n");
    }
    else
    {
        if(config.type != new_type)
        {
            config.createTypedVector(new_type); 

            config.type = new_type;
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nType for the vector was entered.\n" << std::endl; 
}

void input_vector(Config& config)
{
    if(config.type == "")
    {
        throw std::logic_error("\nEnter type of vector first!\n");
    }

    std::cout << "\nEnter 4-d " << config.type << " vector:\n" << std::endl;

    config.vect->clear();

    std::string str_num;

    for(int i = 0; i < 4; i++)
    {
        std::cout << "Enter remaining " << 4 - i << " values for vector.\n" << std::endl;

        //No ignore for entering 4 numbers in one string
        std::cin >> str_num;
        try
        {
            if(config.type == "int")
            {
                auto* vec = dynamic_cast<Config::TypedVector<int>*>(config.vect.get());
                if(vec)
                {
                    vec->data.push_back(std::stoi(str_num));
                }
            }
            else if(config.type == "float")
            {
                auto* vec = dynamic_cast<Config::TypedVector<float>*>(config.vect.get());
                if(vec)
                {
                    vec->data.push_back(std::stof(str_num));
                }
            }
            else if(config.type == "double")
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
            console_log("Invalid number for vector!\n");
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nVector was entered.\n" <<std::endl;
}

void print_menu_values(Config& config)
{
    std::cout << "\nEntered menu values: \n";
    std::cout << "Name - " << config.program_name << std::endl;
    std::cout << "Type - " << config.type << std::endl;
    std::cout << "Vector values: ";

    if(config.type == "int")
    {
        auto* vec = dynamic_cast<Config::TypedVector<int>*>(config.vect.get());
        if(vec)
        {
            for(int val : vec->data) std::cout << val << ' ';
        }
    }
    else if(config.type == "float")
    {
        auto* vec = dynamic_cast<Config::TypedVector<float>*>(config.vect.get());
        if(vec)
        {
            for(float val : vec->data) std::cout << val << ' ';
        }
    }
    else if(config.type == "double")
    {
        auto* vec = dynamic_cast<Config::TypedVector<double>*>(config.vect.get());
        if(vec)
        {
            for(double val : vec->data) std::cout << val << ' ';
        }
    }

    std::cout << '\n' << std::endl;
}

void print_console_values(Config& config)
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

int show_menu(Config& config)
{
    std::string& name = config.program_name;
    std::string& type = config.type;

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
            input_name(name);
        }
        else if(new_command == "type")
        {
            try
            {
                input_type(config);
            }
            catch(std::exception& ex)
            {
                console_log(ex);
            }
        }
        else if(new_command == "vector")
        {
            try
            {
                input_vector(config);
            }
            catch(std::exception& ex)
            {
                console_log(ex);
            }
        }
        else if(new_command == "menu")
        {
            print_menu_values(config);
        }
        else if(new_command == "console")
        {
            print_console_values(config);
        }
        else if(new_command == "exit")
        {
            std::cout << "\nExit from the menu\n" << std::endl;

            menu_flag = false;
        }
        else
        {
            console_log("\nInvalid command!\n");
        }
    }
    return 0;
}