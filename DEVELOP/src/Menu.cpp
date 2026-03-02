#include "../include/Menu.h"
#include "../include/log.h"

#include <iostream>
#include <limits>
#include <algorithm>


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
Menu::Menu(DataPool& data_pool, AppSettings& app_settings) : menu_flag_(true), app_settings_(app_settings), data_pool_(data_pool)
{
    add_menu_items();
};

void Menu::add_menu_items()
{
    // Команда name - ввод имени программы
    menu_items_.push_back(std::make_unique<MenuItem>("name", [this]()
    {
        this->input_name();
    }));
    
    // Команда type - выбор типа вектора
    menu_items_.push_back(std::make_unique<MenuItem>("type", [this]()
    {
        this->input_type();
    }));
    
    // Команда vector - ввод вектора
    menu_items_.push_back(std::make_unique<MenuItem>("vector", [this]()
    {
        this->input_vector();
    }));
    
    // Команда console - показать настройки из командной строки
    menu_items_.push_back(std::make_unique<MenuItem>("settings", [this]()
    {
        this->print_settings();
    }));
    
    // Команда exit - выход из программы
    menu_items_.push_back(std::make_unique<MenuItem>("exit", [this]()
    {
        this->exit();
    }));
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
    std::transform(new_type.begin(), new_type.end(), new_type.begin(), ::tolower);

    if(std::find(allowed_types.begin(), allowed_types.end(), new_type) == allowed_types.end())
    {
        //Invalid type (couldn't find input_type in allowed types list)
        throw std::invalid_argument("\nInvalid type value!\n");
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
        std::cout << "\nType is not entered!\n" << std::endl;
        return;
    }

    std::cout << "\nEnter 4 " << current_type << " values:\n" << std::endl;
    
    std::string str_num;
    std::vector<int> int_vec;
    std::vector<float> float_vec;
    std::vector<double> double_vec;
    
    for(int i = 0; i < 4; i++)
    {
        std::cout << "Enter value " << i + 1 << ": ";
        std::cin >> str_num;
        
        try
        {
            size_t pos;
            
            if (current_type == "int")
            {
                int num = std::stoi(str_num, &pos);
                if (pos != str_num.length())
                {
                    throw std::invalid_argument("Invalid integer format!");
                }
                int_vec.push_back(num);
                
                if (i == 3 && num == 0)
                {
                    int_vec.pop_back(); 
                    throw std::invalid_argument("W-component of vector cannot be zero!");
                }
            }
            else if (current_type == "float")
            {
                float num = std::stof(str_num, &pos);
                if (pos != str_num.length())
                {
                    throw std::invalid_argument("Invalid float format!");
                }
                float_vec.push_back(num);
                
                if (i == 3 && num == 0.0f)
                {
                    float_vec.pop_back();
                    throw std::invalid_argument("W-component of vector cannot be zero!");
                }
            }
            else if (current_type == "double")
            {
                double num = std::stod(str_num, &pos);
                if (pos != str_num.length())
                {
                    throw std::invalid_argument("Invalid double format!");
                }
                double_vec.push_back(num);
                
                if (i == 3 && num == 0.0)
                {
                    double_vec.pop_back();
                    throw std::invalid_argument("W-component of vector cannot be zero!");
                }
            }
        }
        catch(std::exception& ex)
        {
            i--; 
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            console_log(ex);
            continue;
        }
    }
    
    try
    {
        if (current_type == "int" && int_vec.size() == 4)
        {
            data_pool_.insert(std::make_unique<TypedVector<int>>(current_type, int_vec));
        }
        else if (current_type == "float" && float_vec.size() == 4)
        {
            data_pool_.insert(std::make_unique<TypedVector<float>>(current_type, float_vec));
        }
        else if (current_type == "double" && double_vec.size() == 4)
        {
            data_pool_.insert(std::make_unique<TypedVector<double>>(current_type, double_vec));
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nVector added successfully!\n " << std::endl;
    }
    catch(std::exception& ex)
    {
        console_log(ex);
    }
}

void Menu::print_settings()
{
    std::cout << "\nApp settings: \n";
    std::cout << "Name - " << app_settings_.get_name() <<std::endl;
    std::cout << "Ip-address - ";
    app_settings_.print_ip();
    std::cout << "Port - " << (unsigned short int)app_settings_.get_port() << std::endl;
    std::cout << "Role - " << app_settings_.get_role() << std::endl;
    std::cout << "i - " << app_settings_.get_i() << std::endl;
    std::cout << "Library - " << app_settings_.get_library() << std::endl;
}

void Menu::exit()
{
    std::cout << "\nExit from the menu\n" << std::endl;
    menu_flag_ = false;
}

void Menu::parse_command(std::string& command)
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

    std::cout << "\nEnter one of the following commands:\nName - name for the program;\nType - type of the vector;\nVector - 4-d int vector;\nExit - exit from the menu;\nSettings - show app settings arguments\n" << std::endl;
    std::cout << "Warning! No multiple values are allowed in one string except for 4-d vector values! (cin.ignore used).\n" << std::endl;

    while(menu_flag_)
    {
        std::cout << "Enter new command:\n" << std::endl;
        std::cin >> new_command;
        parse_command(new_command);
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
            console_log(ex);
        }
    }
}

MenuItem* Menu::find_item(const std::string& command) const
{
    for (const auto& item : menu_items_)
    {
        if (item->get_command_name() == command)
        {
            return item.get();
        }
    }
    return nullptr;
}