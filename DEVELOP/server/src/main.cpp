#include "../include/Server.h"
#include "../include/Logger.h"

int main()
{
    Logger logger("../logs/log.txt");

    try
    {
        Server server(1100, logger);

        server.main_cycle();
    }
    catch(std::exception& ex)
    {
        logger.file_log(ex);
        std::cout << ex.what();
    }

    return 0;
}