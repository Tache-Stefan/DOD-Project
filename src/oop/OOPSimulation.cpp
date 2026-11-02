#include "OOPSimulation.hpp"
#include "core/Config.hpp"
#include <random>
#include <cmath>
 
OOPSimulation::OOPSimulation()
    : worldWidth(static_cast<float>(Config::WINDOW_WIDTH)),
      worldHeight(static_cast<float>(Config::WINDOW_HEIGHT)),
      nextEntityId(1) {}
 
void OOPSimulation::init(size_t initialEntities, float width, float height) {
    setWorldBounds(width, height);
    grid.init(worldWidth, worldHeight, Config::GRID_CELL_SIZE);
    entities.clear();
    nextEntityId = 1;
    raceActive = false;
    teamCounts.fill(0);
    setEntityCount(initialEntities);
}
 
void OOPSimulation::update(float dt) {
    for (auto& entity : entities) {
        entity->update(dt);
    }
 
    if (collisionsEnabled) {
        resolveCollisions();
    }
 
    for (auto& entity : entities) {
        entity->checkBoundaryCollision(worldWidth, worldHeight);
    }
}
 
void OOPSimulation::resolveCollisions() {
    const size_t n = entities.size();
    if (n < 2) return;
 
    std::vector<SpaceCraft*> crafts;
    crafts.reserve(n);
    for (auto& entity : entities) {
        if (auto* craft = entity->asSpaceCraft(); craft && craft->isActive()) {
            crafts.push_back(craft);
        }
    }
 
    const size_t activeCount = crafts.size();
    if (activeCount < 2) return;
 
    grid.clear(activeCount);
    for (size_t i = 0; i < activeCount; ++i) {
        grid.insert(static_cast<uint32_t>(i), crafts[i]->getPosX(), crafts[i]->getPosY());
    }
 
    grid.forEachCandidatePair([this, &crafts](uint32_t i, uint32_t j) {
        if (crafts[i]->resolveCollisionWith(*crafts[j])) {
            if (raceActive) {
                const uint8_t teamI = crafts[i]->getTeamId();
                const uint8_t teamJ = crafts[j]->getTeamId();
                if (teamI != teamJ && teamI < Config::NUM_TEAMS && teamJ < Config::NUM_TEAMS) {
                    const size_t countI = teamCounts[teamI];
                    const size_t countJ = teamCounts[teamJ];
                    const size_t total = countI + countJ;

                    static uint32_t rng = 0x853c49e6;
                    rng ^= rng << 13;
                    rng ^= rng >> 17;
                    rng ^= rng << 5;

                    if (total > 0 && (rng % total) < countI) {
                        crafts[j]->setTeam(teamI, crafts[i]->getColor());
                        if (teamCounts[teamJ] > 0) teamCounts[teamJ]--;
                        teamCounts[teamI]++;
                    } else {
                        crafts[i]->setTeam(teamJ, crafts[j]->getColor());
                        if (teamCounts[teamI] > 0) teamCounts[teamI]--;
                        teamCounts[teamJ]++;
                    }
                }
            }
        }
    });
}
 
void OOPSimulation::render() const {
    for (const auto& entity : entities) {
        entity->render();
    }
}
 
void OOPSimulation::setEntityCount(size_t count) {
    if (count == entities.size()) {
        return;
    }
 
    if (count < entities.size()) {
        entities.resize(count);
        recomputeTeamCounts();
    } else {
        spawnEntities(count - entities.size());
    }
}
 
size_t OOPSimulation::getEntityCount() const {
    return entities.size();
}
 
std::string_view OOPSimulation::getName() const {
    return "OOP";
}
 
SimArchitecture OOPSimulation::getArchitecture() const {
    return SimArchitecture::OOP;
}
 
size_t OOPSimulation::getTheoreticalMemoryBytes() const {
    constexpr size_t heapMetadataPerAlloc = 16;
    size_t vectorStorage = entities.capacity() * sizeof(std::unique_ptr<Actor>);
    size_t actorsStorage = entities.size() * (sizeof(Ufo) + heapMetadataPerAlloc);
 
    return sizeof(*this) + vectorStorage + actorsStorage;
}
 
void OOPSimulation::setWorldBounds(float width, float height) {
    worldWidth = (width > 0.0f) ? width : static_cast<float>(Config::WINDOW_WIDTH);
    worldHeight = (height > 0.0f) ? height : static_cast<float>(Config::WINDOW_HEIGHT);
    grid.setWorldBounds(worldWidth, worldHeight);
}
 
void OOPSimulation::spawnEntities(size_t count) {
    entities.reserve(entities.size() + count);
 
    std::random_device rd;
    std::mt19937 gen(rd());
 
    const float maxSpeed = Config::BALL_MAX_SPAWN_SPEED;
    std::uniform_real_distribution<float> velDist(-maxSpeed, maxSpeed);
 
    const float radius = Config::BALL_RADIUS;
    const float padding = Config::SPAWN_BORDER_PADDING;
 
    float minX = radius + padding;
    float maxX = (worldWidth > minX + 1.0f) ? (worldWidth - radius - padding) : minX;
    float minY = radius + padding;
    float maxY = (worldHeight > minY + 1.0f) ? (worldHeight - radius - padding) : minY;
 
    std::uniform_real_distribution<float> posXDist(minX, maxX);
    std::uniform_real_distribution<float> posYDist(minY, maxY);
 
    for (size_t i = 0; i < count; ++i) {
        float posX = posXDist(gen);
        float posY = posYDist(gen);
        float velX = velDist(gen);
        float velY = velDist(gen);
 
        // Ensure minimum velocity so entities don't stay still
        if (std::abs(velX) < 15.0f) velX = (velX < 0.0f) ? -25.0f : 25.0f;
        if (std::abs(velY) < 15.0f) velY = (velY < 0.0f) ? -25.0f : 25.0f;
 
        uint8_t team = 0;
        Color col = Config::COLOR_BALL_OOP;
        if (raceActive) {
            team = static_cast<uint8_t>(entities.size() % Config::NUM_TEAMS);
            col = Config::TEAMS[team].color;
        }
        if (team < Config::NUM_TEAMS) {
            teamCounts[team]++;
        }

        entities.push_back(std::make_unique<Ufo>(
            nextEntityId++,
            posX,
            posY,
            velX,
            velY,
            team,
            col,
            radius
        ));
    }
}

void OOPSimulation::startRace() {
    raceActive = true;
    teamCounts.fill(0);
    for (size_t i = 0; i < entities.size(); ++i) {
        if (auto* craft = entities[i]->asSpaceCraft()) {
            uint8_t team = static_cast<uint8_t>(i % Config::NUM_TEAMS);
            craft->setTeam(team, Config::TEAMS[team].color);
            teamCounts[team]++;
        }
    }
}

void OOPSimulation::stopRace() {
    raceActive = false;
    teamCounts.fill(0);
    for (auto& entity : entities) {
        if (auto* craft = entity->asSpaceCraft()) {
            craft->setTeam(0, Config::COLOR_BALL_OOP);
        }
    }
    teamCounts[0] = entities.size();
}

std::array<size_t, Config::NUM_TEAMS> OOPSimulation::getTeamCounts() const {
    return teamCounts;
}

void OOPSimulation::recomputeTeamCounts() {
    teamCounts.fill(0);
    for (const auto& entity : entities) {
        if (const auto* craft = entity->asSpaceCraft()) {
            uint8_t team = craft->getTeamId();
            if (team < Config::NUM_TEAMS) {
                teamCounts[team]++;
            }
        }
    }
}