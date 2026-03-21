#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include "./AppSettings.h"
#include "./DataPool.h"
#include "./Logger.h"
#include "./Client.h"


//MenuItem
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
/*
Инвариант класса:
сохранение инвариантов классов AppSettings, DataPool
*/
public:
    Menu(Logger& file_logger, DataPool& data_pool, AppSettings& app_settings, Client& client);
    ~Menu() = default;
    
    static void parse_input(std::string& command);
    void show_menu();

    std::string current_type;

protected:
    Logger& logger_;
    DataPool& data_pool_;
    AppSettings& app_settings_;
    Client& client_;
    bool menu_flag_;
    std::unordered_map<std::string, std::unique_ptr<MenuItem>> menu_items_;
    void add_menu_items();
    void input_name();
    void input_type();
    void input_vector();
    void print_settings() const;
    void print_help() const;
    void print_vectors() const;
    void send_vector();
    void quit();
    void exit();
    MenuItem* find_item(const std::string& command);
};