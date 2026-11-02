#include "UIManager.hpp"
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include <cstdio>

void UIManager::init() {
    rlImGuiSetup(true);
}

void UIManager::shutdown() {
    rlImGuiShutdown();
}

void UIManager::render(UIState& state, ISimulation* activeSim) {
    rlImGuiBegin();

    ImGui::SetNextWindowPos(ImVec2(15.0f, 15.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(380.0f, 530.0f), ImGuiCond_FirstUseEver);

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
            if (activeSim) {
                activeSim->setCollisionsEnabled(state.enableCollisions);
            }
        }
        ImGui::Checkbox("Show Spatial Grid", &state.showSpatialGrid);

        ImGui::Separator();

        ImGui::Text("Color Race (4-Team):");
        if (activeSim) {
            if (!activeSim->isRaceActive()) {
                ImGui::TextDisabled("Status: Normal");
                if (ImGui::Button("Start 4-Team Color Race", ImVec2(-1.0f, 28.0f))) {
                    activeSim->startRace();
                }
            } else {
                auto counts = activeSim->getTeamCounts();
                size_t total = activeSim->getEntityCount();
                int winningTeam = -1;

                for (size_t t = 0; t < Config::NUM_TEAMS; ++t) {
                    const auto& team = Config::TEAMS[t];
                    float fraction = (total > 0) ? (static_cast<float>(counts[t]) / static_cast<float>(total)) : 0.0f;
                    float pct = fraction * 100.0f;

                    ImVec4 teamColorVec(team.color.r / 255.0f, team.color.g / 255.0f, team.color.b / 255.0f, 1.0f);
                    ImGui::TextColored(teamColorVec, "%s: %zu (%.1f%%)", team.name, counts[t], pct);

                    char overlay[32];
                    snprintf(overlay, sizeof(overlay), "%.1f%%", pct);
                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, teamColorVec);
                    ImGui::ProgressBar(fraction, ImVec2(-1.0f, 14.0f), overlay);
                    ImGui::PopStyleColor();

                    if (total > 0 && counts[t] == total) {
                        winningTeam = static_cast<int>(t);
                    }
                }

                if (winningTeam >= 0) {
                    const auto& winner = Config::TEAMS[winningTeam];
                    ImVec4 winColorVec(winner.color.r / 255.0f, winner.color.g / 255.0f, winner.color.b / 255.0f, 1.0f);
                    ImGui::Spacing();
                    ImGui::TextColored(winColorVec, ">>> %s WON THE RACE! <<<", winner.name);
                }

                ImGui::Spacing();
                float halfWidth = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
                if (ImGui::Button("Restart Race", ImVec2(halfWidth, 26.0f))) {
                    activeSim->startRace();
                }
                ImGui::SameLine();
                if (ImGui::Button("Reset to Normal", ImVec2(halfWidth, 26.0f))) {
                    activeSim->stopRace();
                }
            }
        }

        ImGui::Separator();

        ImGui::Text("Telemetry:");
        float dt = GetFrameTime();
        ImGui::Text("FPS: %d (%.2f ms)", GetFPS(), dt * 1000.0f);

        if (state.showSpatialGrid) {
            ImGui::TextColored(ImVec4(0.2f, 0.85f, 1.0f, 1.0f), "Grid Cell: %.1f px", Config::GRID_CELL_SIZE);
        }

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
