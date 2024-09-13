#ifndef CHIP8_ENGINE_HPP_
#define CHIP8_ENGINE_HPP_

#include <chip8_engine_fwd.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>

namespace nznyx::chip8 {
class chip8_engine {
public:
    chip8_engine(
        const std::function<void()> &clear_screen,
        const std::function<bool(int, int, const std::uint8_t *, int)> &draw,
        std::size_t opcodes_per_sec = 700
    ) noexcept;

    void change_fontset(const std::uint8_t fontset[80]) noexcept;
    void reset() noexcept;
    void load(const std::uint8_t *rom, std::size_t size) noexcept;
    void load(const std::string &filename);

    bool is_pressed(key key) noexcept;
    void set_pressed(key key, bool state) noexcept;
    void execute(std::uint16_t opcode);
    void execute();

    /* DEBUG FUNCTIONS */
    const std::uint8_t *get_memory() const noexcept;

    /* MEMORY LAYOUT CONSTANTS */
    constexpr static std::size_t STACK_SIZE = 16;
    constexpr static std::size_t FONT_POSITION = 0x50;
    constexpr static std::size_t PROGRAM_POSITION = 0x200;
    constexpr static std::size_t MEMORY_SIZE = 4096;
    constexpr static std::size_t VARIABLE_REGISTER_SIZE = 16;

private:
    std::function<void()> clear_screen_;
    std::function<bool(int, int, const std::uint8_t *, int)> draw_;
    std::size_t opcodes_per_hertz_ = 0;
    std::size_t opcodes_since_timer_decrementing_ = 0;

    /* CHIP8 memory */
    std::uint16_t stack_[STACK_SIZE] = {};
    std::uint8_t memory_[MEMORY_SIZE] = {};
    std::uint8_t variable_registers_[VARIABLE_REGISTER_SIZE] = {};
    std::uint16_t keys_pressed_ = 0;  // binary 1 if pressed
    std::uint16_t program_counter_ = PROGRAM_POSITION;
    std::uint16_t index_register_ = 0;
    std::uint8_t stack_pointer_ = 0;
    std::uint8_t delay_timer_ = 0;
    std::uint8_t sound_timer_ = 0;
};
}  // namespace nznyx::chip8

#endif  // CHIP8_ENGINE_HPP_
