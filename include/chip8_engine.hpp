#ifndef CHIP8_ENGINE_HPP_
#define CHIP8_ENGINE_HPP_

#include <chip8_engine_fwd.hpp>
#include <cstddef>
#include <cwchar>
#include <stack>

namespace nznyx::chip8 {
class chip8_engine {
public:
    constexpr static std::size_t FONT_POSITION = 0x050;
    constexpr static std::size_t PROGRAM_POSITION = 0x200;

    void change_fontset(const unsigned char fontset[80]) noexcept;

private:
    std::stack<unsigned char> stack;
    unsigned char memory_[4096] = {0};
    unsigned char V[16] = {0};
    unsigned short PC = PROGRAM_POSITION;
    unsigned short I = 0;
    unsigned char delay_timer = 0;
    unsigned char sound_timer = 0;
};
}  // namespace nznyx::chip8

#endif  // CHIP8_ENGINE_HPP_
