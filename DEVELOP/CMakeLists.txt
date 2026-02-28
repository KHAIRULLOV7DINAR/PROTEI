cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HOMETASK_1)

include_directories("./include")

set(SOURCE_LIST
    src/main.cpp
    src/console_menu.cpp
    src/log.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
