#include <gtest/gtest.h>

#include "../include/console_menu.h"
#include "../include/log.h"
#include "../include/config.hpp"

//Ввод непечатных символов
TEST(MenuTest, CommandInput)
{
    std::string new_command;

    new_command = "eX It\n";
    parse_command(new_command);
    EXPECT_EQ(new_command, "exit");

    new_command = "M3NU!\t";
    parse_command(new_command);
    EXPECT_EQ(new_command, "m3nu");

    new_command = " \n\t!@#$%^";
    parse_command(new_command);
    EXPECT_EQ(new_command, "");
}

//Ввод вектора
//Проверка используемой функции, которая добавляет лишь одно значение вектора, преобразованного из str к int/float/double
TEST(MenuTest, VectorInputTest)
{
    Config config;
    std::string str_num;

    //int
    config.type = "int";
    config.createTypedVector(config.type);

    str_num = "qwerty";
    EXPECT_THROW(vector_push(str_num, config, 0), std::invalid_argument);
    config.vect->clear();

    str_num = "12a4";
    EXPECT_THROW(vector_push(str_num, config, 0), std::invalid_argument);
    config.vect->clear();

    str_num = "-123";
    EXPECT_NO_THROW(vector_push(str_num, config, 0));
    config.vect->clear();

    //float
    config.type = "float";
    config.createTypedVector(config.type);

    str_num = "qwerty";
    EXPECT_THROW(vector_push(str_num, config, 0), std::invalid_argument);
    config.vect->clear();

    str_num = "132.as44";
    EXPECT_THROW(vector_push(str_num, config, 0), std::invalid_argument);
    config.vect->clear();

    str_num = "-25.3454";
    EXPECT_NO_THROW(vector_push(str_num, config, 0));
    config.vect->clear();
}

//W аргумент вектора
TEST(MenuTest, VectorWArgument)
{
    Config config;
    std::string str_num;

    //float
    config.type = "float";
    config.createTypedVector(config.type);

    std::vector<std::string> nums = {
        "1.3",
        "2.23",
        "3.4",
        "0.0"
    };
    for(int i = 0; i < 3; i ++)
    {
        EXPECT_NO_THROW(vector_push(nums[i], config, i));
    }

    EXPECT_THROW(vector_push(nums[3], config, 3), std::invalid_argument);

}