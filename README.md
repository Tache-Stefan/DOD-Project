# Alien Invasion — OOP vs. DOD Benchmark

A high-performance 2D benchmark simulating up to **100,000 interactive UFO drones** in real time, comparing classical **Object-Oriented Programming (OOP)** against **Data-Oriented Design (DOD)** in C++20 using [Raylib](https://www.raylib.com/) and [Dear ImGui](https://github.com/ocornut/imgui).

---

## Overview

The primary goal of this project is to explore and compare how data layout affects CPU cache efficiency and runtime performance:
- **OOP (Array of Structures):** Organizes simulation state using a traditional class hierarchy (`Actor` → `SpaceCraft` → `Ufo`) with virtual methods and dynamic dispatch.
- **DOD (Structure of Arrays):** Flattens entity data into contiguous linear memory buffers, maximizing L1/L2 cache locality and memory throughput.

Both architectures run against the exact same physics and spatial hashing algorithms, allowing a direct, side-by-side performance comparison.

---

## Team Roles & Contributions

| Member | Primary Focus | Custom Feature |
| :--- | :--- | :--- |
| **Maria** | **OOP Architecture & UI/UX** | **Color Race** |
| **Stefan** | **DOD Architecture & Collisions** | **Grid View in UI** |

---

## Key Features

- **Live Architecture Switch:** Instantly toggle between OOP and DOD modes at runtime with a single click.
- **Entity Population Scaler:** Dynamic slider scaling active UFOs from **100** up to **100,000** entities in real time.
- **2D Collision Engine:** Full screen boundary rebounds and circle-to-circle elastic collision resolution powered by an efficient **2D Spatial Hash Grid** ($O(N)$ candidate pair generation).
- **Simulation Controls:** Adjust simulation speed (0.0x – 3.0x), pause/resume execution, and toggle collisions on the fly.
- **Live Telemetry:** Real-time display of FPS, frame time (ms), active UFO counts, and theoretical RAM footprint.

### Custom Features

- **Color Race (Maria):** An interactive 4-team minigame (Red, Blue, Green, Yellow). When UFOs collide, they convert each other based on team population probabilities. The UI tracks real-time progress bars and team percentages until a winning team conquers the arena.
- **Grid View in UI (Stefan):** A visual debug overlay for the 2D Spatial Hash Grid. Toggling this displays the spatial grid lines and highlights actively occupied cells in real time, along with cell size telemetry in the control panel.

---

## Architecture Comparison

| Aspect | Object-Oriented Programming (OOP) | Data-Oriented Design (DOD) |
| :--- | :--- | :--- |
| **Data Layout** | Array of Structures (AoS) via `std::vector<std::unique_ptr<Actor>>` | Structure of Arrays (SoA) via contiguous parallel vectors (`posX`, `posY`, `velX`, etc.) |
| **Dispatch** | Virtual method dispatch (`update()`, `render()`) | Linear, sequential system loops |
| **Cache Locality** | Lower due to heap fragmentation and pointer indirection | High cache line utilization and sequential memory access |

---

## Build & Run Instructions

### Prerequisites

- **CMake** (>= 3.20)
- **C++20 compliant compiler**
- **Git** (dependencies like Raylib, Dear ImGui, and rlImGui are fetched automatically via CMake `FetchContent`)

#### Linux Dependencies:
```bash
sudo apt update
sudo apt install -y cmake build-essential git \
    libasound2-dev libx11-dev libxrandr-dev libxi-dev \
    libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
    libwayland-dev libxkbcommon-dev
```

### Build

```bash
# Configure the build directory (Release mode recommended for benchmark performance)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Compile the executable
cmake --build build -j$(nproc)
```

### Run

```bash
./build/AlienInvasion
```
