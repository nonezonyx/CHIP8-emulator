#include <gtest/gtest.h>
#include <sys/stat.h>
#include <chip8_engine.hpp>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ostream>
#include <sstream>
#include "chip8_engine_fwd.hpp"

using namespace nznyx::chip8;

struct debug_screen {
    bool screen[64 * 32] = {0};

    void clear_screen() {
        std::fill(screen, screen + 64 * 32, false);
    }

    bool draw(int x, int y, const std::uint8_t *sprite, int n) {
        bool changed = false;
        int line = 0;
        while (y < 32 && line < n) {
            for (int i = 0; x + i < 64 && i < 8; ++i) {
                bool &pixel = screen[64 * y + x + i];
                bool snapshot = pixel;
                pixel ^= sprite[line] >> (7 - i) & 1;
                changed |= snapshot != pixel;
            }
            ++line;
            ++y;
        }
        return changed;
    }

    friend std::ostream &
    operator<<(std::ostream &out, const debug_screen screen) {
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 64; ++x) {
                out << (screen.screen[64 * y + x] ? "##" : "  ");
            }
            out << '\n';
        }
        return out;
    }
};

static debug_screen screen{};
static auto bound_clear = std::bind(&debug_screen::clear_screen, &screen);
static auto bound_draw = [](int x, int y, const std::uint8_t *sprite, int n) {
    return screen.draw(x, y, sprite, n);
};

TEST(chip8_engine_test, constants) {
    EXPECT_EQ(chip8_engine::FONT_POSITION, 0x50);
    EXPECT_EQ(chip8_engine::PROGRAM_POSITION, 0x200);
    EXPECT_EQ(chip8_engine::MEMORY_SIZE, 4096);
    EXPECT_EQ(chip8_engine::VARIABLE_REGISTER_SIZE, 16);
}

TEST(chip8_engine_test, change_font) {
    chip8_engine chip8{nullptr, nullptr};
    EXPECT_FALSE(memcmp(chip8.get_memory() + 0x50, chip8_default_fontset, 80));

    std::uint8_t new_font[80];
    std::iota(new_font, new_font + 80, 0);
    chip8.change_fontset(new_font);
    EXPECT_FALSE(memcmp(chip8.get_memory() + 0x50, new_font, 80));
}

TEST(chip8_engine_test, load_from_file_IBM_logo) {
    screen.clear_screen();
    chip8_engine chip8{bound_clear, bound_draw};
    chip8.load(TEST_DATA_DIR "IBM_Logo.ch8");
    for (int i = 0; i < 100; ++i) {
        chip8.execute();
    }

    std::stringstream ss;
    ss << screen << '\n';

    std::fstream expected_file(TEST_DATA_DIR "IBM_Logo.txt");
    std::stringstream expected_ss;
    expected_ss << expected_file.rdbuf();

    EXPECT_EQ(ss.str(), expected_ss.str());
}

TEST(chip8_engine_test, opcode_corax89) {
    screen.clear_screen();
    chip8_engine chip8{bound_clear, bound_draw};
    chip8.load(TEST_DATA_DIR "test_opcode.ch8");
    for (int i = 0; i < 10000; ++i) {
        chip8.execute();
    }

    std::stringstream ss;
    ss << screen << '\n';

    std::fstream expected_file(TEST_DATA_DIR "test_opcode.txt");
    std::stringstream expected_ss;
    expected_ss << expected_file.rdbuf();

    EXPECT_EQ(ss.str(), expected_ss.str());
}
