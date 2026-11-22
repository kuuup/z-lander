<!--
  NOTE: This document and parts of the build/platform integration were created with AI assistance.
  Exact file-level attributions are documented in the AI Contributions & Attribution section below.
-->

# Z-Lander

#### Video Demo: <URL HERE>

#### Description:

**Z-Lander** is a modular, portable Lunar Lander arcade game written in **C99**, built as my CS50x final project. Its defining goal is to run the **exact same hardware-agnostic game engine** on two completely different targets:

1. **Desktop Linux (Prototype & Development Target):** Rendered with [Raylib](https://www.raylib.com/) into a 4× pixel-perfect scaled window for fast iteration and debugging.
2. **Embedded Keyboard Hardware (Final Target):** Running natively as a Zephyr/ZMK firmware module directly on the **68×160 monochrome Sharp Memory-in-Pixel (MiP) display** of a wireless [Keebart Piantor Pro BT](https://www.keebart.com/de/produkte/piantor-kabellos) split ergonomic keyboard (nRF52840 / ARM Cortex-M4F) at a smooth 30 FPS.

In the game, you pilot a lunar lander descending toward a procedurally generated moonscape. Using a main vertical thruster and left/right horizontal thrusters—each consuming limited fuel—you must guide the 9-pixel-wide spacecraft onto the flat landing pad at a safe horizontal and vertical velocity (`|vx| <= 0.3`, `|vy| <= 0.3`). Touching down too fast, hitting sloped terrain, or drifting off-screen results in a crash with an explosion sprite; a soft touchdown on the flat pad wins the round, and pressing Start/Reset immediately generates a new terrain for the next attempt.

---

## Project Structure & File Breakdown

```text
.
├── include/
│   ├── game.h              # Core screen dimensions, Pixel/GameState structs, game_init/game_tick API
│   ├── ship.h              # Ship, explosion, and thruster flame sprite accessors
│   ├── terrain.h           # Procedural terrain generation and height-map lookup API
│   ├── render.h            # Platform-agnostic rendering and input contract
│   └── zmk_zlander.h       # Public entry point to attach the Z-Lander widget to a ZMK screen
├── src/
│   ├── game.c              # Core game loop tick, physics, fuel, collision, and state machine (pure C99)
│   ├── ship.c              # Relative pixel-art definitions for ship, explosion, and engine flames
│   ├── terrain.c           # Parabolic height-map generator with randomized landing pad
│   └── platform/
│       ├── desktop/
│       │   ├── main.c      # Linux entry point, RNG seeding, and desktop while(game_tick()) loop
│       │   └── render.c    # Raylib implementation of render.h (4x RenderTexture2D scaling & keyboard input)
│       └── zmk/
│           └── render.c    # Zephyr/ZMK implementation of render.h (direct display_write, 3x5 font, ZMK events)
├── zephyr/
│   └── module.yml          # Zephyr module manifest for west.yml integration
├── CMakeLists.txt          # Zephyr/ZMK build configuration (selects core + ZMK platform)
├── Kconfig                 # Kconfig option (CONFIG_Z_LANDER) restricted to central display builds
├── Makefile                # GNU Makefile for Linux desktop builds (selects core + desktop platform)
├── install-deps.sh         # Helper script to install Raylib system dependencies on apt/dnf Linux distros
└── README.md               # Project documentation
```

### Core Engine (100% Platform-Independent C99)

- **`include/game.h` & `src/game.c`:** Defines the `68×160` logical playfield, the `Pixel` and `GameState` structs (position, `vx`/`vy`, fuel, running/collision/landed flags), and the game logic. `game_init()` resets the state and initializes the renderer, while `game_tick()` executes a single frame: starting the frame, polling thruster inputs, applying gravity and fuel burn, updating coordinates, checking boundary/terrain collisions and landing velocity thresholds, invoking rendering callbacks, and returning whether the game should continue.
- **`include/ship.h` & `src/ship.c`:** Stores all spacecraft sprites as relative pixel-offset arrays around the ship's center-bottom landing reference point: the main lander (`ship_pixels`), the crash debris (`explosion_pixels`), and the three thruster plumes (`main_flame_pixels`, `left_flame_pixels`, `right_flame_pixels`). Helper functions translate each relative offset into absolute screen coordinates.
- **`include/terrain.h` & `src/terrain.c`:** Maintains a 1D height map (`float terrain[SCREEN_WIDTH]`). Each round places a flat `13`-pixel-wide landing pad at a random horizontal position and elevation, then smoothly connects the left and right screen edges to the pad using parabolic curves (`eval_parabola`). `get_terrain_height(x)` provides bounds-checked height queries for both rendering and collision detection.
- **`include/render.h`:** Acts as the strict interface contract between the core engine and any platform backend. It declares five input polling functions (`key_left_pressed`, `key_right_pressed`, `key_main_pressed`, `key_reset_pressed`, `key_exit_pressed`) and the drawing/lifecycle hooks (`init_renderer`, `begin_frame`, `end_frame`, `render_ship`, `render_*_thruster`, `render_terrain`, `render_telemetry`, `render_text`, `render`).

### Desktop Platform (`src/platform/desktop/`)

- **`src/platform/desktop/main.c`:** Seeds `rand()` with `time(NULL)`, calls `game_init()`, and drives the game with a simple `while (game_tick())` loop.
- **`src/platform/desktop/render.c`:** Implements `render.h` using Raylib. It maps arrow keys, `ENTER`, and `ESCAPE` to the input interface, renders all game pixels, telemetry, and text onto an off-screen `68×160` `RenderTexture2D`, and upscales that texture 4× with nearest-neighbor filtering (`TEXTURE_FILTER_POINT`) to a `272×640` window at 30 FPS.

### Embedded ZMK Platform (`src/platform/zmk/` & Zephyr Module Files)

- **`include/zmk_zlander.h` & `src/platform/zmk/render.c`:** Implements `render.h` for the Piantor Pro BT's Sharp MiP display (`sharp,ls0xx`) and ZMK v0.3 firmware. It listens to ZMK's `zmk_layer_state_changed` event to start/stop the game when dedicated Layer 4 is toggled, listens to `zmk_position_state_changed` for physical key presses (`A`/`S`/`D`/`Space`/`Enter`/`Q`), renders directly into a packed 1,360-byte 1-bit framebuffer with 90° portrait-to-landscape rotation, draws telemetry and menu text using a compact built-in 3×5 bitmap font, and drives `game_tick()` at 30 FPS via a Zephyr `k_work_delayable` scheduled on `zmk_display_work_q()`.
- **`zephyr/module.yml`, `Kconfig`, `CMakeLists.txt`:** Turn this repository into a standalone Zephyr/ZMK module. When `CONFIG_Z_LANDER=y` is enabled on a central split half with a display, CMake compiles the unmodified core (`src/game.c`, `src/ship.c`, `src/terrain.c`) together with `src/platform/zmk/render.c` directly into the ZMK firmware image.
- **`Makefile` & `install-deps.sh`:** Automate Linux dependency installation, cloning/building Raylib into `external/raylib`, and compiling the core + desktop platform with `-Wall -Wextra -Wstrict-prototypes -std=c99`.

---

## Key Design Choices

### 1. Link-Time Polymorphism via `render.h`
Rather than littering the codebase with `#ifdef __ZEPHYR__` preprocessor branches, the project separates the hardware-agnostic core (`src/*.c`) from platform implementations (`src/platform/desktop/` and `src/platform/zmk/`). Both backends implement the exact same `render.h` contract, and the build system (`Makefile` for Linux, `CMakeLists.txt` for ZMK) selects which platform files are compiled and linked. As a result, the desktop compiler never sees Zephyr/LVGL headers, the ARM cross-compiler never sees Raylib, and any missing interface function is caught immediately at link time.

### 2. Inversion of Control (`game_init()` / `game_tick()`)
An early desktop version kept a blocking `do-while` loop inside `game_init()`. While that works in a standalone Linux process, a blocking loop in Zephyr RTOS would freeze Bluetooth, USB HID, and key-matrix scanning. Splitting initialization (`game_init()`) from single-frame execution (`game_tick()`) hands loop control to the host platform: `main.c` runs `while (game_tick())` on Linux, whereas ZMK schedules a non-blocking `k_work_delayable` every 33 ms on the dedicated display work queue.

### 3. Pixel-Accurate Desktop Simulation (`RenderTexture2D`)
Because the target Sharp MiP display is only `68×160` pixels, opening a native `68×160` desktop window is tiny on modern monitors. Instead of multiplying every coordinate by a scale factor, the desktop renderer draws into a `68×160` off-screen `RenderTexture2D` and blits it 4× larger with `TEXTURE_FILTER_POINT`. This keeps all drawing logic in true hardware coordinates and previews the exact pixel rasterization seen on the keyboard.

### 4. Embedded Performance: From LVGL Canvas to Direct `display_write()`
The Piantor Pro BT's Sharp MiP display is physically mounted vertically (`68×160`) but addressed horizontally (`160×68`) by Zephyr's `ls0xx` driver. The first ZMK prototype rendered into a 10.8 KB LVGL canvas (`LV_IMG_CF_TRUE_COLOR`) refreshed via `lv_task_handler()`, which only achieved ~12–15 FPS due to LVGL's software rendering overhead and 1 MHz SPI transfers.

Replacing the LVGL canvas during gameplay with a **packed 1,360-byte 1-bit framebuffer** (`160 × 68 / 8` bytes—8× less RAM) written directly via Zephyr's `display_write()`—combined with 90° coordinate rotation in `draw_pixel()`, frame-time-compensated 33 ms scheduling, and raising the Sharp MiP SPI clock from 1 MHz to 2 MHz (within the LS013B7DH03 datasheet spec)—eliminated LVGL overhead and brought the hardware version up to a smooth, responsive **30 FPS**. On exiting the game layer, invalidating the root LVGL screen seamlessly restores the normal ZMK status screen.

### 5. Zero-Dependency Embedded Text & Host-Safe Input
ZMK builds Zephyr with `PICOLIBC_INTEGER_PRINTF_SCANF`, meaning standard `snprintf("%.2f", ...)` does not format floating-point numbers. To keep firmware size small and avoid rotating LVGL text, the ZMK backend includes a tiny 3×5 bitmap font and an integer-only fixed-point formatter (`format_fixed2`) for `vx`/`vy` telemetry. For input, Z-Lander runs on a dedicated ZMK layer (Layer 4) filled with `&none` bindings so gameplay keystrokes are never sent to the connected computer, while `zmk_position_state_changed` still captures `A`/`S`/`D`/`Space`/`Enter` and a live Dark/Light display inversion toggle on `Q` (right next to `ESC`).

---

## Controls

| Action | Desktop (Raylib) | Piantor Pro BT (ZMK Layer 4) |
| :--- | :--- | :--- |
| **Enter / Exit Game** | Launch / `ESC` | Layer 3 + Top-Left (`ESC`) to enter; Top-Left (`ESC`) to exit |
| **Start / Reset Round** | `ENTER` | `SPACE` (left thumb) or `ENTER` (right thumb) |
| **Main Thruster (Up)** | `UP Arrow` | `S` |
| **Left Thruster (Move Left)** | `LEFT Arrow` | `A` |
| **Right Thruster (Move Right)** | `RIGHT Arrow` | `D` |
| **Invert Colors (Dark/Light)** | — | `Q` (key right of `ESC`) |

---

## Quickstart

### Desktop Linux

1. **Install system dependencies** (Debian/Ubuntu `apt` or Fedora `dnf`):
   ```bash
   chmod +x install-deps.sh
   ./install-deps.sh
   ```
2. **Build and run** (`make` automatically clones and compiles Raylib as a static library):
   ```bash
   make
   ./z-lander
   ```
3. **Clean up:**
   - `make clean`: Removes `build/` and the `z-lander` binary.
   - `make clean-all`: Also removes the cloned `external/raylib` directory.

### ZMK Firmware Integration (Piantor Pro BT)

1. Add this repository as a Zephyr module in your `zmk-config/config/west.yml`.
2. Enable `CONFIG_Z_LANDER=y` in your board `.conf` (e.g. `config/piantor_pro_bt.conf`).
3. Call `zmk_zlander_init(screen)` inside your custom status screen (`#if IS_ENABLED(CONFIG_Z_LANDER)`).
4. Add a `&tog 4` binding to enter/exit Layer 4 (`Z-LANDER`) filled with `&none` bindings so game inputs are isolated from the host.

---

## AI Contributions & Attribution

In accordance with CS50's Academic Honesty guidelines, AI assistance (Google Gemini) was used as a sparring partner for architecture review, build automation, embedded ZMK/Zephyr integration, and documentation. All AI-assisted sections in C source files are also marked inline (`// AI-assisted:` or `// generated with AI`).

| File / Component | AI Assisted | Human Written / Edited | Notes |
| :--- | :---: | :---: | :--- |
| `src/game.c`, `include/game.h` | ❌ No | 👤 Yes | Core physics, state machine, collision/landing logic, `game_tick` loop design. |
| `src/ship.c`, `include/ship.h` | ❌ No | 👤 Yes | Ship, explosion, and thruster pixel-art definitions and coordinate accessors. |
| `src/terrain.c`, `include/terrain.h` | 🤖 Partial | 👤 Yes | Procedural terrain & pad logic human-written; `eval_parabola()` helper AI-generated. |
| `src/platform/desktop/main.c` | ❌ No | 👤 Yes | Desktop entry point and `while (game_tick())` driver loop. |
| `src/platform/desktop/render.c`, `include/render.h` | 🤖 Partial | 👤 Yes | Input & drawing functions human-written; 4× `RenderTexture2D` scaling (`init_renderer`, `begin_frame`, `end_frame`) AI-assisted. |
| `src/platform/zmk/render.c`, `include/zmk_zlander.h` | 🤖 Yes | 👤 Review / Hardware Test | Zephyr `display_write()` 1-bit framebuffer, 3×5 font, ZMK event listeners, and timer bridge. |
| `zephyr/module.yml`, `CMakeLists.txt`, `Kconfig` | 🤖 Yes | 👤 Review | Zephyr/ZMK module build integration. |
| `Makefile`, `install-deps.sh` | 🤖 Yes | 👤 Partial | Desktop build automation, header dependencies, and Raylib setup. |
| `README.md` | 🤖 Yes | 👤 Review | CS50x project documentation and architecture summary. |
