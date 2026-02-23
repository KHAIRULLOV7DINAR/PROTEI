#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include "./config.hpp"

int read_console_string(int argc, char** argv, Config & config);
int parse_console_string(Config& config);
int show_menu(Config& config);

#endif