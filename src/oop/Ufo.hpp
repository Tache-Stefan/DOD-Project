#pragma once

#include "SpaceCraft.hpp"
#include "core/Config.hpp"
#include <raylib.h>

class Ufo : public SpaceCraft {
public:
    Ufo(uint32_t id, float posX, float posY, float velX, float velY,
        float radius = Config::BALL_RADIUS)
        : SpaceCraft(id, posX, posY, velX, velY, radius) {}

    ~Ufo() override = default;

    void render() const override;
};
