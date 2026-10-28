#pragma once

#include "Actor.hpp"
#include "core/Config.hpp"
#include "physics/Physics.hpp"

class SpaceCraft : public Actor {
public:
    SpaceCraft(uint32_t id, float posX, float posY, float velX, float velY, float radius = Config::BALL_RADIUS)
        : Actor(id),
          posX(posX),
          posY(posY),
          velX(velX),
          velY(velY),
          radius(radius > 0.0f ? radius : Config::BALL_RADIUS) {}

    ~SpaceCraft() override = default;

    void update(float dt) override {
        if (!isActive()) return;

        Physics::integratePosition(posX, posY, velX, velY, dt);
    }

    void checkBoundaryCollision(float worldWidth, float worldHeight) noexcept override {
        Physics::resolveBoundary(posX, posY, velX, velY, radius, worldWidth, worldHeight, Config::RESTITUTION);
    }

    bool resolveCollisionWith(SpaceCraft& other) noexcept {
        return Physics::resolveCircleCollision(
            posX, posY, velX, velY, radius,
            other.posX, other.posY, other.velX, other.velY, other.radius,
            Config::RESTITUTION
        );
    }

    SpaceCraft* asSpaceCraft() noexcept override { return this; }
    const SpaceCraft* asSpaceCraft() const noexcept override { return this; }

    [[nodiscard]] float getPosX() const noexcept { return posX; }
    [[nodiscard]] float getPosY() const noexcept { return posY; }
    void setPosition(float x, float y) noexcept { posX = x; posY = y; }

    [[nodiscard]] float getVelX() const noexcept { return velX; }
    [[nodiscard]] float getVelY() const noexcept { return velY; }
    void setVelocity(float vx, float vy) noexcept { velX = vx; velY = vy; }

    [[nodiscard]] float getRadius() const noexcept { return radius; }
    void setRadius(float r) noexcept { radius = r; }

protected:
    float posX;
    float posY;
    float velX;
    float velY;
    float radius;
};
