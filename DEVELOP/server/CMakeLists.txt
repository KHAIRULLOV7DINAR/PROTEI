cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HOME_TASK_SERVER)

#поиск nlohmann-json3
find_package(nlohmann_json REQUIRED)

include_directories("./include")

set(SOURCE_LIST
    src/main.cpp
    src/Server.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

#подключение nlohmann-json3
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)