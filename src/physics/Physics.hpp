#pragma once

#include "core/Config.hpp"
#include "raylib.h"
#include <cmath>

namespace Physics {

    inline void integratePosition(float& x, float& y, float vx, float vy, float dt) {
        x += vx * dt;
        y += vy * dt;
    }

    inline void integratePosition(Vector2& pos, Vector2 vel, float dt) {
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
    }

    inline void resolveBoundary(float& x, float& y, float& vx, float& vy,
                                float radius, float worldWidth, float worldHeight,
                                float restitution = Config::RESTITUTION) {
        // Left wall
        if (x - radius < 0.0f) {
            x = radius;
            vx = -vx * restitution;
        }
        // Right wall
        else if (x + radius > worldWidth) {
            x = worldWidth - radius;
            vx = -vx * restitution;
        }

        // Top wall
        if (y - radius < 0.0f) {
            y = radius;
            vy = -vy * restitution;
        }
        // Bottom wall
        else if (y + radius > worldHeight) {
            y = worldHeight - radius;
            vy = -vy * restitution;
        }
    }

    inline void resolveBoundary(Vector2& pos, Vector2& vel,
                                float radius, float worldWidth, float worldHeight,
                                float restitution = Config::RESTITUTION) {
        resolveBoundary(pos.x, pos.y, vel.x, vel.y, radius, worldWidth, worldHeight, restitution);
    }

    inline bool resolveCircleCollision(
        float& x1, float& y1, float& vx1, float& vy1, float r1, float m1,
        float& x2, float& y2, float& vx2, float& vy2, float r2, float m2,
        float restitution = Config::RESTITUTION
    ) {
        const float dx = x2 - x1;
        const float dy = y2 - y1;
        const float distSq = dx * dx + dy * dy;
        const float minDist = r1 + r2;

        if (distSq >= minDist * minDist || distSq <= 0.00001f) {
            return false;
        }

        const float dist = std::sqrt(distSq);
        const float nx = dx / dist;
        const float ny = dy / dist;

        const float overlap = minDist - dist;
        const float invM1 = (m1 > 0.0f) ? (1.0f / m1) : 0.0f;
        const float invM2 = (m2 > 0.0f) ? (1.0f / m2) : 0.0f;
        const float invMassSum = invM1 + invM2;

        if (invMassSum <= 0.0f) {
            return false;
        }

        const float ratio1 = invM1 / invMassSum;
        const float ratio2 = invM2 / invMassSum;

        x1 -= nx * overlap * ratio1;
        y1 -= ny * overlap * ratio1;
        x2 += nx * overlap * ratio2;
        y2 += ny * overlap * ratio2;

        const float relVx = vx1 - vx2;
        const float relVy = vy1 - vy2;
        const float velAlongNormal = relVx * nx + relVy * ny;

        // Only resolve if objects are approaching each other
        if (velAlongNormal <= 0.0f) {
            return true;
        }

        // 1D elastic impulse along normal
        const float impulse = ((1.0f + restitution) * velAlongNormal) / invMassSum;

        vx1 -= impulse * invM1 * nx;
        vy1 -= impulse * invM1 * ny;
        vx2 += impulse * invM2 * nx;
        vy2 += impulse * invM2 * ny;

        return true;
    }

    inline bool resolveCircleCollision(
        float& x1, float& y1, float& vx1, float& vy1, float r1,
        float& x2, float& y2, float& vx2, float& vy2, float r2,
        float restitution = Config::RESTITUTION
    ) {
        return resolveCircleCollision(x1, y1, vx1, vy1, r1, Config::BALL_MASS,
                                      x2, y2, vx2, vy2, r2, Config::BALL_MASS,
                                      restitution);
    }

    inline bool resolveCircleCollision(
        Vector2& pos1, Vector2& vel1, float r1, float m1,
        Vector2& pos2, Vector2& vel2, float r2, float m2,
        float restitution = Config::RESTITUTION
    ) {
        return resolveCircleCollision(
            pos1.x, pos1.y, vel1.x, vel1.y, r1, m1,
            pos2.x, pos2.y, vel2.x, vel2.y, r2, m2,
            restitution
        );
    }

    inline bool resolveCircleCollision(
        Vector2& pos1, Vector2& vel1, float r1,
        Vector2& pos2, Vector2& vel2, float r2,
        float restitution = Config::RESTITUTION
    ) {
        return resolveCircleCollision(
            pos1.x, pos1.y, vel1.x, vel1.y, r1, Config::BALL_MASS,
            pos2.x, pos2.y, vel2.x, vel2.y, r2, Config::BALL_MASS,
            restitution
        );
    }

}
