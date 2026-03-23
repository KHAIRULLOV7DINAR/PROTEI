#include "../include/Server.h"
#include "../../utils/Logger.h"

int main()
{
    Logger logger;

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