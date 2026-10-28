#pragma once

#include <cstdint>

enum class ActorState {
    Active,
    Inactive,
    Destroyed
};

class SpaceCraft;

class Actor {
public:
    explicit Actor(uint32_t id)
        : id(id), state(ActorState::Active) {}

    virtual ~Actor() = default;

    virtual void update(float dt) = 0;
    virtual void render() const = 0;
    virtual void checkBoundaryCollision(float worldWidth, float worldHeight) = 0;

    virtual SpaceCraft* asSpaceCraft() noexcept { return nullptr; }
    virtual const SpaceCraft* asSpaceCraft() const noexcept { return nullptr; }

    [[nodiscard]] uint32_t getId() const noexcept { return id; }
    [[nodiscard]] ActorState getState() const noexcept { return state; }
    void setState(ActorState newState) noexcept { state = newState; }
    [[nodiscard]] bool isActive() const noexcept { return state == ActorState::Active; }

protected:
    uint32_t id;
    ActorState state;
};
