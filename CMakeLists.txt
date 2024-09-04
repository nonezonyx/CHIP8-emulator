cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(chip8-emulator CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)  # Recommended by Boost.DLL's manual

find_package(Boost 1.71 REQUIRED filesystem)  # Required by Boost.DLL
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(include)

add_executable(chip8-emulator src/main.cpp src/chip8_engine.cpp)
