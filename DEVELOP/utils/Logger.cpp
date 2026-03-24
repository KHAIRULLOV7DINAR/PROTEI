#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>

#include "Logger.h"


static void ensure_log_directory()
{
    std::filesystem::path log_dir("../logs");
    if (!std::filesystem::exists(log_dir))
    {
        std::filesystem::create_directory(log_dir);
    }
}

std::string Logger::generate_log_filename()
{
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t_now);

    std::ostringstream oss;
    oss << std::put_time(&tm, "../logs/log_%Y-%m-%d_%H-%M-%S.txt");
    return oss.str();
}

void Logger::open_file(const std::string& filename)
{
    ensure_log_directory();
    file_.open(filename, std::ios::app);
    if (!file_.is_open())
    {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

Logger::Logger()
{
    open_file(generate_log_filename());
}

void Logger::file_log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (!file_.is_open())
    {
        return;
    }

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

void Logger::file_log(const std::exception& ex)
{
    file_log(ex.what());
}

void Logger::info_file_log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (!file_.is_open())
    {
        return;
    }

    time_t rt;
    time(&rt);
    struct tm* ti = localtime(&rt);

    file_ << "INFO:" << std::endl;
    file_ << asctime(ti);
    file_ << message << "\n" << std::endl;
}

void Logger::vector_log(std::vector<double> vect)
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto el : vect)
    {
        file_ << el << ' ';
    }
    
    file_ << '\n' << std::endl;
}

void Logger::console_log(std::exception& ex)
{
    std::lock_guard<std::mutex> lock(mutex_);
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
    std::lock_guard<std::mutex> lock(mutex_);
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
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << er << '\n' << std::endl;
}

void Logger::simple_console_log(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << msg << '\n' << std::endl;
}

void Logger::simple_console_log(std::exception& ex)
{
    simple_console_log(ex.what());
}