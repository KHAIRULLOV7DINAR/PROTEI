#include "../include/Logger.h"


Logger::Logger(const std::string& filename)
{
    file_.open(filename, std::ios::app);
}

Logger::~Logger()
{
    if (file_.is_open())
    {
        file_.close();
    }
}

void Logger::file_log(const std::string& message)
{
    if (!file_.is_open()) return;

    time_t rt;
    time(&rt);
    struct tm* ti = localtime(&rt);

    file_ << "ERROR:" << std::endl;
    file_ << asctime(ti);
    file_ << message << "\n" << std::endl;
}

void Logger::file_log(std::exception& ex)
{
    file_log(ex.what());
}

void Logger::console_log(std::exception& ex)
{
    time_t rt;
    struct tm* ti;
    time(&rt);
    ti = localtime(&rt);

    std::cout << "ERROR:" << std::endl;
    std::cout << asctime(ti);
    std::cout << ex.what() << '\n' << std::endl;
}

void Logger::console_log(const char* er)
{
    time_t rt;
    struct tm* ti;
    time(&rt);
    ti = localtime(&rt);

    std::cout << "ERROR:" << std::endl;
    std::cout << asctime(ti);
    std::cout << er << '\n' << std::endl;
}

void Logger::simple_console_log(const char* er)
{
    std::cout << er << '\n' << std::endl;
}

void Logger::simple_console_log(std::exception& ex)
{
    simple_console_log(ex.what());
}