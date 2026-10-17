<!--
  NOTE: This document and parts of the codebase were generated/structured with AI assistance.
  File/Section level attributions are documented in the AI Contributions section.
-->

# Z-Lander

A modular, portable Lunar Lander game written in C99, designed for the Piantor Pro BT keyboard (nRF52840 / ZMK Firmware) featuring an integrated display.

## 🎯 Project Goal

Development of a performant arcade classic using a strictly decoupled architecture:

- **Desktop Version:** Fast, iterative prototype running on Linux using Raylib for visualization.
- **Embedded Version (Target):** Direct port of the game logic to ZMK firmware with zero external frameworks.

## 🏗️ Architecture & Design Principles

The project enforces a strict **Linear Frame Loop** (one-way data flow) to completely eliminate circular dependencies and ensure maximum portability:

```
[ 1. Input Polling ]  --->  [ 2. Game Logic & Physics ]  --->  [ 3. Rendering ]
```

1. **Input Polling:** Key states are polled at the start of each frame and translated into neutral command structs.
2. **Game Logic & Physics:** Hardware-agnostic mathematics and state management written in pure C99. Completely decoupled from Raylib, display drivers, or windowing systems.
3. **Rendering:** Abstracted output step. Renders via Raylib on the desktop and directly via hardware display drivers on the keyboard.

### Hardware Constraints & Considerations

- **Fixed Timestep / Delta Time:** Physics calculations are synchronized using frame time deltas to maintain consistent gameplay across varying display refresh rates (e.g., maximum ~30 FPS on the keyboard display).
- **Early Hardware PoC:** Display performance and bus bandwidth (I2C/SPI) will be validated early on the target hardware to ensure physical feasibility.

## 🤖 AI Contributions & Attribution

This project utilizes AI assistance (Google Gemini) as a sparring partner for architecture design, build automation, and documentation. Below is the breakdown of AI involvement per file:

| File / Component  | AI Generated | Human Written / Edited | Description / Notes                                                 |
| :---------------- | :----------: | :--------------------: | :------------------------------------------------------------------ |
| `Makefile`        |    🤖 Yes    |       👤 Partial       | Build configuration, automatic dependency cloning/linking.          |
| `install-deps.sh` |    🤖 Yes    |       👤 Partial       | Cross-distro dependency resolution script (apt/dnf).                |
| `README.md`       |    🤖 Yes    |       👤 Review        | Project documentation, context tracking, and architecture overview. |
| `src/main.c`      |  ❌ Pending  |       👤 Planned       | Application entry point and linear frame loop execution.            |
| `src/game.c`      |  ❌ Pending  |       👤 Planned       | Core game state and physics logic (pure C99).                       |
| `src/render.c`    |  ❌ Pending  |       👤 Planned       | Raylib rendering implementation (desktop target).                   |

_Note: In source code files (`.c`, `.h`), individual functions or blocks generated with AI assistance will contain inline comments indicating AI authorship (`// AI-generated: [description]`)._

## 📂 Project Structure

```
.
├── src/            # C source files (main.c, game.c, render.c)
├── include/        # C header files (.h)
├── external/       # External dependencies (Raylib, automatically cloned/built)
├── build/          # Compiled object files
├── Makefile        # GNU Makefile for Linux desktop builds
└── install-deps.sh # System package installation script (apt/dnf support)
```

## 🚀 Quickstart (Desktop Linux)

### 1. Install System Dependencies

Run once to install the required build tools and graphics libraries (X11, OpenGL, Wayland):

```bash
chmod +x install-deps.sh
./install-deps.sh
```

### 2. Build and Run

Running `make` automatically clones and compiles Raylib as a static library, then builds the application executable:

```bash
make
./z-lander
```

### 3. Cleaning Up

- `make clean`: Removes local build artifacts (`build/`, `z-lander`).
- `make clean-all`: Removes local artifacts as well as the cloned Raylib repository under `external/`.

## 🛠️ Tech Stack & Tooling

- **Language:** C99
- **Build System:** GNU Make
- **Editor/Workflow:** Neovim / GDB
- **Target Hardware:** Piantor Pro BT (nRF52840)

---

_Disclaimer: This repository contains code and documentation authored with AI assistance. Refer to the AI Contributions table above for specific file-level provenance._
