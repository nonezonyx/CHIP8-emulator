#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <boost/noncopyable.hpp>
#include <cstdint>
#include <optional>
#include <string>
#include <view_fwd.hpp>

namespace nznyx::chip8 {

class abstract_view : public boost::noncopyable {
public:
    virtual void clear_screen() = 0;
    virtual bool draw(int x, int y, const std::uint8_t *sprite, int n) = 0;
    virtual std::optional<flags_type> update_flags(flags_type flags) = 0;
    virtual std::string help() = 0;
    virtual std::string greeting() = 0;
    /* Returns true on exit */
    virtual bool process_events(const key_input_func_type &key_input) = 0;

    abstract_view() = default;
    virtual ~abstract_view() = default;
};
}  // namespace nznyx::chip8

#endif  // VIEW_HPP_
