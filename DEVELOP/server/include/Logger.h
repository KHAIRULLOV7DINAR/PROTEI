#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>


class Logger
{
public:
    Logger(const std::string& filename = "../logs/log.txt");
    ~Logger();

    void file_log(const std::string& message);
    void file_log(std::exception& ex);

    void vector_log(std::vector<double> vect);

    void info_file_log(const std::string& message);

    void console_log(std::exception& ex);
    void console_log(const char* er);

    void simple_console_log(const char* er);
    void simple_console_log(std::exception& ex);
private:
    std::ofstream file_;
};

#endif