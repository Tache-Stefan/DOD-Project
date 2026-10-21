## Task Breakdown

### Maria — OOP Architecture, UI/UX & Lifecycle Management

**Primary Focus:** Classical polymorphic object-oriented architecture, ImGui control interface, and dynamic entity lifecycle management.

- [ ] **T1.1 (Engine & UI Setup):**
  - Integrate `rlImGui` into the core Raylib loop.
  - Design the main control panel layout.
- [ ] **T1.2 (3-Level OOP Hierarchy):**
  - (Level 1): Abstract base class with `id`, lifecycle state, and pure virtual methods.
  - (Level 2): Kinematics implementation (`position`, `velocity`, `mass`, force).
  - (Level 3): Specific features.
- [ ] **T1.3 (OOP Scene Manager):**
  - Manage container storage using `std::vector<std::unique_ptr<Actor>>`.
  - Handle runtime spawning and deletion when interacting with the population slider.
- [ ] **T1.4 (Interactive UI Controls):**
  - Population slider: `100` to `100,000` entities with immediate scene reflection.
  - Architecture toggle: `OOP Mode` vs. `DOD Mode`.
  - Simulation speed slider: Delta time scale.
- [ ] **T1.5 (Custom Feature):**
  - **TBD**

---

### Stefan — DOD Architecture, Spatial Partitioning & Telemetry

**Primary Focus:** Data-Oriented Design, collision optimization and hardware resource telemetry.

- [ ] **T2.1 (Build System & Dependencies):**
  - Configure `CMakeLists.txt`for Raylib 5.0 and ImGui.
- [ ] **T2.2 (DOD Architecture - Structure of Arrays):**
  - Define contiguous linear arrays.
  - Implement linear execution systems tailored for L1/L2 CPU cache lines.
- [ ] **T2.3 (Collision Engine & Spatial Partitioning):**
  - Mathematical resolution for boundary rebounds and elastic 2D circle-to-circle impacts.
  - Implement a **2D Spatial Hash Grid**: cell-based space partitioning with $O(N)$ complexity.
  - Connect the spatial grid to both DOD and OOP workflows.
- [ ] **T2.4 (Telemetry System & OS Memory Metrics):**
  - Real-time FPS counter.
  - Fetch real OS process memory consumption.
  - Theoretical memory footprint calculation.
- [ ] **T2.5 (Custom Feature):**
  - **TBD**

## Common Integration Points

1. **Simulation Interface Abstraction:** Define a shared abstract interface `ISimulation` so the UI switch swaps architectures polymorphically without polluting `main.cpp`.
2. **Benchmarking & Final Report:** Jointly run performance benchmarks across identical hardware configurations at **1k**, **10k**, **50k**, and **100k** entities, recording comparative metrics for FPS, Frame Time, and RAM usage.
