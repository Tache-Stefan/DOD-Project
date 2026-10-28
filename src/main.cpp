#include <raylib.h>
#include "core/Config.hpp"
#include "core/ISimulation.hpp"
#include "oop/OOPSimulation.hpp"
#include "dod/DODSimulation.hpp"
#include "ui/UIManager.hpp"
#include <memory>

static std::unique_ptr<ISimulation> createSimulation(SimArchitecture arch, size_t entityCount, float width, float height, bool enableCollisions = true) {
    std::unique_ptr<ISimulation> sim;
    if (arch == SimArchitecture::DOD) {
        sim = std::make_unique<DODSimulation>();
    } else {
        sim = std::make_unique<OOPSimulation>();
    }
    sim->init(entityCount, width, height);
    sim->setCollisionsEnabled(enableCollisions);
    return sim;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE);

    UIManager uiManager;
    uiManager.init();

    UIState uiState;
    uiState.entityCount = static_cast<int>(Config::DEFAULT_ENTITIES);
    uiState.simSpeed = Config::DEFAULT_SIM_SPEED;
    uiState.selectedArch = SimArchitecture::OOP;

    // Only maintain one active simulation in memory
    std::unique_ptr<ISimulation> currentSim = createSimulation(
        uiState.selectedArch,
        static_cast<size_t>(uiState.entityCount),
        static_cast<float>(Config::WINDOW_WIDTH),
        static_cast<float>(Config::WINDOW_HEIGHT),
        uiState.enableCollisions
    );
    SimArchitecture activeArch = uiState.selectedArch;

    while (!WindowShouldClose()) {
        const float screenW = static_cast<float>(GetScreenWidth());
        const float screenH = static_cast<float>(GetScreenHeight());

        // Handle live architecture switch: destroy old sim and create the new one
        if (uiState.selectedArch != activeArch) {
            activeArch = uiState.selectedArch;
            currentSim = createSimulation(
                activeArch,
                static_cast<size_t>(uiState.entityCount),
                screenW,
                screenH,
                uiState.enableCollisions
            );
        }

        if (IsWindowResized()) {
            currentSim->setWorldBounds(screenW, screenH);
        }

        float dt = GetFrameTime();
        if (!uiState.isPaused) {
            currentSim->update(dt * uiState.simSpeed);
        }

        BeginDrawing();
        ClearBackground(Config::COLOR_BACKGROUND);

        currentSim->render();

        uiManager.render(uiState, currentSim.get());

        EndDrawing();
    }

    uiManager.shutdown();
    CloseWindow();

    return 0;
}
