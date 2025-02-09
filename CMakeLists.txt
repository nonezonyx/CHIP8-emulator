cmake_minimum_required(VERSION 3.22)

# Global settings
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)  # Recommended by Boost.DLL's manual

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

project(chip8-emulator CXX)

# Options to enable/disable building different components
option(BUILD_CHIP8_EMULATOR "Build the Chip8 Emulator executable" ON)
option(BUILD_CHIP8_VIEW_DLL "Build the SDL2 view DLL" ON)
option(BUILD_CHIP8_TESTS "Build the Chip8 Engine tests" OFF)

# Locate dependencies
cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(SDL2 REQUIRED)

# Add directories for include files
include_directories(include ${SDL2_INCLUDE_DIRS})

# Add subdirectories for optional components
if (BUILD_CHIP8_EMULATOR)
    add_subdirectory(src/emulator)
endif()

if (BUILD_CHIP8_VIEW_DLL)
    add_subdirectory(src/view)
endif()

if (BUILD_CHIP8_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
