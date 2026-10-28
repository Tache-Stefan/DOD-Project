#include "UIManager.hpp"
#include "oop/OOPSimulation.hpp"
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>

void UIManager::init() {
    rlImGuiSetup(true);
}

void UIManager::shutdown() {
    rlImGuiShutdown();
}

void UIManager::render(UIState& state, ISimulation* activeSim) {
    rlImGuiBegin();

    ImGui::SetNextWindowPos(ImVec2(15.0f, 15.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(360.0f, 320.0f), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Alien Invasion Control Panel")) {
        if (activeSim) {
            ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.3f, 1.0f), "Mode: %.*s",
                               static_cast<int>(activeSim->getName().size()),
                               activeSim->getName().data());
        }

        ImGui::Spacing();
        ImGui::Text("Architecture Switch:");
        int archChoice = (state.selectedArch == SimArchitecture::OOP) ? 0 : 1;
        if (ImGui::RadioButton("OOP Mode", &archChoice, 0)) {
            state.selectedArch = SimArchitecture::OOP;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("DOD Mode", &archChoice, 1)) {
            state.selectedArch = SimArchitecture::DOD;
        }

        ImGui::Separator();

        ImGui::Text("Simulation Controls:");
        if (ImGui::SliderInt("Entities", &state.entityCount,
                            static_cast<int>(Config::MIN_ENTITIES),
                            static_cast<int>(Config::MAX_ENTITIES))) {
            if (activeSim) {
                activeSim->setEntityCount(static_cast<size_t>(state.entityCount));
            }
        }

        ImGui::SliderFloat("Speed Scale", &state.simSpeed,
                           Config::MIN_SIM_SPEED, Config::MAX_SIM_SPEED, "%.2fx");
        ImGui::Checkbox("Pause Simulation", &state.isPaused);
        if (ImGui::Checkbox("Ball Collisions", &state.enableCollisions)) {
            if (auto* oopSim = dynamic_cast<OOPSimulation*>(activeSim)) {
                oopSim->setCollisionsEnabled(state.enableCollisions);
            }
        }

        ImGui::Separator();

        ImGui::Text("Telemetry:");
        float dt = GetFrameTime();
        ImGui::Text("FPS: %d (%.2f ms)", GetFPS(), dt * 1000.0f);

        if (activeSim) {
            ImGui::Text("Active UFOs: %zu", activeSim->getEntityCount());

            size_t bytes = activeSim->getTheoreticalMemoryBytes();
            float megabytes = static_cast<float>(bytes) / (1024.0f * 1024.0f);
            ImGui::Text("Theoretical RAM: %.2f MB (%zu bytes)", megabytes, bytes);
        }
    }
    ImGui::End();

    rlImGuiEnd();
}
