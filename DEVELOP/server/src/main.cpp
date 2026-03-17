#include "../include/Server.h"

int main()
{
    try
    {
        Server server(1100);

        server.main_cycle();
    }
    catch(std::exception& ex)
    {
        std::cout << ex.what();
    }

    return 0;
}