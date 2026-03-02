#include "../include/AppSettings.h"
#include "../include/DataPool.h"
#include "../include/Menu.h"
#include "../include/log.h"

#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
    // Address sanitizer check (раскомментировать для теста)
    // int arr[4];
    // arr[5] = 12;
    
    try
    {
        AppSettings app_settings(argc, argv);
        DataPool data_pool;
        Menu menu(data_pool, app_settings);
        
        menu.show_menu();
    }
    catch (std::exception& ex)
    {
        console_log(ex);
    }
    
    return 0;
}