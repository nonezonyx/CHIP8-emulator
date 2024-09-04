#include <cassert>
#include <chip8_engine.hpp>
#include <chip8_engine_fwd.hpp>
#include <cstring>

namespace nznyx::chip8 {

void chip8_engine::change_fontset(const unsigned char fontset[80]) noexcept {
    std::memcpy(memory_ + FONT_POSITION, fontset, 80);
}

}  // namespace nznyx::chip8
