#include <algorithm>
#include <cassert>
#include <chip8_engine.hpp>
#include <chip8_engine_fwd.hpp>
#include <cstring>
#include <fstream>
#include <utility>

namespace nznyx::chip8 {

chip8_engine::chip8_engine() {
    change_fontset(chip8_default_fontset);
}

void chip8_engine::change_fontset(const unsigned char fontset[80]) noexcept {
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
    delay_timer = 0;
    sound_timer = 0;
}

void chip8_engine::load(const unsigned char *rom, std::size_t size) noexcept {
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

const unsigned char *chip8_engine::get_memory() const noexcept {
    return memory_;
}

}  // namespace nznyx::chip8
