#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <string>
#include <vector>
#include <unordered_map>

class AppSettings
{
public:
    AppSettings(int argc, char** argv);
    ~AppSettings() = default;

    // Геттеры
    const std::vector<unsigned char>& get_ip() const;
    unsigned short get_port() const;
    std::string get_role() const;
    int get_i() const;
    std::string get_library() const;
    std::string get_name() const;

    void set_name(std::string& name);

    void print_ip() const;

private:
    // Мапы для хранения получаемых и дефолтных значений консольной строки
    std::unordered_map<std::string, std::string> string_values_;
    std::unordered_map<std::string, std::string> default_values_;

    // Аргументы командной строки
    std::vector<unsigned char> ip_;
    unsigned short port_ = 0;
    std::string role_;
    int i_ = 0;
    std::string library_;

    //Имя программы 
    std::string name_;

    void read_console_string(int argc, char** argv);
    void parse_ip_string();
    void parse_port();
    void parse_i();
    void parse_console_string();
};

#endif