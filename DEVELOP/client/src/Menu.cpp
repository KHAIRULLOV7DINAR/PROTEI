#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <cstdlib>

#include "../include/Menu.h"
#include "../../utils/utils.h"


//MenuItems:

//NameItem
NameItem::NameItem(AppSettings& app_settings) : app_settings_(app_settings){}

void NameItem::execute() const
{
    std::string name;

    std::cout << "\nEnter name for program:\n" << std::endl;
    std::cin >> name;
    app_settings_.set_name(name);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nName for the program was entered.\n" << std::endl;
}

//TypeItem
TypeItem::TypeItem(DataPool& data_pool) : data_pool_(data_pool){}

void TypeItem::execute() const
{
    std::string new_type;
    const std::array<std::string, 3>& allowed_types = data_pool_.get_allowed_types();

    std::cout << "\nEnter type for vector:\n" << std::endl;
    std::cin >> new_type;
    fix_input(new_type);

    if(std::find(allowed_types.begin(), allowed_types.end(), new_type) == allowed_types.end())
    {
        throw std::invalid_argument("Invalid type value!");
    }
    else
    {
        data_pool_.set_current_input_type(new_type);
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nType for the vector was entered.\n" << std::endl; 
}

//VectorItem
VectorItem::VectorItem(DataPool& data_pool) : data_pool_(data_pool){}

void VectorItem::execute() const
{
    const std::string& current_type = data_pool_.get_current_input_type();

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

//SettingsItem
SettingsItem::SettingsItem(AppSettings& app_settings) : app_settings_(app_settings){}

void SettingsItem::execute() const
{
    std::cout << "\nApp settings: \n"
              << "Name -\t\t" << app_settings_.get_name() <<std::endl
              << "Address -\t";
    app_settings_.print_address();
    std::cout << "Role -\t\t" << app_settings_.get_role() << std::endl
              << "i -\t\t" << app_settings_.get_i() << std::endl
              << "Library -\t" << app_settings_.get_library() << std::endl;
}

//HelpItem
void HelpItem::execute() const
{
    std::cout << "\nName -\t\tenter name for the program;\n"
            << "Type -\t\tenter type of the vector;\n"
            << "Vector -\tenter 4-d int vector;\n"
            << "Vectors -\tshow entered vectors\n"
            << "Settings -\tshow app settings arguments\n"
            << "Help -\t\tshow available commands\n"
            << "Exit -\t\texit the program\n"
            << "Send -\t\tsend the first vector in data pool to server\n"
            << "\nWarning! No multiple values are allowed in one string except for 4-d vector values! (cin.ignore used).\n"
            << std::endl;
}

//VectorsItem
VectorsItem::VectorsItem(DataPool& data_pool) : data_pool_(data_pool){}

void VectorsItem::execute() const
{
    data_pool_.print_vectors();
}

//SendItem
SendItem::SendItem(Client& client, DataPool& data_pool) : client_(client), data_pool_(data_pool){}

void SendItem::execute() const
{
    std::unique_ptr<BaseVector> first_vect = data_pool_.first();
    const std::string& current_type = first_vect->get_type();

    client_.get_vect(std::move(first_vect));


    std::unique_ptr<BaseVector> received;
    
    if (current_type == "int")
    {
        received = client_.receive_response<int>();
    }
    else if (current_type == "float")
    {
        received = client_.receive_response<float>();
    }
    else if (current_type == "double")
    {
        received = client_.receive_response<double>();
    }
    else
    {
        throw std::runtime_error("Unknown type for response!");
    }
    
    if (received)
    {
        std::cout << "Response received: ";
        received->print();

        data_pool_.insert(std::move(received));
    }
}

//ExitQuitItem
ExitQuitItem::ExitQuitItem(bool& menu_flag) : menu_flag_(menu_flag){}

void ExitQuitItem::execute() const
{
    std::cout << "\nExit from the menu\n" << std::endl;
    menu_flag_ = false;
}
//Menu
Menu::Menu(Logger& logger, bool& running) : logger_(logger), running_(running) {}

void Menu::add_item(const std::string& command, std::unique_ptr<MenuItem> item)
{
    items_[command] = std::move(item);
}

void Menu::show_menu()
{
    MenuItem* help_item = find_item("help");
    if (help_item)
    {
        help_item->execute();
    }

    while (running_)
    {
        std::cout << "\nEnter new command:\n" << std::endl;
        std::string cmd;
        std::cin >> cmd;
        fix_input(cmd);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        MenuItem* item = find_item(cmd);
        try
        {
            if (item)
                item->execute();
            else
                throw std::invalid_argument("Invalid command!");
        }
        catch (std::exception& ex)
        {
            logger_.simple_console_log(ex);
            logger_.file_log(ex);
        }
    }
}

MenuItem* Menu::find_item(const std::string& command)
{
    auto it = items_.find(command);
    if(it != items_.end())
    {
        return it->second.get();
    }
    else
    {
        return nullptr;
    }
}