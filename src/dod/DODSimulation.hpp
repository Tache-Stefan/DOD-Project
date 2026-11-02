#pragma once

#include "core/ISimulation.hpp"
#include "core/Config.hpp"
#include "core/CircleTexture.hpp"
#include "physics/Physics.hpp"
#include "physics/SpatialGrid.hpp"
#include "raylib.h"

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

class DODSimulation : public ISimulation {
public:
    DODSimulation() = default;
    ~DODSimulation() override = default;

    void init(size_t initialEntities, float worldWidth, float worldHeight) override {
        m_worldWidth = worldWidth;
        m_worldHeight = worldHeight;
        m_grid.init(m_worldWidth, m_worldHeight, Config::GRID_CELL_SIZE);
        setEntityCount(initialEntities);
    }

    void update(float dt) override {
        const size_t count = m_posX.size();

        for (size_t i = 0; i < count; ++i) {
            Physics::integratePosition(m_posX[i], m_posY[i], m_velX[i], m_velY[i], dt);
        }

        if (m_collisionsEnabled && count > 1) {
            m_grid.clear(count);
            for (size_t i = 0; i < count; ++i) {
                m_grid.insert(static_cast<uint32_t>(i), m_posX[i], m_posY[i]);
            }

            m_grid.forEachCandidatePair([this](uint32_t i, uint32_t j) {
                Physics::resolveCircleCollision(
                    m_posX[i], m_posY[i], m_velX[i], m_velY[i], m_radius[i],
                    m_posX[j], m_posY[j], m_velX[j], m_velY[j], m_radius[j]
                );
            });
        }

        for (size_t i = 0; i < count; ++i) {
            Physics::resolveBoundary(m_posX[i], m_posY[i], m_velX[i], m_velY[i],
                                     m_radius[i], m_worldWidth, m_worldHeight);
        }
    }

    void render() const override {
        const size_t count = m_posX.size();
        for (size_t i = 0; i < count; ++i) {
            CircleTexture::draw(m_posX[i], m_posY[i], m_radius[i], Config::COLOR_BALL_DOD);
        }
    }

    void renderDebug() const override {
        m_grid.drawDebug();
    }

    void setEntityCount(size_t count) override {
        const size_t current = m_posX.size();

        if (count > current) {
            m_posX.reserve(count);
            m_posY.reserve(count);
            m_velX.reserve(count);
            m_velY.reserve(count);
            m_radius.reserve(count);

            std::random_device rd;
            std::mt19937 gen(rd());

            const float r = Config::BALL_RADIUS;
            const float padding = Config::SPAWN_BORDER_PADDING;
            const float minX = r + padding;
            const float maxX = (m_worldWidth > minX + 1.0f) ? (m_worldWidth - r - padding) : minX;
            const float minY = r + padding;
            const float maxY = (m_worldHeight > minY + 1.0f) ? (m_worldHeight - r - padding) : minY;

            std::uniform_real_distribution<float> posXDist(minX, maxX);
            std::uniform_real_distribution<float> posYDist(minY, maxY);

            const float maxSpeed = Config::BALL_MAX_SPAWN_SPEED;
            std::uniform_real_distribution<float> velDist(-maxSpeed, maxSpeed);

            for (size_t i = current; i < count; ++i) {
                float x = posXDist(gen);
                float y = posYDist(gen);
                float vx = velDist(gen);
                float vy = velDist(gen);

                if (std::abs(vx) < 15.0f) vx = (vx < 0.0f) ? -25.0f : 25.0f;
                if (std::abs(vy) < 15.0f) vy = (vy < 0.0f) ? -25.0f : 25.0f;

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
        m_grid.setWorldBounds(width, height);
    }

    void setCollisionsEnabled(bool enabled) noexcept override {
        m_collisionsEnabled = enabled;
    }

    [[nodiscard]] bool areCollisionsEnabled() const noexcept override {
        return m_collisionsEnabled;
    }

private:
    std::vector<float> m_posX;
    std::vector<float> m_posY;
    std::vector<float> m_velX;
    std::vector<float> m_velY;
    std::vector<float> m_radius;

    SpatialGrid m_grid;
    float m_worldWidth = static_cast<float>(Config::WINDOW_WIDTH);
    float m_worldHeight = static_cast<float>(Config::WINDOW_HEIGHT);
    bool m_collisionsEnabled = true;
};
