#include <gtest/gtest.h>
#include <sstream>
#include <memory>

#include "../include/Menu.h"
#include "../include/AppSettings.h"
#include "../include/DataPool.h"
#include "../include/log.h"


class MenuWrapper : public Menu
{
public:
    using Menu::Menu;

    using Menu::input_type;
    using Menu::input_vector;
};

class MenuTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        char* argv[] = {const_cast<char*>("program"), const_cast<char*>("-a"), const_cast<char*>("127.127.127.127"), const_cast<char*>("-L"), const_cast<char*>("myLib")};
        app_settings = std::make_unique<AppSettings>(5, argv);
        data_pool = std::make_unique<DataPool>();
        menu = std::make_unique<MenuWrapper>(*data_pool, *app_settings);
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
    std::unique_ptr<MenuWrapper> menu;
    
    std::stringstream input_stream;
    std::streambuf* cin_buffer = nullptr;
};


TEST_F(MenuTestFixture, EnterCommandTest)
{
    std::string new_command = "e x \nit";

    menu->parse_command(new_command);

    EXPECT_EQ("exit", new_command);
}

TEST_F(MenuTestFixture, InputVectorValueTest)
{
    set_cin_input("int\n");
    EXPECT_NO_THROW(menu->input_type());
    restore_cin(); 

    set_cin_input("1 2 sdsdaf 4\n");
    EXPECT_THROW(menu->input_vector(), std::invalid_argument);
    restore_cin();
}

TEST_F(MenuTestFixture, InputVectorWTest)
{
    set_cin_input("float\n");
    EXPECT_NO_THROW(menu->input_type());
    restore_cin();

    set_cin_input("1.1 2.2 3.3 0.0\n");
    EXPECT_THROW(menu->input_vector(), std::invalid_argument);
    restore_cin();
}