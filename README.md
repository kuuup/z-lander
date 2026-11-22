<!--
  NOTE: This document and parts of the build/platform integration were created with AI assistance.
  Exact file-level attributions are documented in the AI Contributions & Attribution section below.
-->

# Z-Lander

#### Video Demo: https://youtu.be/l1OoY_5gSZY

#### Description:

**Z-Lander** is a modular, portable Lunar Lander arcade game written in **C99** for my CS50x final project. The core design goal is to run the **exact same hardware-agnostic game engine** on two very different platforms:

1. **Desktop Linux:** Rendered with [Raylib](https://www.raylib.com/) into a 4× pixel-perfect scaled window for fast development and testing.
2. **Embedded Keyboard Hardware:** Running natively as a Zephyr/ZMK firmware module on the **68×160 monochrome Sharp Memory-in-Pixel (MiP) display** of a wireless [Keebart Piantor Pro BT](https://www.keebart.com/de/produkte/piantor-kabellos) split keyboard (nRF52840 / ARM Cortex-M4F) at 30 FPS.

You pilot a lunar lander descending toward a procedurally generated moonscape. Using a main vertical thruster and left/right horizontal thrusters—each consuming limited fuel—you must land the 9-pixel-wide ship on the flat landing pad at a safe velocity (`|vx| <= 0.3`, `|vy| <= 0.3`). Landing too fast, hitting sloped terrain, or drifting off-screen triggers a crash and explosion sprite; a soft touchdown on the pad wins the round, and pressing Start/Reset generates a new terrain.

---

## Project Structure & File Breakdown

```text
.
├── include/
│   ├── game.h              # Screen dimensions, Pixel/GameState structs, game_init/game_tick API
│   ├── ship.h              # Ship, explosion, and thruster flame sprite accessors
│   ├── terrain.h           # Procedural terrain generation and height-map lookup API
│   ├── render.h            # Platform-agnostic rendering and input contract
│   └── zmk_zlander.h       # Public entry point to attach Z-Lander to a ZMK display screen
├── src/
│   ├── game.c              # Core game tick, physics, fuel, collision, and state machine (pure C99)
│   ├── ship.c              # Relative pixel-art definitions for ship, explosion, and engine flames
│   ├── terrain.c           # Parabolic height-map generator with randomized landing pad
│   └── platform/
│       ├── desktop/
│       │   ├── main.c      # Linux entry point, RNG seeding, and while(game_tick()) loop
│       │   └── render.c    # Raylib backend (4x RenderTexture2D scaling & keyboard input)
│       └── zmk/
│           └── render.c    # Zephyr/ZMK backend (direct display_write, 3x5 font, ZMK events)
├── zephyr/module.yml       # Zephyr module manifest for west.yml integration
├── CMakeLists.txt          # Zephyr/ZMK build configuration (core + ZMK backend)
├── Kconfig                 # CONFIG_Z_LANDER option (restricted to central display builds)
├── Makefile                # GNU Makefile for Linux desktop builds (core + desktop backend)
├── install-deps.sh         # Helper script to install Raylib system dependencies (apt/dnf)
└── README.md               # Project documentation
```

### Core Engine (`src/*.c`, `include/*.h`)
- **`game.h` / `game.c`:** Defines the `68×160` playfield, `GameState`, physics (gravity, thrust, fuel consumption), collision/landing validation across the ship's 9-pixel width, and the `game_init()` / `game_tick()` lifecycle.
- **`ship.h` / `ship.c`:** Stores relative pixel-art coordinates for the ship, explosion, and thruster flames, plus helpers that translate them to screen coordinates.
- **`terrain.h` / `terrain.c`:** Generates a 1D height map (`terrain[SCREEN_WIDTH]`) with a randomly placed `13`-pixel flat landing pad connected to both screen edges via parabolic curves (`eval_parabola`).
- **`render.h`:** Defines the strict interface between the core engine and any platform backend (input polling, frame lifecycle, sprite/terrain drawing, telemetry, and text).

### Platform Backends & Build Files
- **`src/platform/desktop/main.c` & `render.c`:** Desktop Linux entry point (`srand`, `while (game_tick())`) and Raylib renderer using a `68×160` `RenderTexture2D` scaled 4× with nearest-neighbor filtering (`TEXTURE_FILTER_POINT`).
- **`src/platform/zmk/render.c` & `include/zmk_zlander.h`:** Embedded backend for ZMK v0.3. Listens to ZMK layer and key-position events, renders directly to a 1-bit framebuffer with 90° rotation and a compact 3×5 bitmap font, and drives `game_tick()` at 30 FPS via a Zephyr `k_work_delayable`.
- **`Makefile`, `install-deps.sh`, `CMakeLists.txt`, `Kconfig`, `zephyr/module.yml`:** Build and module files that compile the unmodified core with either the desktop backend (GCC + Raylib) or the ZMK backend (Zephyr/ARM).

---

## Key Design Choices

### 1. Link-Time Polymorphism via `render.h`
Instead of mixing desktop and embedded code with `#ifdef` blocks, the core engine (`src/*.c`) depends only on `render.h`. Each target provides its own implementation (`src/platform/desktop/render.c` vs. `src/platform/zmk/render.c`), and the build system (`Makefile` vs. `CMakeLists.txt`) selects which one is compiled and linked. Neither platform ever sees the other's libraries, and missing interface functions are caught at link time.

### 2. Inversion of Control (`game_init()` / `game_tick()`)
An early version ran a blocking `do-while` loop inside `game_init()`. Because a blocking loop in Zephyr RTOS would stall Bluetooth, USB, and key scanning, the loop was moved out of `game.c`: `game_init()` handles setup and `game_tick()` executes a single frame. On Linux, `main.c` drives `while (game_tick())`; on ZMK, a non-blocking `k_work_delayable` triggers `game_tick()` every 33 ms on `zmk_display_work_q()`.

### 3. Pixel-Accurate Desktop Simulation (`RenderTexture2D`)
Rather than scaling every coordinate manually for modern monitors, the desktop backend renders into a native `68×160` `RenderTexture2D` and scales it 4× to `272×640` with `TEXTURE_FILTER_POINT`. This keeps all drawing code in true hardware coordinates and matches the exact pixel grid of the keyboard display.

### 4. ZMK Integration as a Swap-In Backend
Because the core engine is completely decoupled from Raylib, porting Z-Lander to the Piantor Pro BT only required implementing `render.h` for ZMK (`src/platform/zmk/render.c`) and adding Zephyr module metadata. The ZMK backend (implemented with AI assistance) isolates game inputs on a dedicated Layer 4 (`&none` bindings) and writes directly to the Sharp MiP display for smooth 30 FPS playback, restoring the normal status screen when leaving the layer.

---

## Controls

| Action | Desktop (Raylib) | Piantor Pro BT (ZMK Layer 4) |
| :--- | :--- | :--- |
| **Enter / Exit Game** | Launch / `ESC` | Layer 3 + Top-Left (`ESC`) to enter; Top-Left (`ESC`) to exit |
| **Start / Reset Round** | `ENTER` | `SPACE` (left thumb) or `ENTER` (right thumb) |
| **Main Thruster (Up)** | `UP Arrow` | `S` |
| **Left / Right Thruster** | `LEFT` / `RIGHT Arrow` | `A` / `D` |
| **Invert Colors (Dark/Light)** | — | `Q` (key right of `ESC`) |

---

## Quickstart

### Desktop Linux
```bash
chmod +x install-deps.sh
./install-deps.sh   # Install Raylib system dependencies (apt/dnf)
make                # Clones & builds Raylib, then compiles z-lander
./z-lander
```
Use `make clean` to remove build artifacts or `make clean-all` to also remove `external/raylib`.

### ZMK Firmware Module (Piantor Pro BT)
1. Add this repository as a Zephyr module in `zmk-config/config/west.yml`.
2. Enable `CONFIG_Z_LANDER=y` in `config/piantor_pro_bt.conf`.
3. Call `zmk_zlander_init(screen)` inside `custom_status_screen.c` (guarded by `#if IS_ENABLED(CONFIG_Z_LANDER)`).
4. Map `&tog 4` on Layer 3 and configure Layer 4 (`Z-LANDER`) with `&tog 4` on exit and `&none` on all other keys.

---

## AI Contributions & Attribution

In accordance with CS50's Academic Honesty guidelines, AI assistance (Google Gemini) was used as a sparring partner for architecture review, build automation, ZMK/Zephyr integration, and documentation. AI-assisted code blocks are also marked inline (`// AI-assisted:` or `// generated with AI`).

| File / Component | AI Assisted | Human Written / Edited | Notes |
| :--- | :---: | :---: | :--- |
| `src/game.c`, `include/game.h` | ❌ No | 👤 Yes | Core physics, state machine, collision/landing logic, `game_tick` loop design. |
| `src/ship.c`, `include/ship.h` | ❌ No | 👤 Yes | Ship, explosion, and thruster pixel-art definitions and coordinate accessors. |
| `src/terrain.c`, `include/terrain.h` | 🤖 Partial | 👤 Yes | Procedural terrain & pad logic human-written; `eval_parabola()` AI-generated. |
| `src/platform/desktop/main.c` | ❌ No | 👤 Yes | Desktop entry point and `while (game_tick())` driver loop. |
| `src/platform/desktop/render.c`, `include/render.h` | 🤖 Partial | 👤 Yes | Input & drawing human-written; 4× `RenderTexture2D` scaling AI-assisted. |
| `src/platform/zmk/render.c`, `include/zmk_zlander.h` | 🤖 Yes | 👤 Review / Hardware Test | Direct `display_write()` 1-bit framebuffer, 3×5 font, ZMK event & timer bridge. |
| `zephyr/module.yml`, `CMakeLists.txt`, `Kconfig` | 🤖 Yes | 👤 Review | Zephyr/ZMK module build integration. |
| `Makefile`, `install-deps.sh` | 🤖 Yes | 👤 Partial | Desktop build automation, header dependencies, and Raylib setup. |
| `README.md` | 🤖 Yes | 👤 Review | CS50x project documentation. |
