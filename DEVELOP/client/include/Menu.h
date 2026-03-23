#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include "./AppSettings.h"
#include "./DataPool.h"
#include "../../utils/Logger.h"
#include "./Client.h"


//MenuItem
class MenuItem
{
public:
    virtual ~MenuItem() = default;
    
    virtual void execute() const = 0;
};

//наследники 
class NameItem : public MenuItem
{
public: 
    NameItem(AppSettings& app_settings);

    void execute() const override;
private:
    AppSettings& app_settings_;
};

class TypeItem : public MenuItem
{
public: 
    TypeItem(DataPool& data_pool);

    void execute() const override;
private:
    DataPool& data_pool_;
};

class VectorItem : public MenuItem
{
public: 
    VectorItem(DataPool& data_pool);

    void execute() const override;
private:
    DataPool& data_pool_;
};

class SettingsItem : public MenuItem
{
public: 
    SettingsItem(AppSettings& app_settings);

    void execute() const override;
private:
    AppSettings& app_settings_;
};

class HelpItem : public MenuItem
{
public: 
    void execute() const override;
};

class VectorsItem : public MenuItem
{
public: 
    VectorsItem(DataPool& data_pool);

    void execute() const override;
private:
    DataPool& data_pool_;
};

class SendItem : public MenuItem
{
public: 
    SendItem(Client& client, DataPool& data_pool);

    void execute() const override;
private:
    Client& client_;
    DataPool& data_pool_;
};

class ExitQuitItem : public MenuItem
{
public: 
    ExitQuitItem(bool& menu_flag);

    void execute() const override;
private:
    bool& menu_flag_;
};

//Menu
class Menu
{
/*
Инвариант класса:
сохранение инвариантов классов AppSettings, DataPool
*/
public:
    Menu(Logger& logger, bool& running);

    void add_item(const std::string& command, std::unique_ptr<MenuItem> item);
    void show_menu();

private:
    Logger& logger_;
    bool& running_; 
    std::unordered_map<std::string, std::unique_ptr<MenuItem>> items_;

    MenuItem* find_item(const std::string& command);
};