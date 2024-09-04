#ifndef CHIP8_ENGINE_HPP_
#define CHIP8_ENGINE_HPP_

#include <chip8_engine_fwd.hpp>
#include <cstddef>
#include <functional>
#include <string>

namespace nznyx::chip8 {
class chip8_engine {
public:
    chip8_engine();

    void change_fontset(const unsigned char fontset[80]) noexcept;
    void reset() noexcept;
    void load(const unsigned char *rom, std::size_t size) noexcept;
    void load(const std::string &filename);

    bool is_pressed(key key) noexcept;
    void set_pressed(key key, bool state) noexcept;
    void execute(std::uint16_t instruction);

    /* DEBUG FUNCTIONS */
    const unsigned char *get_memory() const noexcept;

    /* MEMORY LAYOUT CONSTANTS */
    constexpr static std::size_t STACK_SIZE = 16;
    constexpr static std::size_t FONT_POSITION = 0x50;
    constexpr static std::size_t PROGRAM_POSITION = 0x200;
    constexpr static std::size_t MEMORY_SIZE = 4096;
    constexpr static std::size_t VARIABLE_REGISTER_SIZE = 16;

private:
    std::function<void()> clear_screen_;
    std::function<void(int, int)> draw_pixel_;
    std::function<void(int, int, const unsigned char *, int)> draw_sprite_;
    unsigned short stack_[STACK_SIZE] = {0};
    unsigned char memory_[MEMORY_SIZE] = {0};
    unsigned char variable_registers_[VARIABLE_REGISTER_SIZE] = {0};
    unsigned short keys_pressed_ = 0;  // binary 1 if pressed
    unsigned short program_counter_ = PROGRAM_POSITION;
    unsigned short index_register_ = 0;
    unsigned char stack_pointer_ = 0;
    unsigned char delay_timer = 0;
    unsigned char sound_timer = 0;
};
}  // namespace nznyx::chip8

#endif  // CHIP8_ENGINE_HPP_
