#pragma once

#include "core/ISimulation.hpp"
#include "core/Config.hpp"
#include "physics/Physics.hpp"
#include "raylib.h"

#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

class DodSimulation : public ISimulation {
public:
    DodSimulation() = default;
    ~DodSimulation() override = default;

    void init(size_t initialEntities, float worldWidth, float worldHeight) override {
        m_worldWidth = worldWidth;
        m_worldHeight = worldHeight;
        setEntityCount(initialEntities);
    }

    void update(float dt) override {
        const size_t count = m_posX.size();
        if (count == 0) return;

        for (size_t i = 0; i < count; ++i) {
            Physics::integratePosition(m_posX[i], m_posY[i], m_velX[i], m_velY[i], dt);
        }

        for (size_t i = 0; i < count; ++i) {
            Physics::resolveBoundary(m_posX[i], m_posY[i], m_velX[i], m_velY[i],
                                     m_radius[i], m_worldWidth, m_worldHeight);
        }

        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                Physics::resolveCircleCollision(
                    m_posX[i], m_posY[i], m_velX[i], m_velY[i], m_radius[i],
                    m_posX[j], m_posY[j], m_velX[j], m_velY[j], m_radius[j]
                );
            }
        }
    }

    void render() const override {
        const size_t count = m_posX.size();
        for (size_t i = 0; i < count; ++i) {
            DrawCircle(
                static_cast<int>(m_posX[i]),
                static_cast<int>(m_posY[i]),
                m_radius[i],
                Config::COLOR_BALL_DOD
            );
        }
    }

    void setEntityCount(size_t count) override {
        const size_t current = m_posX.size();

        if (count > current) {
            m_posX.reserve(count);
            m_posY.reserve(count);
            m_velX.reserve(count);
            m_velY.reserve(count);
            m_radius.reserve(count);

            for (size_t i = current; i < count; ++i) {
                const float r = Config::BALL_RADIUS;
                const float minX = r + Config::SPAWN_BORDER_PADDING;
                const float maxX = (m_worldWidth > r * 2.0f) ? (m_worldWidth - r - Config::SPAWN_BORDER_PADDING) : minX;
                const float minY = r + Config::SPAWN_BORDER_PADDING;
                const float maxY = (m_worldHeight > r * 2.0f) ? (m_worldHeight - r - Config::SPAWN_BORDER_PADDING) : minY;

                const float x = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX + 1.0f)));
                const float y = minY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxY - minY + 1.0f)));

                const float speed = Config::BALL_MAX_SPAWN_SPEED;
                const float vx = -speed + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.0f * speed)));
                const float vy = -speed + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.0f * speed)));

                m_posX.push_back(x);
                m_posY.push_back(y);
                m_velX.push_back(vx);
                m_velY.push_back(vy);
                m_radius.push_back(r);
            }
        } else if (count < current) {
            m_posX.resize(count);
            m_posY.resize(count);
            m_velX.resize(count);
            m_velY.resize(count);
            m_radius.resize(count);
        }
    }

    [[nodiscard]] size_t getEntityCount() const override {
        return m_posX.size();
    }

    [[nodiscard]] std::string_view getName() const override {
        return "DOD";
    }

    [[nodiscard]] SimArchitecture getArchitecture() const override {
        return SimArchitecture::DOD;
    }

    [[nodiscard]] size_t getTheoreticalMemoryBytes() const override {
        return m_posX.size() * (sizeof(float) * 5);
    }

    void setWorldBounds(float width, float height) override {
        m_worldWidth = width;
        m_worldHeight = height;
    }

private:
    std::vector<float> m_posX;
    std::vector<float> m_posY;
    std::vector<float> m_velX;
    std::vector<float> m_velY;
    std::vector<float> m_radius;

    float m_worldWidth = static_cast<float>(Config::WINDOW_WIDTH);
    float m_worldHeight = static_cast<float>(Config::WINDOW_HEIGHT);
};
