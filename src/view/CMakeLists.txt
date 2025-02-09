# Create a shared library for SDL2-based view
add_library(sdl2_view SHARED sdl2_view.cpp)

# Specify needed include directories and link libraries
target_include_directories(sdl2_view PRIVATE ${CMAKE_BINARY_DIR}/include ${SDL2_INCLUDE_DIRS})
target_link_libraries(sdl2_view PRIVATE Boost::filesystem Boost::system ${SDL2_LIBRARIES})

# Export all symbols for DLL (mostly useful on Windows)
set_target_properties(sdl2_view PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
