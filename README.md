# 🎮 CHIP8-emulator

A flexible CHIP-8 emulator with plugin-based viewer support, implementing the original instruction set. Designed for extensibility through dynamic libraries and clean architecture.

## 🚀 TLDR

Clone this repo and compile project with:

```bash
mkdir build && cd build
cmake ..
make
```

To run:

```bash
./chip8-emulator sdl2-view your_rom.ch8
```

🕹️[Here](https://johnearnest.github.io/chip8Archive/) you could get some roms

![Spacejam rom](assets/spacejam.png)

## ✨ Features

- 🧩 Full implementation of original CHIP-8 instruction set
- 🔌 Plugin-based viewer system using Boost.DLL
- 🌟 Example SDL2-based viewer implementation
- 🌐 Cross-platform compatibility
- 🧹 Clean, modern C++ architecture

## 📦 Installation

### 🔑 Dependencies

- C++23 compiler
- CMake 3.22+
- SDL2 development libraries
- Boost 1.74+ (Headers and DLL components)

### 🔨 Building

```bash
mkdir build && cd build
cmake ..
make
```

This will produce:

- `chip8-emulator` (main executable)
- `libsdl2-view.so` (SDL2 viewer plugin)

### Usage

Run the emulator with a viewer plugin and ROM:

```bash
./chip8-emulator sdl2-view ../roms/game.ch8
```

## 🎮 Controls

Uses standart key mapping

```
COSMAC VIP    maps to
 1 2 3 C      1 2 3 4
 4 5 6 D      Q W E R
 7 8 9 E      A S D F
 A 0 B F      Z X C V
```

## 🔌 Plugin system

The emulator uses Boost.DLL to load viewer plugins at runtime. Implement the `abstract_view` interface from `include/view.hpp` to create custom viewers.

Example viewer implementations:

- SDL2 Viewer (included)
- Terminal Viewer (planned)
- Browser Viewer (planned)

## ⚙️ Building Specific Components

Select components with CMake options:

```bash
cmake -DBUILD_CHIP8_EMULATOR=ON -DBUILD_CHIP8_VIEW_DLL=ON -DBUILD_CHIP8_TESTS=OFF ..
```

Available targets:

- `BUILD_CHIP8_EMULATOR` (main executable)
- `BUILD_CHIP8_VIEW_DLL` (SDL2 viewer plugin)
- `BUILD_CHIP8_TESTS` (Test suite)

## 🔮 Future Plans

✨ Feature wishlist for future releases:

- 🚀 Super CHIP 48/XO-CHIP support
- 🖤 Additional viewers
  - Terminal Viewer
  - Browser/Web Viewer
- 💾 Save/Load state functionality
- 🪲 Debugging interface and disassembler

# 🎉 Happy tinkering & emulating! 🌟
