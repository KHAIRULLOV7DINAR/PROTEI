#include <gtest/gtest.h>
#include <sstream>
#include <memory>

#include "../include/Menu.h"
#include "../include/AppSettings.h"
#include "../include/DataPool.h"
#include "../include/Client.h"
#include "../../utils/Logger.h"
#include "../../utils/utils.h"


class MenuWrapper : public Menu
{
public:
    MenuWrapper(Logger& logger, bool& running) : Menu(logger, running) {}

    void set_app_settings(AppSettings& settings)
    {
        app_settings_ = &settings;
    }
    void set_data_pool(DataPool& pool)
    {
        data_pool_ = &pool;
    }
    void set_client(Client& client)
    { 
        client_ = &client;
    }
    
    void execute_name_item() 
    {
        if (app_settings_)
        {
            NameItem item(*app_settings_);
            item.execute();
        }
    }
    
    void execute_type_item()
    {
        if (data_pool_)
        {
            TypeItem item(*data_pool_);
            item.execute();
        }
    }
    
    void execute_vector_item()
    {
        if (data_pool_)
        {
            VectorItem item(*data_pool_);
            item.execute();
        }
    }

private:
    AppSettings* app_settings_ = nullptr;
    DataPool* data_pool_ = nullptr;
    Client* client_ = nullptr;
};

//Фикстура меню для тестов
class MenuTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        char* argv[] = {
            const_cast<char*>("program"), 
            const_cast<char*>("-a"), 
            const_cast<char*>("127.0.0.1"), 
            const_cast<char*>("-p"), 
            const_cast<char*>("1100"), 
            const_cast<char*>("-L"), 
            const_cast<char*>("myLib")
        };
        
        network_address = std::make_unique<NetworkAddress>();
        logger = std::make_unique<Logger>();
        app_settings = std::make_unique<AppSettings>(7, argv, *network_address);
        data_pool = std::make_unique<DataPool>();
        client = std::make_unique<Client>(*network_address);
        running = true;
        menu = std::make_unique<MenuWrapper>(*logger, running);
        
        menu->set_app_settings(*app_settings);
        menu->set_data_pool(*data_pool);
        menu->set_client(*client);
    }

    void TearDown() override
    {
        restore_cin();
    }

    void set_cin_input(const std::string& input)
    {
        restore_cin();
        cin_buffer = std::cin.rdbuf();
        std::cin.rdbuf(input_stream.rdbuf());
        input_stream.str(input);
    }

    void restore_cin()
    {
        if (cin_buffer)
        {
            std::cin.rdbuf(cin_buffer);
            cin_buffer = nullptr;
        }
    }
    
    void input_type()
    {
        menu->execute_type_item();
    }
    
    void input_vector()
    {
        menu->execute_vector_item();
    }
    
    std::unique_ptr<NetworkAddress> network_address;
    std::unique_ptr<Logger> logger;
    std::unique_ptr<AppSettings> app_settings;
    std::unique_ptr<DataPool> data_pool;
    std::unique_ptr<Client> client;
    std::unique_ptr<MenuWrapper> menu;
    bool running;
    
    std::stringstream input_stream;
    std::streambuf* cin_buffer = nullptr;
};

//Тестирование метода преобразования ввода(удаление непечатных символов и приведение команд к нижнему регистру)
TEST_F(MenuTestFixture, EnterCommandTest1)
{
    std::string new_command = "e x \nit";
    fix_input(new_command);
    EXPECT_EQ("exit", new_command);

    new_command = "!@#$%^&*()_+";
    fix_input(new_command);
    EXPECT_EQ("", new_command);

    new_command = "TY  \t!  p E";
    fix_input(new_command);
    EXPECT_EQ("type", new_command);
}

//Тестирование ввода векторов
//int
TEST_F(MenuTestFixture, InputIntVectorValueTest)
{
    //Отсутсвие типа
    set_cin_input("1 2 1 4\n");
    EXPECT_THROW(input_vector(), std::logic_error);
    restore_cin();

    set_cin_input("int\n");
    EXPECT_NO_THROW(input_type());
    restore_cin();

    //Правильный ввод
    set_cin_input("  1  2 3         4\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();

    set_cin_input("0 -234 12 -54235\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();

    //Неправльный ввод
    //Ввод символов
    set_cin_input("1 2 sdsdaf 4\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    set_cin_input("1 2 3 4a4\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    //Ввод не 4-х мерного вектора
    set_cin_input("1 2 4\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    set_cin_input("1 2 3 4 5\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    //Неправильны тип данных
    set_cin_input("1.23 2 3 4\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    set_cin_input("1 2 -3.0 4\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();
}

//float
TEST_F(MenuTestFixture, InputFloatVectorValueTest)
{
    //Отсутсвие типа
    set_cin_input("1.0 2.0 1.0 4.0\n");
    EXPECT_THROW(input_vector(), std::logic_error);
    restore_cin();

    set_cin_input("float\n");
    EXPECT_NO_THROW(input_type());
    restore_cin();

    //Правильный ввод
    set_cin_input("  1.0  2.0 3.0         4.0\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();

    set_cin_input("0.0001 -234.023 12 -54235.1234\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();

    //Неправльный ввод
    //Ввод символов
    set_cin_input("1.0123 2.23 sdsdaf 4.2\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    set_cin_input("1.232 -2.123 3.234 4.2a4\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    //Ввод не 4-х мерного вектора
    set_cin_input("1.234 -2.234 4.0\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    set_cin_input("1.234 2.124 3.2 4.0 5\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    //Неправильны тип данных (не будет ошибки из-за неявного преобразования инта в флоат)
    set_cin_input("1 2 3 4\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();

    set_cin_input("1 2 -3.0 4\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();
}

//double
TEST_F(MenuTestFixture, InputDoubleVectorValueTest)
{
    //Отсутсвие типа
    set_cin_input("1.0 2.0 1.0 4.0\n");
    EXPECT_THROW(input_vector(), std::logic_error);
    restore_cin();

    set_cin_input("double\n");
    EXPECT_NO_THROW(input_type());
    restore_cin();

    //Правильный ввод
    set_cin_input("  1.0  2.0 3.0         4.0\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();

    set_cin_input("0.0001 -234.023 12 -54235.1234\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();

    //Неправльный ввод
    //Ввод символов
    set_cin_input("1.0123 2.23 sdsdaf 4.2\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    set_cin_input("1.232 -2.123 3.234 4.2a4\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    //Ввод не 4-х мерного вектора
    set_cin_input("1.234 -2.234 4.0\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    set_cin_input("1.234 2.124 3.2 4.0 5\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    //Неправильны тип данных (не будет ошибки из-за неявного преобразования инта в дабл)
    set_cin_input("1 2 3 4\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();

    set_cin_input("1 2 -3.0 4\n");
    EXPECT_NO_THROW(input_vector());
    restore_cin();
}

//Тестирование проверки w-компоненты вектора
TEST_F(MenuTestFixture, InputVectorWTest)
{   
    //int
    set_cin_input("int\n");
    EXPECT_NO_THROW(input_type());
    restore_cin();

    set_cin_input("1 2 3 0\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    //float
    set_cin_input("float\n");
    EXPECT_NO_THROW(input_type());
    restore_cin();

    set_cin_input("1.1 2.2 3.3 0.0\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();

    //double
    set_cin_input("double\n");
    EXPECT_NO_THROW(input_type());
    restore_cin();

    set_cin_input("1.1 2.2 3.3 0.0000\n");
    EXPECT_THROW(input_vector(), std::invalid_argument);
    restore_cin();
}