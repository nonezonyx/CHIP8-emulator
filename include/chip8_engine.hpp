#ifndef CHIP8_ENGINE_HPP_
#define CHIP8_ENGINE_HPP_

#include <chip8_engine_fwd.hpp>

namespace nznyx::chip8 {
class chip8_engine {
private:
public:
    void load_fontset() noexcept;
};
}  // namespace nznyx::chip8

#endif  // CHIP8_ENGINE_HPP_
