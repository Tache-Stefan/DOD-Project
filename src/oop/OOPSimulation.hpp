#pragma once
 
#include "core/ISimulation.hpp"
#include "core/Config.hpp"
#include "physics/SpatialGrid.hpp"
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
    void renderDebug() const override;
 
    void setEntityCount(size_t count) override;
    [[nodiscard]] size_t getEntityCount() const override;
 
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] SimArchitecture getArchitecture() const override;
    [[nodiscard]] size_t getTheoreticalMemoryBytes() const override;
 
    void setWorldBounds(float width, float height) override;
 
    void setCollisionsEnabled(bool enabled) noexcept override { collisionsEnabled = enabled; }
    [[nodiscard]] bool areCollisionsEnabled() const noexcept override { return collisionsEnabled; }

    void startRace() override;
    void stopRace() override;
    [[nodiscard]] bool isRaceActive() const noexcept override { return raceActive; }
    [[nodiscard]] std::array<size_t, Config::NUM_TEAMS> getTeamCounts() const override;

    [[nodiscard]] const std::vector<std::unique_ptr<Actor>>& getEntities() const noexcept {
        return entities;
    }
 
private:
    void spawnEntities(size_t count);
    void resolveCollisions();
    void recomputeTeamCounts();

    std::vector<std::unique_ptr<Actor>> entities;
    SpatialGrid grid;
    float worldWidth;
    float worldHeight;
    uint32_t nextEntityId;
    bool collisionsEnabled = true;
    bool raceActive = false;
    std::array<size_t, Config::NUM_TEAMS> teamCounts{};
};