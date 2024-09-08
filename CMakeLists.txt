cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(chip8-emulator CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)  # Recommended by Boost.DLL's manual

# Add the GoogleTest submodule
add_subdirectory(lib/googletest)

find_package(Boost 1.71 REQUIRED filesystem)  # Required by Boost.DLL
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(include)

add_executable(chip8-emulator src/main.cpp src/chip8_engine.cpp)
target_link_libraries(chip8-emulator PRIVATE gtest gtest_main)

add_executable(chip8-engine-test tests/chip8_engine_test.cpp src/chip8_engine.cpp)
target_link_libraries(chip8-engine-test PRIVATE gtest gtest_main)
target_compile_definitions(chip8-engine-test PRIVATE TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/test_data/")

enable_testing()

add_test(NAME MyClassTest COMMAND MyProjectTests)
