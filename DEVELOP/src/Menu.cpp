#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <cstdlib>

#include "../include/Menu.h"
#include "../include/Logger.h"


//MenuItem
MenuItem::MenuItem(const std::string& command_name, std::function<void()> f_action)
    : command_name_(command_name), f_action_(f_action){}

const std::string& MenuItem::get_command_name() const
{
    return command_name_;
}

void MenuItem::execute() const
{
    if (f_action_)
    {
        f_action_();
    }
}

//Menu
Menu::Menu(Logger& file_logger, DataPool& data_pool, AppSettings& app_settings) :logger_(file_logger), menu_flag_(true), app_settings_(app_settings), data_pool_(data_pool)
{
    add_menu_items();
};

void Menu::add_menu_items()
{
    // Команда name - ввод имени программы
    menu_items_["name"] = std::make_unique<MenuItem>("name", [this]()
    {
        this->input_name();
    });
    
    // Команда type - выбор типа вектора
    menu_items_["type"] = std::make_unique<MenuItem>("type", [this]()
    {
        this->input_type();
    });
    
    // Команда vector - ввод вектора
    menu_items_["vector"] = std::make_unique<MenuItem>("vector", [this]()
    {
        this->input_vector();
    });
    
    // Команда console - показать настройки из командной строки
    menu_items_["settings"] = std::make_unique<MenuItem>("settings", [this]()
    {
        this->print_settings();
    });
    
    // Команда exit - выход из программы
    menu_items_["exit"] = std::make_unique<MenuItem>("exit", [this]()
    {
        this->exit();
    });

    // Команда quit - выход из программы
    menu_items_["quit"] = std::make_unique<MenuItem>("quit", [this]()
    {
        this->quit();
    });

    // Команда help - вывод справки по командам
    menu_items_["help"] = std::make_unique<MenuItem>("help", [this]()
    {
        this->print_help();
    });

    // Команда vectors - вывод записанных векторов
    menu_items_["vectors"] = std::make_unique<MenuItem>("vectors", [this]()
    {
        this->print_vectors();
    });
}

void Menu::input_name()
{
    std::string name;

    std::cout << "\nEnter name for program:\n" << std::endl;
    std::cin >> name;
    app_settings_.set_name(name);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nName for the program was entered.\n" << std::endl;
}

void Menu::input_type()
{
    std::string new_type;
    const std::vector<std::string>& allowed_types = data_pool_.get_allowed_types();

    std::cout << "\nEnter type for vector:\n" << std::endl;
    std::cin >> new_type;
    parse_input(new_type);

    if(std::find(allowed_types.begin(), allowed_types.end(), new_type) == allowed_types.end())
    {
        //Invalid type (couldn't find input_type in allowed types list)
        throw std::invalid_argument("Invalid type value!");
    }
    else
    {
        current_type = new_type;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nType for the vector was entered.\n" << std::endl; 
}

void Menu::input_vector()
{
    if (current_type.empty())
    {
        throw std::logic_error("Type is not entered!");
    }

    std::cout << "\nEnter 4 " << current_type << " values separated by spaces:\n" << std::endl;
    
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::vector<std::string> str_nums;
    std::string str_num;
    while (ss >> str_num)
    {
        str_nums.push_back(str_num);
    }

    if (str_nums.size() != 4)
    {
        throw std::invalid_argument("Invalid amount of vector values was given!");
    }

    if (current_type == "int")
    {
        std::vector<int> vec;
        for (const auto& t : str_nums)
        {
            size_t pos;
            int num = std::stoi(t, &pos);
            if (pos != t.length())
            {
                throw std::invalid_argument("Invalid integer format!");
            }
            vec.push_back(num);
        }
        if (vec[3] == 0)
        {
            throw std::invalid_argument("W-component of vector cannot be zero!");
        }
        data_pool_.insert(std::make_unique<TypedVector<int>>(current_type, vec));
    }
    else if (current_type == "float")
    {
        std::vector<float> vec;
        for (const auto& t : str_nums)
        {
            size_t pos;
            float num = std::stof(t, &pos);
            if (pos != t.length())
            {
                throw std::invalid_argument("Invalid float format!");
            }
            vec.push_back(num);
        }
        if (vec[3] == 0.0f)
        {
            throw std::invalid_argument("W-component of vector cannot be zero!");
        }
        data_pool_.insert(std::make_unique<TypedVector<float>>(current_type, vec));
    }
    else if (current_type == "double")
    {
        std::vector<double> vec;
        for (const auto& t : str_nums)
        {
            size_t pos;
            double num = std::stod(t, &pos);
            if (pos != t.length())
            {
                throw std::invalid_argument("Invalid double format!");
            }
            vec.push_back(num);
        }
        if (vec[3] == 0.0)
        {
            throw std::invalid_argument("W-component of vector cannot be zero!");
        }
        data_pool_.insert(std::make_unique<TypedVector<double>>(current_type, vec));
    }
    else
    {
        throw std::logic_error("Invalid type!");
    }

    std::cout << "\nVector added successfully!\n" << std::endl;
}

void Menu::print_settings() const
{
    std::cout << "\nApp settings: \n"
              << "Name -\t\t" << app_settings_.get_name() <<std::endl
              << "Address -\t";
    app_settings_.print_address();
    std::cout << "Role -\t\t" << app_settings_.get_role() << std::endl
              << "i -\t\t" << app_settings_.get_i() << std::endl
              << "Library -\t" << app_settings_.get_library() << std::endl;
}

void Menu::print_help() const
{
    std::cout << "\nName -\t\tenter name for the program;\n"
              << "Type -\t\tenter type of the vector;\n"
              << "Vector -\tenter 4-d int vector;\n"
              << "Vectors -\tshow entered vectors\n"
              << "Settings -\tshow app settings arguments\n"
              << "Help -\t\tshow available commands\n"
              << "Exit -\t\texit the program\n"
              << "\nWarning! No multiple values are allowed in one string except for 4-d vector values! (cin.ignore used).\n"
              << std::endl;
}

void Menu::print_vectors() const
{
    data_pool_.print_vectors();
}

void Menu::exit()
{
    std::cout << "\nExit from the menu\n" << std::endl;
    menu_flag_ = false;
}

//Чем конкретно должно отличаться от exit????????
void Menu::quit()
{
    std::cout << "\nQuit from the menu\n" << std::endl;
    exit();
}

void Menu::parse_input(std::string& command)
{
    std::string result;
    for (char c : command)
    {
        if (std::isalnum(static_cast<unsigned char>(c)))
        {
            result += std::tolower(static_cast<unsigned char>(c));
        }
    }
    command = result;
}

void Menu::show_menu()
{
    std::string new_command;

    std::cout << "\nEnter one of the following commands:";

    print_help();

    while(menu_flag_)
    {
        std::cout << "\nEnter new command:\n" << std::endl;
        std::cin >> new_command;
        parse_input(new_command);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  

        MenuItem* item = find_item(new_command);
        try
        {
            if(item)
            {
                item->execute();
            }
            else
            {
                throw std::invalid_argument("Invalid command!");
            }
        }
        catch(std::exception& ex)
        {
            logger_.simple_console_log(ex);
            logger_.file_log(ex);
        }
    }
}

MenuItem* Menu::find_item(const std::string& command)
{
    auto pair = menu_items_.find(command);
    if (pair != menu_items_.end())
    {
        return pair->second.get();
    }
    return nullptr;
}