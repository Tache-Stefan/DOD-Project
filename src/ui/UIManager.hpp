#pragma once

#include "core/ISimulation.hpp"
#include "core/Config.hpp"

struct UIState {
    int entityCount = static_cast<int>(Config::DEFAULT_ENTITIES);
    float simSpeed = Config::DEFAULT_SIM_SPEED;
    bool isPaused = false;
    bool enableCollisions = true;
    SimArchitecture selectedArch = SimArchitecture::OOP;
};

class UIManager {
public:
    UIManager() = default;
    ~UIManager() = default;

    void init();
    void shutdown();
    void render(UIState& state, ISimulation* activeSim);
};
