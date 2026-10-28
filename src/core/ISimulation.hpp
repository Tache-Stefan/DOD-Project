#pragma once

#include <cstddef>
#include <string_view>

enum class SimArchitecture {
    OOP,
    DOD
};

class ISimulation {
public:
    virtual ~ISimulation() = default;

    virtual void init(size_t initialEntities, float worldWidth, float worldHeight) = 0;

    virtual void update(float dt) = 0;

    virtual void render() const = 0;

    virtual void setEntityCount(size_t count) = 0;

    [[nodiscard]] virtual size_t getEntityCount() const = 0;

    [[nodiscard]] virtual std::string_view getName() const = 0;

    [[nodiscard]] virtual SimArchitecture getArchitecture() const = 0;

    [[nodiscard]] virtual size_t getTheoreticalMemoryBytes() const = 0;

    virtual void setWorldBounds(float width, float height) = 0;

    virtual void setCollisionsEnabled(bool enabled) = 0;
    [[nodiscard]] virtual bool areCollisionsEnabled() const = 0;
};
