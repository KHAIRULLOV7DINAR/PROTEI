#ifndef MENU_H
#define MENU_H

#include <memory>
#include <string>
#include <vector>
#include <functional>

#include "./AppSettings.h"
#include "./DataPool.h"


//Menu
class MenuItem
{
public:
    MenuItem(const std::string& command_name, std::function<void()> f_action);
    
    const std::string& get_command_name() const;
    void execute() const;

private:
    std::string command_name_;
    std::function<void()> f_action_;
};

//Menu
class Menu
{
public:
    Menu(DataPool& data_pool, AppSettings& app_settings);
    ~Menu() = default;
    
    static void parse_command(std::string& command);
    void show_menu();

    std::string current_type;

private:
    DataPool& data_pool_;
    AppSettings& app_settings_;
    bool menu_flag_;
    std::vector<std::unique_ptr<MenuItem>> menu_items_;

    void add_menu_items();
    void input_name();
    void input_type();
    void input_vector();
    void print_settings();
    void print_vectors();
    void exit();
    MenuItem* find_item(const std::string& command) const;
};

#endif