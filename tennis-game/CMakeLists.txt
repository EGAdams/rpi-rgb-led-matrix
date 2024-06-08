cmake_minimum_required(VERSION 3.10)

# Project name
project(tennis-game)

# Enable testing
enable_testing()

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/Mode1Score)
include_directories( "/home/adamsl/rpi-rgb-led-matrix/tennis-game/GameState" )

# Add the main source file
add_executable(Mode1Score Mode1Score/Mode1Score.cpp)

# Add the Google Test framework
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add the test executable
add_executable(runTests Mode1Score/Mode1ScoreTest.cpp)

# Link the test executable against Google Test and the main library
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

# Add the test to CMake's test suite
add_test(NAME runTests COMMAND runTests)
