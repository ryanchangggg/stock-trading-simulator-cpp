cmake_minimum_required(VERSION 3.14)
project(stock-trading-simulator-cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources
file(GLOB_RECURSE SOURCES src/*.cpp)

# Include directories
include_directories(include)

# Main executable
add_executable(stock_trading_simulator ${SOURCES})

# Tests (optional, requires Google Test)
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
    add_executable(stock_trading_simulator_test ${TEST_SOURCES} ${SOURCES})
    target_include_directories(stock_trading_simulator_test PRIVATE tests)
    target_link_libraries(stock_trading_simulator_test GTest::GTest GTest::Main)
    add_test(NAME stock_trading_simulator_test COMMAND stock_trading_simulator_test)
endif()
