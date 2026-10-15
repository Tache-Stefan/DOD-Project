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
