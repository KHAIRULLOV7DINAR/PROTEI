#include <gtest/gtest.h>
#include <sstream>
#include <memory>

#include "../include/Menu.h"
#include "../include/AppSettings.h"
#include "../include/DataPool.h"
#include "../include/log.h"


class MenuTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        char* argv[] = {const_cast<char*>("program"), const_cast<char*>("-a"), const_cast<char*>("127.127.127.127"), const_cast<char*>("-L"), const_cast<char*>("myLib")};
        app_settings = std::make_unique<AppSettings>(5, argv);
        data_pool = std::make_unique<DataPool>();
        menu = std::make_unique<Menu>(*data_pool, *app_settings);
    }

    void TearDown() override{}

    void set_cin_input(const std::string& input)
    {
        cin_buffer = std::cin.rdbuf();
        std::cin.rdbuf(input_stream.rdbuf());
        input_stream.str(input);
    }

    void restore_cin()
    {
        std::cin.rdbuf(cin_buffer);
    }

    std::unique_ptr<AppSettings> app_settings;
    std::unique_ptr<DataPool> data_pool;
    std::unique_ptr<Menu> menu;
    
    std::stringstream input_stream;
    std::streambuf* cin_buffer = nullptr;
};


TEST_F(MenuTestFixture, EnterCommandTest)
{
    std::string new_command = "e x \nit";

    menu->parse_command(new_command);

    EXPECT_EQ("exit", new_command);
}