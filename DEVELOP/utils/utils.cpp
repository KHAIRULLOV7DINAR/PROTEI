#include <cctype>
#include <string>

#include "utils.h"


void fix_input(std::string& command)
{
    if(command.empty())
    {
        return;
    }

    char* char_ptr = &command[0];

    for (char c : command)
    {
        if (std::isalnum(static_cast<unsigned char>(c)))
        {
            *char_ptr = std::tolower(static_cast<unsigned char>(c));
            ++char_ptr;
        }
    }
    command.resize(char_ptr - &command[0]);
}