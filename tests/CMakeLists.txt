# Add the GoogleTest submodule
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/googletest ${CMAKE_BINARY_DIR}/googletest)

# Build the test executable
add_executable(chip8-engine-test chip8_engine_test.cpp ${CMAKE_SOURCE_DIR}/src/emulator/chip8_engine.cpp)

target_link_libraries(chip8-engine-test PRIVATE gtest gtest_main Boost::filesystem Boost::system)

# Define a compile-time constant with the path to the test data directory
target_compile_definitions(chip8-engine-test PRIVATE TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/test_data/")

# Add the test executable as a test to CMake
add_test(NAME Chip8EngineTest COMMAND chip8-engine-test)
