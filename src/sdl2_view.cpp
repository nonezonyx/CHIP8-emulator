#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL_events.h>
#include <array>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <view.hpp>
#include "chip8_engine_fwd.hpp"
#include "view_fwd.hpp"

namespace nznyx::chip8 {

class sdl2_basic_view : public abstract_view {
public:
    sdl2_basic_view() {
        screen_.fill(false);
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error(SDL_GetError());
        }

        window_ = SDL_CreateWindow(
            "Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            64 * SCALE, 32 * SCALE, SDL_WINDOW_SHOWN
        );

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

        SDL_RenderSetLogicalSize(renderer_, 64, 32);

        texture_ = SDL_CreateTexture(
            renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
            64, 32
        );

        if (!window_ || !renderer_ || !texture_) {
            throw std::runtime_error("Could not initialize SDL2");
        }

        update_texture();
    }

    void clear_screen() override {
        screen_.fill(false);
        update_texture();
    }

    bool draw(int x, int y, const std::uint8_t *sprite, int n) override {
        bool changed = false;
        int line = 0;
        while (y < 32 && line < n) {
            for (int i = 0; x + i < 64 && i < 8; ++i) {
                bool &pixel = screen_[64 * y + x + i];
                bool snapshot = pixel;
                pixel ^= sprite[line] >> (7 - i) & 1;
                changed |= snapshot && snapshot != pixel;
            }
            ++line;
            ++y;
        }
        update_texture();
        return changed;
    }

    std::optional<flags_type> update_flags(flags_type flags) override {
        return flags;
    }

    std::string help() override {
        return "Example sdl2 help";
    }

    std::string greeting() override {
        return "Example sdl2 greeting";
    }

    bool process_events(const key_input_func_type &key_input) override {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return true;
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                auto key = translate_key(e.key.keysym.sym);
                if (key.has_value()) {
                    key_input(key.value(), e.type == SDL_KEYDOWN);
                }
            }
        }
        return false;
    }

    ~sdl2_basic_view() override {
        if (texture_) {
            SDL_DestroyTexture(texture_);
        }
        if (renderer_) {
            SDL_DestroyRenderer(renderer_);
        }
        if (window_) {
            SDL_DestroyWindow(window_);
        }
        SDL_Quit();
    }

private:
    static constexpr int SCALE = 20;
    SDL_Window *window_;
    SDL_Renderer *renderer_;
    SDL_Texture *texture_;

    std::array<bool, 64 * 32> screen_;
    std::uint32_t off_color_ = 0xFF000000;
    std::uint32_t on_color_ = -1;

    void update_texture() {
        std::array<uint32_t, 64 * 32> texture_pixels;
        for (size_t i = 0; i < screen_.size(); i++) {
            texture_pixels[i] = screen_[i] ? on_color_ : off_color_;
        }

        SDL_UpdateTexture(
            texture_, nullptr, texture_pixels.data(), 64 * sizeof(uint32_t)
        );

        SDL_RenderClear(renderer_);
        SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
        SDL_RenderPresent(renderer_);
    }

    static std::optional<key> translate_key(SDL_Keycode key) noexcept {
        switch (key) {
            case SDLK_1:
                return key::KEY_1;
            case SDLK_2:
                return key::KEY_2;
            case SDLK_3:
                return key::KEY_3;
            case SDLK_4:
                return key::KEY_C;
            case SDLK_q:
                return key::KEY_4;
            case SDLK_w:
                return key::KEY_5;
            case SDLK_e:
                return key::KEY_6;
            case SDLK_r:
                return key::KEY_D;
            case SDLK_a:
                return key::KEY_7;
            case SDLK_s:
                return key::KEY_8;
            case SDLK_d:
                return key::KEY_9;
            case SDLK_f:
                return key::KEY_E;
            case SDLK_z:
                return key::KEY_A;
            case SDLK_x:
                return key::KEY_0;
            case SDLK_c:
                return key::KEY_B;
            case SDLK_v:
                return key::KEY_F;
            default:
                return std::nullopt;
        }
    }
};

}  // namespace nznyx::chip8

extern "C" BOOST_SYMBOL_EXPORT nznyx::chip8::sdl2_basic_view view;
nznyx::chip8::sdl2_basic_view view;
