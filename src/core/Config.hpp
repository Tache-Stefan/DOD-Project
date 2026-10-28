#pragma once

#include "raylib.h"
#include <cstddef>

namespace Config {

    inline constexpr int WINDOW_WIDTH = 1280;
    inline constexpr int WINDOW_HEIGHT = 720;
    inline constexpr const char* WINDOW_TITLE = "Alien Invasion - OOP vs DOD";

    inline constexpr size_t DEFAULT_ENTITIES = 1000;
    inline constexpr size_t MIN_ENTITIES     = 100;
    inline constexpr size_t MAX_ENTITIES     = 100000;

    inline constexpr float BALL_RADIUS          = 4.0f;
    inline constexpr float BALL_MASS            = 1.0f;
    inline constexpr float BALL_MAX_SPAWN_SPEED = 120.0f;
    inline constexpr float SPAWN_BORDER_PADDING = 5.0f;

    inline constexpr float RESTITUTION         = 1.0f; // 1.0 = fully elastic
    inline constexpr float DEFAULT_SIM_SPEED   = 1.0f;
    inline constexpr float MIN_SIM_SPEED       = 0.0f;
    inline constexpr float MAX_SIM_SPEED       = 3.0f;

    inline constexpr Color COLOR_BACKGROUND    = { 24, 24, 36, 255 };
    inline constexpr Color COLOR_BALL_DEFAULT  = { 70, 180, 255, 220 }; // Neon cyan/blue

    inline constexpr Color COLOR_BALL_OOP      = { 255, 110, 110, 220 }; // Neon coral/red
    inline constexpr Color COLOR_BALL_DOD      = { 70, 180, 255, 220 }; // Neon cyan/blue

    inline constexpr Color COLOR_BORDER        = { 102, 191, 255, 80 };

}
