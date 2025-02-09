cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(chip8-emulator CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)  # Recommended by Boost.DLL's manual

# Add the GoogleTest submodule
add_subdirectory(lib/googletest)

cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include)

add_executable(chip8-emulator src/main.cpp src/chip8_engine.cpp)
target_link_libraries(chip8-emulator PRIVATE gtest gtest_main Boost::filesystem Boost::system )

add_executable(chip8-engine-test tests/chip8_engine_test.cpp src/chip8_engine.cpp)
target_link_libraries(chip8-engine-test PRIVATE gtest gtest_main)
target_compile_definitions(chip8-engine-test PRIVATE TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/test_data/")

add_library(sdl2_view SHARED src/sdl2_view.cpp)
target_include_directories(sdl2_view PRIVATE include ${SDL2_INCLUDE_DIRS})
target_link_libraries(sdl2_view PRIVATE Boost::filesystem Boost::system ${SDL2_LIBRARIES})
set_target_properties(sdl2_view PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

enable_testing()

add_test(NAME MyClassTest COMMAND MyProjectTests)
