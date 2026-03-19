#include <iostream>
#include <memory>

#include "../include/AppSettings.h"
#include "../include/DataPool.h"
#include "../include/Menu.h"
#include "../include/Logger.h"
#include "../include/Client.h"


int main(int argc, char** argv)
{
    // Address sanitizer check (раскомментировать для теста)
    // int arr[4];
    // arr[5] = 12;

    Logger logger("../logs/log.txt");

    logger.file_log("Program started!\n");
    
    try
    {
        NetworkAddress network_address;
        AppSettings app_settings(argc, argv, network_address);
        DataPool data_pool;
        Client client(network_address);
        Menu menu(logger, data_pool, app_settings, client);
        
        menu.show_menu();
    }
    catch (std::exception& ex)
    {
        logger.console_log(ex);
    }
    
    return 0;
}