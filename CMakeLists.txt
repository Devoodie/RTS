cmake_minimum_required(VERSION 3.10)
project(RTS LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RTS_BUILD_TESTS "Build RTS Tests" OFF)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

find_package(raylib CONFIG REQUIRED)

add_library(grid STATIC src/utils/grid.cpp)
target_include_directories(grid PUBLIC include)
target_link_libraries(grid PUBLIC raylib)

add_library(entities STATIC src/engine/entities.cpp)
target_include_directories(entities PUBLIC include)
target_link_libraries(entities PUBLIC grid)

add_library(engine STATIC src/engine/engine.cpp)
target_include_directories(engine PUBLIC include)
target_link_libraries(engine PUBLIC entities grid)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE engine)

if(RTS_BUILD_TESTS)
    find_package(GTest CONFIG REQUIRED)
    add_executable(tests
            src/tests/astartest.cpp)
    target_link_libraries(tests PRIVATE
            engine
            GTest::gtest
            GTest::gtest_main
    )
endif()