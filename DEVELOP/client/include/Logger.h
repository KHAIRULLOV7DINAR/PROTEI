#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>


class Logger
{
public:
    Logger(const std::string& filename = "../logs/log.txt");
    ~Logger();

    void file_log(const std::string& message);
    void file_log(std::exception& ex);

    void console_log(std::exception& ex);
    void console_log(const char* er);

    void simple_console_log(const char* er);
    void simple_console_log(std::exception& ex);
private:
    std::ofstream file_;
};