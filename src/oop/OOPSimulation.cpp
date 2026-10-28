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
    entities.clear();
    nextEntityId = 1;
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
    for (size_t i = 0; i < activeCount; ++i) {
        SpaceCraft* craftA = crafts[i];
        for (size_t j = i + 1; j < activeCount; ++j) {
            craftA->resolveCollisionWith(*crafts[j]);
        }
    }
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

        entities.push_back(std::make_unique<Ufo>(
            nextEntityId++,
            posX,
            posY,
            velX,
            velY,
            radius
        ));
    }
}
