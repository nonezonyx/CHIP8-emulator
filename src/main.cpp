#include <boost/dll/import.hpp>
#include <boost/version.hpp>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include "chip8_engine.hpp"
#include "chip8_engine_fwd.hpp"
#include "view.hpp"
#include "view_fwd.hpp"

#if BOOST_VERSION >= 107600
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import_symbol
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import
#endif

int main(int argc, char *argv[]) {
    const int target_frequency = 700;
    if (argc < 3) {
        std::string usage = std::format(
            "Usage:\n"
            "  {} <view_dynamic_lib> <file_to_run> [optional_flags]\n\n"
            "Arguments:\n"
            "  <view_dynamic_lib>     Path to the dynamic library (view) to "
            "use for rendering.\n"
            "  <file_to_run>          Path to the CHIP-8 program or ROM to "
            "execute.\n\n"
            "Optional Flags:\n"
            "  [optional_flags]       Flags or options for the rendering view, "
            "if supported.\n\n",
            argv[0]
        );
        std::cout << usage << std::endl;
        return 1;
    }

    auto view = boost_dll_import_symbol<nznyx::chip8::abstract_view>(
        argv[1],  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        "view",
        boost::dll::load_mode::
            append_decorations  // NOLINT(misc-include-cleaner)
    );

    std::cout << view->greeting() << std::endl;
    const nznyx::chip8::clear_func_type clear_func =
        std::bind(&nznyx::chip8::abstract_view::clear_screen, view.get());
    const nznyx::chip8::draw_func_type draw_func = std::bind(
        &nznyx::chip8::abstract_view::draw, view.get(), std::placeholders::_1,
        std::placeholders::_2, std::placeholders::_3, std::placeholders::_4
    );

    auto chip8 = nznyx::chip8::chip8_engine(clear_func, draw_func);
    const nznyx::chip8::key_input_func_type key_input = std::bind(
        &nznyx::chip8::chip8_engine::set_pressed, &chip8, std::placeholders::_1,
        std::placeholders::_2
    );

    chip8.load(argv[2]);

    const std::chrono::nanoseconds cycle_duration(
        1'000'000'000 / target_frequency
    );
    auto previous_time = std::chrono::high_resolution_clock::now();
    while (true) {
        chip8.execute();
        if (view->process_events(key_input)) {
            return 0;
        }

        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - previous_time
            );
        previous_time = std::chrono::high_resolution_clock::now();

        if (elapsed_time < cycle_duration) {
            std::this_thread::sleep_for(cycle_duration - elapsed_time);
        }
    }
}
