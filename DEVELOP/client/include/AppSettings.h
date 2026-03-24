#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "NetworkAddress.h"


class AppSettings
{
/*
Инвариант класса:
ip имеет формат ipv4 (4 октета в диапазоне 0-255);
port находится в диапазоне 0-65535;
название библиотеки непустое;
поле имени также непустое.
*/
public:
    AppSettings(int argc, char** argv, NetworkAddress& network_address);
    ~AppSettings() = default;

    // Геттеры
    const std::array<std::byte, 4>& get_ip() const;
    unsigned short get_port() const;
    NetworkAddress& get_network_address() const;
    std::string get_role() const;
    int get_i() const;
    std::string get_library() const;
    std::string get_name() const;

    //Сеттеры
    void set_name(std::string& name);

    void print_address() const;

private:
    // Мапы для хранения получаемых и дефолтных значений консольной строки
    std::unordered_map<std::string, std::string> string_values_;
    std::unordered_map<std::string, std::string> default_values_;

    // Аргументы командной строки
    NetworkAddress& network_address_;
    std::string role_;
    int i_ = 0;
    std::string library_;

    //Имя программы (получаем из меню)
    std::string name_;

    void read_console_string(int argc, char** argv);
    void parse_ip_string();
    void parse_port();
    void parse_i();
    void parse_console_string();
};