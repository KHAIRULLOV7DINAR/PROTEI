#include <iostream>
#include <memory>

#include "../include/AppSettings.h"
#include "../include/DataPool.h"
#include "../include/Menu.h"
#include "../../utils/Logger.h"
#include "../include/Client.h"


int main(int argc, char** argv)
{
    Logger logger;
    logger.file_log("Program started!\n");

    try
    {
        NetworkAddress network_address;
        AppSettings app_settings(argc, argv, network_address);
        DataPool data_pool;
        Client client(network_address);

        bool running = true;
        Menu menu(logger, running);

        menu.add_item("name",    std::make_unique<NameItem>(app_settings));
        menu.add_item("type",    std::make_unique<TypeItem>(data_pool));
        menu.add_item("vector",  std::make_unique<VectorItem>(data_pool));
        menu.add_item("settings",std::make_unique<SettingsItem>(app_settings));
        menu.add_item("help",    std::make_unique<HelpItem>());
        menu.add_item("vectors", std::make_unique<VectorsItem>(data_pool));
        menu.add_item("send",    std::make_unique<SendItem>(client, data_pool));
        menu.add_item("exit",    std::make_unique<ExitQuitItem>(running));
        menu.add_item("quit",    std::make_unique<ExitQuitItem>(running));

        menu.show_menu();
    }
    catch (std::exception& ex)
    {
        logger.console_log(ex);
    }

    return 0;
}