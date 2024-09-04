#include <gtest/gtest.h>
#include <sys/stat.h>
#include <chip8_engine.hpp>
#include <cstring>
#include <numeric>
#include "chip8_engine_fwd.hpp"

using namespace nznyx::chip8;

TEST(chip8_engine_test, constants) {
    EXPECT_EQ(chip8_engine::FONT_POSITION, 0x50);
    EXPECT_EQ(chip8_engine::PROGRAM_POSITION, 0x200);
    EXPECT_EQ(chip8_engine::MEMORY_SIZE, 4096);
    EXPECT_EQ(chip8_engine::VARIABLE_REGISTER_SIZE, 16);
}

TEST(chip8_engine_test, change_font) {
    chip8_engine chip8{};
    EXPECT_FALSE(memcmp(chip8.get_memory() + 0x50, chip8_default_fontset, 80));

    unsigned char new_font[80];
    std::iota(new_font, new_font + 80, 0);
    chip8.change_fontset(new_font);
    EXPECT_FALSE(memcmp(chip8.get_memory() + 0x50, new_font, 80));
}

TEST(chip8_engine_test, load_from_pointer) {
}
