#include <ctime>

#include "../include/log.h"

void console_log(std::exception& ex)
{
    time_t rt;
    struct tm* ti;
    time(&rt);
    ti = localtime(&rt);

    std::cout << "ERROR:" << std::endl;
    std::cout << asctime(ti);
    std::cout << ex.what() << '\n' << std::endl;
}

void console_log(const char* er)
{
    time_t rt;
    struct tm* ti;
    time(&rt);
    ti = localtime(&rt);

    std::cout << "ERROR:" << std::endl;
    std::cout << asctime(ti);
    std::cout << er << '\n' << std::endl;
}