#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include "./config.hpp"

void read_console_string(int argc, char** argv, Config & config);
void parse_ip_string(Config& config);
void parse_port(Config& config);
void parse_i(Config& config);
void parse_console_string(Config& config);

void parse_command(std::string& command);
void input_name(std::string& name);
void input_type(Config& config);
void check_W(Config& config);
void vector_push(std::string& str_num, Config& config, int i);
void input_vector(Config& config);
void print_menu_values(Config& config);
void print_console_values(Config& config);
int show_menu(Config& config);

#endif