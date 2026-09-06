CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(RTS)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

OPTION(RTS_TEST "Build RTS Tests" OFF)

# Apple specific
IF(APPLE)
    LIST(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
ENDIF()

# Raylib
FIND_PACKAGE(raylib CONFIG REQUIRED)

# Main executable
ADD_EXECUTABLE(main "./main.cpp")
TARGET_INCLUDE_DIRECTORIES(main PRIVATE "./include")

# Grid library
ADD_LIBRARY(grid STATIC "src/utils/grid.cpp")
TARGET_INCLUDE_DIRECTORIES(grid PRIVATE "./include")

# Slot Map/ Other Data Structures (MAY CHANGE DEPENDING ON IF WE NEED MORE)
ADD_LIBRARY(slotmap STATIC "src/engine/ui.cpp")
TARGET_INCLUDE_DIRECTORIES(slotmap PRIVATE "./include")

# Entity Library
ADD_LIBRARY(entities STATIC "src/engine/entities.cpp")
TARGET_INCLUDE_DIRECTORIES(entities PRIVATE "./include")

# Engine Library
ADD_LIBRARY(engine STATIC "src/engine/engine.cpp")
TARGET_INCLUDE_DIRECTORIES(engine PRIVATE "./include")

ADD_LIBRARY(ui STATIC "src/engine/ui.cpp")
TARGET_INCLUDE_DIRECTORIES(ui PRIVATE "./include")

# Link libraries
TARGET_LINK_LIBRARIES(grid PRIVATE raylib)
TARGET_LINK_LIBRARIES(engine PUBLIC slotmap)
TARGET_LINK_LIBRARIES(entities PUBLIC grid)
TARGET_LINK_LIBRARIES(ui PRIVATE raylib)
TARGET_LINK_LIBRARIES(grid PUBLIC entities)
TARGET_LINK_LIBRARIES(main PRIVATE grid engine ui)


#Testing Suite
IF(RTS_TEST)
	FIND_PACKAGE(GTest CONFIG REQUIRED)

	ADD_EXECUTABLE(tests "./test/test.cpp")
	TARGET_INCLUDE_DIRECTORIES(tests PRIVATE "./include")
	TARGET_INCLUDE_DIRECTORIES(tests PRIVATE "./test")
	TARGET_LINK_LIBRARIES(tests PRIVATE GTest::gtest_main slotmap)
ENDIF()
