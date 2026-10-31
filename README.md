# Alien Invasion

A high-performance benchmark simulating up to **100,000 interactive UFO drones** in 2D with collisions and screen boundary bounces.

---

## Task Breakdown & Team Roles

| Member | Primary Focus | Custom Feature |
| :--- | :--- | :--- |
| **Maria** | **OOP Architecture & UI/UX** | **TBD** |
| **Stefan** | **DOD Architecture & Collisions** | **TBD** |

---

## Architecture

* **OOP (AoS):** `Actor` -> `SpaceCraft` -> `Ufo`. Stores objects as `std::vector<std::unique_ptr<Actor>>`.
* **DOD (SoA):** Contiguous linear arrays.

---

## Features

- [] **Entity Scaler:** Real-time slider for active entities.
- [] **Live Architecture Switch:** Instant toggle between OOP and DOD to observe performance deltas.
- [] **Full Collision System:** Entity-to-entity elastic rebounds + window border bounces.
- [] **Simulation Speed Control:** Scale delta time.
- [] **Telemetry:** Live FPS counter, frame time history graph, and process RAM usage.
- [] **Custom Features:** TBD.

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
