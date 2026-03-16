CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(RTS)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
OPTION(RTS_BUILD_TESTS "Build RTS Tests" OFF)

# Apple specific
IF(APPLE)
    LIST(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
ENDIF()

# Raylib
FIND_PACKAGE(raylib CONFIG REQUIRED)

# Main executable
ADD_EXECUTABLE(main "./main.c")
SET_PROPERTY(TARGET main PROPERTY C_STANDARD 23)
TARGET_INCLUDE_DIRECTORIES(main PRIVATE "./include/")

# Test executable
IF(RTS_BUILD_TESTS)
    ADD_EXECUTABLE(testArrayList "src/tests/testArrayList.c")
    SET_PROPERTY(TARGET testArrayList PROPERTY C_STANDARD 23)
    TARGET_INCLUDE_DIRECTORIES(testArrayList PRIVATE "./include")
    TARGET_LINK_LIBRARIES(testArrayList PRIVATE arraylist)
ENDIF()

# Grid library
ADD_LIBRARY(grid STATIC "src/utils/grid.c")
SET_PROPERTY(TARGET grid PROPERTY C_STANDARD 23)
TARGET_INCLUDE_DIRECTORIES(grid PRIVATE "./include")

# Arraylist library
ADD_LIBRARY(arraylist STATIC "src/utils/arraylist.c")
SET_PROPERTY(TARGET arraylist PROPERTY C_STANDARD 23)
TARGET_INCLUDE_DIRECTORIES(arraylist PRIVATE "include/utils/")

#Player library
ADD_LIBRARY(player STATIC "src/utils/player.c")
SET_PROPERTY(TARGET player PROPERTY C_STANDARD 23)
TARGET_INCLUDE_DIRECTORIES(player PRIVATE "include/utils/")

#Unit library
# ADD_LIBRARY(unit STATIC "src/utils/unit.c")
# SET_PROPERTY(TARGET unit PROPERTY C_STANDARD 23)
# TARGET_INCLUDE_DIRECTORIES(unit PRIVATE "include/utils/unit.h")


# Link libraries
TARGET_LINK_LIBRARIES(grid PUBLIC raylib)
TARGET_LINK_LIBRARIES(main PRIVATE grid arraylist player)
