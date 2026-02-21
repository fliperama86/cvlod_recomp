# Comprehensive Guide to Successful N64 Recompilation Projects

This document outlines the proven methods, tools, and technical requirements for creating a successful Nintendo 64 static recompilation project, based on the methodologies used in high-profile projects like **Zelda64Recomp** (Majora's Mask) and the **N64: Recompiled** (N64Recomp) toolchain.

## 1. Core Concept: Static Recompilation vs. Emulation

To succeed, it is crucial to understand the fundamental difference:
*   **Emulation:** Simulates the N64 hardware (CPU, RCP, Memory) in software. The original game code runs on this simulated hardware. It is accurate but resource-intensive and often limited to the original game's constraints (FPS, resolution).
*   **Static Recompilation:** Translates the original game's compiled MIPS machine code directly into native code (C/C++) for modern architectures (x86_64, ARM64). This allows the game to run as a native application, offering vastly superior performance, modding capabilities, and modern features.

**Success Factor:** Recompilation allows for "impossible" features on original hardware, such as arbitrary high frame rates (120+ FPS), ultra-wide resolutions, and ray tracing, without the overhead of emulation.

## 2. Essential Tools & Technologies

A successful project relies on a specific set of tools. The current "gold standard" involves:

### A. The Recompiler: N64: Recompiled (N64Recomp)
Developed by **Wiseguy**, this tool is the backbone of modern N64 ports.
*   **Function:** It takes an N64 binary (ROM/ELF) and statically analyzes it to generate equivalent C code.
*   **Output:** It produces files like `funcs.h`, `lookup.cpp`, and `recomp_entrypoint.c` which represent the game's logic in C.
*   **Key Feature:** It does *not* require a full decompilation (source code reconstruction). It works directly with the binary, though symbol maps significantly improve the result.

### B. The Renderer: RT64 (Render Traslater 64)
*   **Function:** A high-level rendering backend designed to intercept N64 graphics commands (display lists) and translate them into modern APIs (DirectX 12, Vulkan).
*   **Benefits:** Enables ray tracing, DLSS/FSR, high-resolution textures, and uncapped framerates.
*   **Integration:** The recompiled code sends graphics commands to RT64 instead of the original N64 RCP (Reality Co-Processor).

### C. Build System
*   **Compilers:** Modern compilers like MSVC (Windows), GCC, or Clang are used to compile the generated C code.
*   **Build Tools:** CMake or similar build systems are used to manage the compilation of the generated massive C codebase and link it with the runtime.

## 3. The Technical Process: Step-by-Step

Creating a port involves these distinct phases:

### Phase 1: Analysis & Preparation
1.  **ROM Acquisition:** You need a specific, compatible version of the game ROM (often the US version).
2.  **Symbol Recovery:** While not strictly required for *running*, having function names and variable names is critical for *modifying* and *fixing* the game. This data is often sourced from existing decompilation projects (like the Zelda decompilation teams).
3.  **Configuration:** Configure the N64Recomp tool (`.toml` files) to define memory regions, ignored functions, and specific handling for unique game mechanics.

### Phase 2: Recompilation
1.  **Translation:** Run N64Recomp. It effectively "lifts" the MIPS assembly into C functions.
    *   *Challenge:* Handling self-modifying code or extremely clever assembly tricks that don't map cleanly to C.
2.  **Code Generation:** The tool outputs a massive amount of C code. This code is often not human-readable but is functionally equivalent to the game logic.

### Phase 3: Runtime Implementation
This is often the hardest part. You must write a **Native Runtime** that replaces the N64's Operating System (OS).
*   **Threading:** The N64 had a unique threading model. You must map this to modern OS threads (Windows/Linux threads).
*   **Audio:** Reimplement the N64's audio interface (AI) to output sound via modern APIs (SDL2, etc.).
*   **Input:** Map modern controllers (XInput, DualSense) to N64 controller inputs.
*   **Save System:** Intercept EEPROM/FlashRAM save calls and redirect them to files on the PC disk.

### Phase 4: Enhancement & Polish
1.  **Graphics Hooks:** Patch the recompiled code to provide extra data to RT64 (e.g., identifying "this object is a light source").
2.  **FPS Unlocking:** Modify game logic (timers) so the game speed is decoupled from the frame rate. Original N64 games often tied physics to the 20FPS limit.
3.  **Widescreen Fixes:** Adjust camera field-of-view (FOV) calculations and UI element positioning to support 16:9 or 21:9 displays without stretching.

## 4. Requirements for a "Successful" Project

To reach the level of Zelda64Recomp, your project must include:

1.  **Legal Safety:** NEVER distribute the game's assets or recompiled code directly. Distribute the **executable** which requires the user to provide their own legally owned ROM. The tool should extract assets from the user's ROM at runtime or installation time.
2.  **User Experience (UX):**
    *   **Launcher:** A simple GUI to select the ROM and settings.
    *   **Controller Support:** Native support for modern gamepads (Xbox, PlayStation, Switch Pro) with correct button prompts.
    *   **High Performance:** Utilization of AVX instructions and multi-threading where possible.
3.  **Modding API:** Expose hooks or a scripting layer that allows the community to replace textures, models, or logic without recompiling the entire project.

## 5. Common Pitfalls & Challenges

*   **Floating Point Math:** N64 used specific floating-point behaviors. Modern CPUs might handle rounding differently, leading to "desyncs" or physics bugs.
*   **Timing Bugs:** N64 games relied on specific hardware timing. On a PC running at 4GHz, race conditions can occur if the emulated threading isn't precise.
*   **Microcode:** Some games used custom microcode for audio or graphics. This often needs to be manually reverse-engineered and reimplemented in C/C++, as generic recompilers might struggle with it.

## Summary Checklist

- [ ] **Target Game:** Choose a game with an existing decompilation or symbol map if possible.
- [ ] **Toolchain:** Set up N64Recomp and a C++ build environment.
- [ ] **Runtime:** Develop a "wrapper" or OS replacement for audio, input, and save data.
- [ ] **Graphics:** Integrate RT64 or write a translation layer for graphics commands.
- [ ] **Polish:** Implement high-FPS fixes and widescreen support.
- [ ] **Distribution:** Create a launcher that validates and uses the user's provided ROM.
