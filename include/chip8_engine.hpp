#ifndef CHIP8_ENGINE_HPP_
#define CHIP8_ENGINE_HPP_

#include <chip8_engine_fwd.hpp>
#include <cstddef>
#include <cwchar>
#include <stack>

namespace nznyx::chip8 {
class chip8_engine {
public:
    chip8_engine();

    void change_fontset(const unsigned char fontset[80]) noexcept;

    /* DEBUG FUNCTIONS */
    const unsigned char *get_memory() const noexcept;

    /* MEMORY LAYOUT CONSTANTS */
    constexpr static std::size_t FONT_POSITION = 0x50;
    constexpr static std::size_t PROGRAM_POSITION = 0x200;
    constexpr static std::size_t MEMORY_SIZE = 4096;
    constexpr static std::size_t VARIABLE_REGISTER_SIZE = 16;

private:
    std::stack<unsigned char> stack;
    unsigned char memory_[MEMORY_SIZE] = {0};
    unsigned char V[VARIABLE_REGISTER_SIZE] = {0};
    unsigned short PC = PROGRAM_POSITION;
    unsigned short I = 0;
    unsigned char delay_timer = 0;
    unsigned char sound_timer = 0;
};
}  // namespace nznyx::chip8

#endif  // CHIP8_ENGINE_HPP_
