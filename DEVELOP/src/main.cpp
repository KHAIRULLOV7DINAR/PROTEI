#include "../include/console_menu.h"

int main(int argc, char** argv)
{
    Config config;

    //Reading console string values
    int read_console_string_flag = read_console_string(argc, argv, config);
    if(read_console_string_flag == 1)
    {
        return read_console_string_flag;
    }

    //Parsing console string values
    int parse_console_string_flag = parse_console_string(config);
    if(parse_console_string_flag == 1)
    {
        return parse_console_string_flag;
    }

    //CLI-Menu
    return show_menu(config);
}