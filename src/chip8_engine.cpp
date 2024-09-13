#include <strings.h>
#include <algorithm>
#include <cassert>
#include <chip8_engine.hpp>
#include <chip8_engine_fwd.hpp>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <fstream>
#include <utility>

namespace nznyx::chip8 {

chip8_engine::chip8_engine(
    const std::function<void()> &clear_screen,
    const std::function<bool(int, int, const std::uint8_t *, int)> &draw,
    std::size_t opcodes_per_sec
) noexcept
    : clear_screen_(clear_screen),
      draw_(draw),
      opcodes_per_hertz_(opcodes_per_sec / 60) {
    assert(opcodes_per_sec > 0);
    change_fontset(chip8_default_fontset);
}

void chip8_engine::change_fontset(const std::uint8_t fontset[80]) noexcept {
    std::memcpy(memory_ + FONT_POSITION, fontset, 80);
}

void chip8_engine::reset() noexcept {
    change_fontset(chip8_default_fontset);
    std::fill(
        variable_registers_, variable_registers_ + VARIABLE_REGISTER_SIZE, 0
    );
    keys_pressed_ = 0;
    program_counter_ = PROGRAM_POSITION;
    index_register_ = 0;
    stack_pointer_ = 0;
    delay_timer_ = 0;
    sound_timer_ = 0;
}

void chip8_engine::load(const std::uint8_t *rom, std::size_t size) noexcept {
    std::memcpy(memory_ + PROGRAM_POSITION, rom, size);
}

void chip8_engine::load(const std::string &filename) {
    std::ifstream file(filename, std::ios_base::binary);
    file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(memory_ + PROGRAM_POSITION), file_size);
    file.close();
}

bool chip8_engine::is_pressed(key key) noexcept {
    return (keys_pressed_ >> std::to_underlying(key)) & 1;
}

void chip8_engine::set_pressed(key key, bool state) noexcept {
    keys_pressed_ &= ~((state ? 0 : 1) << std::to_underlying(key));
}

void chip8_engine::execute(std::uint16_t opcode) {
    ++opcodes_since_timer_decrementing_;
    if (opcodes_since_timer_decrementing_ == opcodes_per_hertz_) {
        opcodes_since_timer_decrementing_ = 0;
        delay_timer_ = delay_timer_ == 0 ? 0 : delay_timer_ - 1;
        sound_timer_ = sound_timer_ == 0 ? 0 : sound_timer_ - 1;
    }
    std::uint8_t type = opcode >> 12;
    std::uint8_t x = (opcode >> 8) & 0xF;
    std::uint8_t y = (opcode >> 4) & 0xF;
    std::uint8_t n = opcode & 0xF;
    std::uint8_t nn = opcode & 0xFF;
    std::uint16_t nnn = opcode & 0xFFF;
    switch (type) {
        case 0x0: {
            clear_screen_();
        } break;
        case 0x1: {
            program_counter_ = nnn;
        } break;
        case 0x6: {
            variable_registers_[x] = nn;
        } break;
        case 0x7: {
            variable_registers_[x] += nn;
        } break;
        case 0xA: {
            index_register_ = nnn;
        } break;
        case 0xD: {
            variable_registers_[0xF] = 0;
            variable_registers_[0xF] = draw_(
                variable_registers_[x] & 63, variable_registers_[y] & 31,
                memory_ + index_register_, n
            );
        } break;
        default:
            assert(false);
    }
}

void chip8_engine::execute() {
    std::uint8_t byte1 = memory_[program_counter_++];
    std::uint8_t byte2 = memory_[program_counter_++];
    std::uint16_t opcode = static_cast<std::uint16_t>(byte1) << 8 | byte2;
    execute(opcode);
}

const std::uint8_t *chip8_engine::get_memory() const noexcept {
    return memory_;
}

}  // namespace nznyx::chip8
