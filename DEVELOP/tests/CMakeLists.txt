cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(run_tests
    test_menu.cpp
    ../src/console_menu.cpp
    ../src/log.cpp)

target_link_libraries(run_tests
    gtest
    gtest_main 
)

include(GoogleTest)
gtest_discover_tests(run_tests)