cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Tests" OFF)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

project(HOME_TASK)

include_directories("./include")

set(SOURCE_LIST
    src/main.cpp
    src/log.cpp
    src/AppSettings.cpp
    src/DataPool.cpp
    src/Menu.cpp)

#cppcheck
#include(cmake/cppcheck.cmake)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

if(BUILD_TESTS)
    enable_testing()

    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.x)

    FetchContent_MakeAvailable(googletest)

    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_subdirectory(tests)
endif()