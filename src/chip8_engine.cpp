#include <strings.h>
#include <algorithm>
#include <cassert>
#include <chip8_engine.hpp>
#include <chip8_engine_fwd.hpp>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <fstream>
#include <optional>
#include <random>
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
    if (!last_key_.has_value()) {
        last_key_ = key;
    }
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
    std::uint8_t nibble = opcode & 0xF;
    std::uint8_t byte = opcode & 0xFF;
    std::uint16_t address = opcode & 0xFFF;
    std::uint8_t &VX = variable_registers_[x];
    std::uint8_t &VY = variable_registers_[y];
    std::uint8_t &VF = variable_registers_[0xF];

    switch (type) {
        case 0x0: {
            switch (byte) {
                case 0xE0: {
                    clear_screen_();
                } break;
                case 0xEE: {
                    program_counter_ = stack_[--stack_pointer_];
                } break;
                default:
                    assert(false);
            }
        } break;
        case 0x1: {
            program_counter_ = address;
        } break;
        case 0x2: {
            stack_[stack_pointer_++] = program_counter_;
            program_counter_ = address;
        } break;
        case 0x3: {
            if (VX == byte) {
                program_counter_ += 2;
            }
        } break;
        case 0x4: {
            if (VX != byte) {
                program_counter_ += 2;
            }
        } break;
        case 0x5: {
            assert(nibble == 0);
            if (VX == VY) {
                program_counter_ += 2;
            }
        } break;
        case 0x6: {
            VX = byte;
        } break;
        case 0x7: {
            VX += byte;
        } break;
        case 0x8: {
            switch (nibble) {
                case 0x0: {
                    VX = VY;
                } break;
                case 0x1: {
                    VX |= VY;
                } break;
                case 0x2: {
                    VX &= VY;
                } break;
                case 0x3: {
                    VX ^= VY;
                } break;
                case 0x4: {
                    std::uint8_t snapshot = VX;
                    VX += VY;
                    VF = VX < snapshot;
                } break;
                case 0x5: {
                    VF = VX < VY;
                    VX -= VY;
                } break;
                case 0x6: {
                    VF = VX & 1;
                    VX >>= 1;
                } break;
                case 0x7: {
                    VF = VY < VX;
                    VX = VY - VY;
                } break;
                case 0xE: {
                    VF = VX >> 7;
                    VX <<= 1;
                } break;
                default:
                    assert(false);
            }
        } break;
        case 0x9: {
            assert(nibble == 0);
            if (VX != VY) {
                program_counter_ += 2;
            }
        } break;
        case 0xA: {
            index_register_ = address;
        } break;
        case 0xB: {
            program_counter_ = address + variable_registers_[0x0];
        } break;
        case 0xC: {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            VX = gen() & byte;
        } break;
        case 0xD: {
            VF = draw_(VX & 63, VY & 31, memory_ + index_register_, nibble);
        } break;
        case 0xE: {
            switch (byte) {
                case 0x9E: {
                    if (is_pressed(static_cast<key>(VX))) {
                        program_counter_ += 2;
                    }
                } break;
                case 0xA1: {
                    if (!is_pressed(static_cast<key>(VX))) {
                        program_counter_ += 2;
                    }
                } break;
                default:
                    assert(false);
            }
        } break;
        case 0xF: {
            switch (byte) {
                case 0x07: {
                    VX = delay_timer_;
                } break;
                case 0x0A: {
                    if (last_key_.has_value()) {
                        VX = static_cast<int>(last_key_.value());
                    } else {
                        program_counter_ -= 2;
                    }
                } break;
                case 0x15: {
                    delay_timer_ = VX;
                } break;
                case 0x18: {
                    sound_timer_ = VX;
                } break;
                case 0x1E: {
                    index_register_ += VX;
                } break;
                case 0x29: {
                    index_register_ = FONT_POSITION + 5 * VX;
                } break;
                case 0x33: {
                    memory_[index_register_] = VX / 100;
                    memory_[index_register_ + 1] = (VX % 100) / 10;
                    memory_[index_register_ + 2] = VX % 10;
                } break;
                case 0x55: {
                    for (std::size_t i = 0; i <= x; ++i) {
                        memory_[index_register_ + i] = variable_registers_[i];
                    }
                } break;
                case 0x65: {
                    for (std::size_t i = 0; i <= x; ++i) {
                        variable_registers_[i] = memory_[index_register_ + i];
                    }
                } break;
                default:
                    assert(false);
            }
        } break;
        default:
            assert(false);
    }

    last_key_ = std::nullopt;
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
