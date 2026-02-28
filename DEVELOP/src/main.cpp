#include "../include/console_menu.h"
#include "../include/log.h"

int main(int argc, char** argv)
{
    Config config;

    //address sanitizer check
    //int arr[4];
    //arr[5] = 12;

    //Reading console string values
    try
    {
        read_console_string(argc, argv, config);
    }
    catch(std::exception& ex)
    {
        console_log(ex);

        return 1;
    }

    //Parsing console string values
    try
    {
        parse_console_string(config);
    }
    catch(std::exception& ex)
    {
        console_log(ex);

        return 1;
    }

    //CLI-Menu
    return show_menu(config);
}