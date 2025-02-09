#ifndef VIEW_FWD_HPP_
#define VIEW_FWD_HPP_

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include "chip8_engine_fwd.hpp"

namespace nznyx::chip8 {
using key_input_func_type = std::function<void(key, bool)>;
using flags_type = std::vector<std::pair<std::string, std::string>>;
class abstract_view;
}  // namespace nznyx::chip8

#endif  // VIEW_FWD_HPP_
