cmake_minimum_required(VERSION 3.20)
project(AlienInvasion VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O3)
endif()

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "Disable raylib examples" FORCE)
set(CUSTOMIZE_BUILD OFF CACHE BOOL "Disable raylib customization" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.93.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    rlImGui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(rlImGui)

FetchContent_GetProperties(imgui SOURCE_DIR imgui_src_dir)
FetchContent_GetProperties(rlImGui SOURCE_DIR rlimgui_src_dir)

add_library(rlImGui_lib STATIC
    ${imgui_src_dir}/imgui.cpp
    ${imgui_src_dir}/imgui_demo.cpp
    ${imgui_src_dir}/imgui_draw.cpp
    ${imgui_src_dir}/imgui_tables.cpp
    ${imgui_src_dir}/imgui_widgets.cpp
    ${rlimgui_src_dir}/rlImGui.cpp
)

target_include_directories(rlImGui_lib PUBLIC
    ${imgui_src_dir}
    ${rlimgui_src_dir}
)

target_link_libraries(rlImGui_lib PUBLIC
    raylib
)

add_executable(AlienInvasion
    src/main.cpp
)

target_include_directories(AlienInvasion PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${imgui_src_dir}
    ${rlimgui_src_dir}
)

target_link_libraries(AlienInvasion PRIVATE
    raylib
    rlImGui_lib
)
