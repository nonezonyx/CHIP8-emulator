add_executable(chip8-emulator main.cpp chip8_engine.cpp)

# Link against libraries
target_link_libraries(chip8-emulator PRIVATE Boost::filesystem Boost::system ${SDL2_LIBRARIES})
