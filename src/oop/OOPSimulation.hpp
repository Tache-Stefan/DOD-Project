#pragma once

#include "core/ISimulation.hpp"
#include "core/Config.hpp"
#include "Actor.hpp"
#include "Ufo.hpp"
#include <vector>
#include <memory>
#include <cstdint>

class OOPSimulation : public ISimulation {
public:
    OOPSimulation();
    ~OOPSimulation() override = default;

    void init(size_t initialEntities, float worldWidth, float worldHeight) override;
    void update(float dt) override;
    void render() const override;

    void setEntityCount(size_t count) override;
    [[nodiscard]] size_t getEntityCount() const override;

    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] SimArchitecture getArchitecture() const override;
    [[nodiscard]] size_t getTheoreticalMemoryBytes() const override;

    void setWorldBounds(float width, float height) override;

    void setCollisionsEnabled(bool enabled) noexcept { collisionsEnabled = enabled; }
    [[nodiscard]] bool areCollisionsEnabled() const noexcept { return collisionsEnabled; }

    [[nodiscard]] const std::vector<std::unique_ptr<Actor>>& getEntities() const noexcept {
        return entities;
    }

private:
    void spawnEntities(size_t count);
    void resolveCollisions();

    std::vector<std::unique_ptr<Actor>> entities;
    float worldWidth;
    float worldHeight;
    uint32_t nextEntityId;
    bool collisionsEnabled = true;
};
