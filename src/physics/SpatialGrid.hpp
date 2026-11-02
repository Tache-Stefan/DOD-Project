#pragma once

#include "core/Config.hpp"
#include <vector>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <cmath>

class SpatialGrid {
public:
    SpatialGrid() = default;

    SpatialGrid(float worldWidth, float worldHeight, float cellSize = Config::GRID_CELL_SIZE) {
        init(worldWidth, worldHeight, cellSize);
    }

    void init(float worldWidth, float worldHeight, float cellSize = Config::GRID_CELL_SIZE) {
        m_cellSize = (cellSize > 0.0f) ? cellSize : Config::GRID_CELL_SIZE;
        m_worldWidth = std::max(1.0f, worldWidth);
        m_worldHeight = std::max(1.0f, worldHeight);

        m_cols = std::max(1, static_cast<int>(std::ceil(m_worldWidth / m_cellSize)));
        m_rows = std::max(1, static_cast<int>(std::ceil(m_worldHeight / m_cellSize)));

        m_head.assign(static_cast<size_t>(m_cols * m_rows), -1);
    }

    void setWorldBounds(float worldWidth, float worldHeight) {
        init(worldWidth, worldHeight, m_cellSize);
    }

    void clear(size_t entityCount) {
        const size_t totalCells = static_cast<size_t>(m_cols * m_rows);
        if (m_head.size() != totalCells) {
            m_head.assign(totalCells, -1);
        } else {
            std::fill(m_head.begin(), m_head.end(), -1);
        }

        if (m_next.size() < entityCount) {
            m_next.resize(entityCount, -1);
        }
    }

    void insert(uint32_t entityIndex, float x, float y) {
        int cx = static_cast<int>(x / m_cellSize);
        int cy = static_cast<int>(y / m_cellSize);

        if (cx < 0) cx = 0;
        else if (cx >= m_cols) cx = m_cols - 1;

        if (cy < 0) cy = 0;
        else if (cy >= m_rows) cy = m_rows - 1;

        const int cellIdx = cy * m_cols + cx;
        m_next[entityIndex] = m_head[cellIdx];
        m_head[cellIdx] = static_cast<int32_t>(entityIndex);
    }

    template <typename Callback>
    void forEachCandidatePair(Callback&& callback) const {
        constexpr int forwardOffsets[4][2] = {
            { 1,  0},
            {-1,  1},
            { 0,  1},
            { 1,  1}
        };

        for (int cy = 0; cy < m_rows; ++cy) {
            for (int cx = 0; cx < m_cols; ++cx) {
                const int cellIdx = cy * m_cols + cx;
                const int32_t headA = m_head[cellIdx];
                if (headA == -1) continue;

                for (int32_t i = headA; i != -1; i = m_next[i]) {
                    for (int32_t j = m_next[i]; j != -1; j = m_next[j]) {
                        callback(static_cast<uint32_t>(i), static_cast<uint32_t>(j));
                    }
                }

                for (const auto& offset : forwardOffsets) {
                    const int ncx = cx + offset[0];
                    const int ncy = cy + offset[1];

                    if (ncx >= 0 && ncx < m_cols && ncy >= 0 && ncy < m_rows) {
                        const int neighborIdx = ncy * m_cols + ncx;
                        const int32_t headB = m_head[neighborIdx];
                        if (headB == -1) continue;

                        for (int32_t i = headA; i != -1; i = m_next[i]) {
                            for (int32_t j = headB; j != -1; j = m_next[j]) {
                                callback(static_cast<uint32_t>(i), static_cast<uint32_t>(j));
                            }
                        }
                    }
                }
            }
        }
    }

    [[nodiscard]] float getCellSize() const noexcept { return m_cellSize; }
    [[nodiscard]] int getCols() const noexcept { return m_cols; }
    [[nodiscard]] int getRows() const noexcept { return m_rows; }
    [[nodiscard]] size_t getTotalCells() const noexcept { return static_cast<size_t>(m_cols * m_rows); }
    [[nodiscard]] size_t getMemoryBytes() const noexcept {
        return (m_head.capacity() * sizeof(int32_t)) + (m_next.capacity() * sizeof(int32_t)) + sizeof(*this);
    }

private:
    float m_cellSize = Config::GRID_CELL_SIZE;
    float m_worldWidth = 0.0f;
    float m_worldHeight = 0.0f;
    int m_cols = 0;
    int m_rows = 0;

    std::vector<int32_t> m_head;
    std::vector<int32_t> m_next;
};
