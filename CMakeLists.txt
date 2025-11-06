cmake_minimum_required(VERSION 3.15)
project(task-manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Manually set SDL2 library to the dylib version
set(SDL2_LIBRARIES /opt/homebrew/lib/libSDL2.dylib)

find_package(OpenGL REQUIRED)

# Add ImGui library
file(GLOB IMGUI_SOURCES
    "imgui/imgui/imgui.cpp"
    "imgui/imgui/imgui_demo.cpp"
    "imgui/imgui/imgui_draw.cpp"
    "imgui/imgui/imgui_tables.cpp"
    "imgui/imgui/imgui_widgets.cpp"
    "imgui/imgui/backends/imgui_impl_sdl2.cpp"
    "imgui/imgui/backends/imgui_impl_opengl3.cpp"
)

add_executable(task-manager
    src/main.cpp
    ${IMGUI_SOURCES}
)

# Include directories
target_include_directories(task-manager PRIVATE
    imgui/imgui
    imgui/imgui/backends
    ${SDL2_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(task-manager PRIVATE
    ${SDL2_LIBRARIES}
)

# Platform-specific settings
if(APPLE)
    target_link_libraries(task-manager PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(task-manager PRIVATE OpenGL)
endif()
