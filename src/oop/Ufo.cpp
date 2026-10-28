#include "Ufo.hpp"

void Ufo::render() const {
    if (!isActive()) return;

    DrawCircle(static_cast<int>(posX), static_cast<int>(posY), radius, Config::COLOR_BALL_OOP);
}
