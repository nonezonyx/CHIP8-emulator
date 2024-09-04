#include <cassert>
#include <chip8_engine.hpp>
#include <chip8_engine_fwd.hpp>
#include <cstring>

namespace nznyx::chip8 {

chip8_engine::chip8_engine() {
    change_fontset(chip8_default_fontset);
}

void chip8_engine::change_fontset(const unsigned char fontset[80]) noexcept {
    std::memcpy(memory_ + FONT_POSITION, fontset, 80);
}

const unsigned char *chip8_engine::get_memory() const noexcept {
    return memory_;
}

}  // namespace nznyx::chip8
