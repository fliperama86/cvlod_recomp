# Castlevania: Legacy of Darkness recompilation WIP

This repository is an in-progress recompilation/recovery project. The current goal is to reach and stabilize real gameplay while preserving enough telemetry to diagnose remaining crashes.

## Reproducible checkout for testers

Testers should pin the **top-level repository commit**, not individual submodule commits. The top-level commit records the exact submodule SHAs that should be used.

Maintainer/tester flow:

```sh
git clone --recursive https://github.com/fliperama86/cvlod_recomp.git
cd cvlod_recomp
git checkout <top-level-sha-provided-by-maintainer>
git submodule sync --recursive
git submodule update --init --recursive
git submodule status --recursive
```

If the repo was already cloned:

```sh
git fetch origin
git checkout <top-level-sha-provided-by-maintainer>
git submodule sync --recursive
git submodule update --init --recursive
git submodule status --recursive
```

Do **not** run:

```sh
git submodule update --remote
```

`--remote` advances submodules to newer upstream commits and can move testers away from the known baseline.

## Current important submodules

The exact pins are determined by the checked-out top-level commit. For reference, the current baseline records these key submodules:

```text
lib/N64ModernRuntime  366e1499288f78a2dc91100bb6083afa51dc7639
lib/rt64              589e172da86f8390cb7d31b904494c522a8f46ba
lib/RmlUi             7a06f27db04fe5d13a5dacc19b2b4544673a4eca
lib/lunasvg           4166d0cccfc059b39d5ecfc372524375e59448f9
```

Use `git submodule status --recursive` to report the actual SHAs when filing build or crash reports. If these differ after checking out the maintainer-provided top-level SHA and running `git submodule update --init --recursive`, the checkout is not on the expected baseline.

## Generated/local files

ROMs and generated artifacts are intentionally not fully distributed through Git.

A source build/test checkout needs, at minimum, the generated `RecompiledFuncs/` outputs and runtime ROM resources expected by the project, including:

```text
RecompiledFuncs/funcs.h
RecompiledFuncs/recomp_overlays.inl
RecompiledFuncs/*.c              # full generated set, not only tracked patched files
rom.z64                          # preferred runtime name; stock Castlevania: LoD ROM image
```

Developer checkouts may also use the older `resources/*.z64` layout as a fallback, but release binaries prefer `./rom.z64`.

For source regeneration on macOS/Linux, do **not** run N64Recomp directly. Use:

```sh
./tools/regen_recomp.sh
```

That script verifies the tool, repairs known truncation issues, reapplies patches, and keeps symbol replacements consistent. On Windows, follow the equivalent step-by-step pipeline in [Regenerating RecompiledFuncs on Windows](#regenerating-recompiledfuncs-on-windows). If external testers are only validating Windows builds/gameplay, the maintainer should provide a prepared source/resources bundle or generated artifact bundle matching the top-level commit.

## macOS release / tag run instructions

### Running the macOS binary asset

Download the macOS `.app` asset from the GitHub release, extract it, and place your legally dumped stock Castlevania: Legacy of Darkness ROM next to the app as `rom.z64`:

```text
LodRecomp.app
rom.z64
```

The current macOS app is Apple Silicon/arm64, ad-hoc signed, and links against Homebrew SDL2 and Freetype. Install runtime dependencies if needed:

```sh
brew install sdl2 freetype
```

From the extracted folder, double-click `LodRecomp.app`, or run:

```sh
open ./LodRecomp.app
```

On first launch, macOS may block the ad-hoc signed app. Open **System Settings → Privacy & Security**, choose **Open Anyway**, then confirm **Open**.

The app intentionally looks for `rom.z64` beside `LodRecomp.app` first, so keep both files in the same folder. If macOS launches the app through App Translocation and that adjacent ROM is hidden from the process, the app will ask you to select your stock LoD ROM once with a native file picker and remember that path for future launches. Runtime NI overlay data is prepared in memory from the stock ROM on startup; users do not need to run a separate preparation script.

### Graphics/settings overlay

The app creates a persistent graphics config at `~/Library/Application Support/LodRecomp/graphics.json` on macOS and `%APPDATA%\LodRecomp\graphics.json` on Windows.
Fresh configs default to `Original2x` internal resolution; existing configs are preserved.

The overlay starts hidden. Press `F1` to show/hide the in-game RmlUi settings overlay. The current overlay includes General, Graphics, Controls, and Audio tabs. Graphics options are clickable and staged until **Apply** writes `graphics.json`; **Discard** restores the active renderer values. Esc / controller B closes the overlay, prompting first if graphics changes are pending.

The UI is currently embedded in the executable, so release packages do not need separate `assets/ui` files yet. For automated visual smoke tests, developers may set `RECOMP_UI_OPEN_ON_START=1` to open the otherwise-hidden overlay after renderer initialization.

Existing graphics hotkeys are still supported:

| Key | Action |
| --- | --- |
| `F1` | Show/hide the RmlUi settings overlay |
| `Esc` | Close prompt/settings overlay |
| `F11` | Toggle windowed/fullscreen |
| `F5` | Cycle internal resolution mode (`Original`, `Original2x`, `Auto`) |
| `F6` | Cycle aspect ratio mode (`Original`, `Expand`, `Manual`) |
| `F7` | Cycle anti-aliasing (`None`, `MSAA2X`, `MSAA4X`, `MSAA8X`) |
| `F8` | Cycle refresh-rate mode (`Original`, `Display`, `Manual`) |

Hotkeys apply live and save automatically. Graphics changes made through the overlay are staged until **Apply**.

Native LoD Expansion Pak high-resolution mode is currently skipped in default builds. The runtime still maps/guards the internal 8MB RDRAM mirror and reports 4MB to the game, but the selector still appears on the stock boot path; default builds therefore hard-skip only that selector (`gs=12`) by requesting the normal post-selector transition (`gs=-5`, which creates `gs=5`) to keep testers on the validated low-resolution route. Developers can investigate the native selector/high-res route with `-DLOD_SKIP_EXPANSION_PAK_SCREEN=OFF -DLOD_ENABLE_NATIVE_HIGH_RES=ON`.

### Building from a release tag

GitHub releases are also tagged source baselines. Use the release tag with `git clone --recursive`; do **not** rely on GitHub's auto-generated "Source code" zip/tarball for a fresh build because it does not include submodule contents and it still requires the local/generated files listed above.

Prerequisites:

```sh
xcode-select --install
brew install cmake ninja pkg-config sdl2 freetype
```

Checkout a release tag, for example `v0.1.2`:

```sh
git clone --recursive --branch v0.1.2 https://github.com/fliperama86/cvlod_recomp.git
cd cvlod_recomp
git submodule sync --recursive
git submodule update --init --recursive
git submodule status --recursive
```

Before building, provide the generated/local files listed in [Generated/local files](#generatedlocal-files), especially a stock `rom.z64` and the full generated `RecompiledFuncs/` output matching the release tag.

Build:

```sh
cmake -S . -B build-macos -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build-macos --target LodRecomp --parallel
```

Codesign before running on macOS:

```sh
codesign -s - --entitlements .github/macos/entitlements.plist -f build-macos/LodRecomp
```

Run from the repository root so `./rom.z64` resolves:

```sh
./build-macos/LodRecomp
```

For tester reports against a release, include:

```sh
git rev-parse HEAD
git describe --tags --always --dirty
git submodule status --recursive
```

## Linux build

Validated on Ubuntu 24.04/aarch64 in Docker with Clang 18. GCC currently fails at the final link step because generated `RECOMP_FUNC` symbols are strong under GCC, which conflicts with the project override shims. Use Clang for Linux builds.

Prerequisites on Ubuntu 24.04:

```sh
sudo apt-get update
sudo apt-get install -y \
  git build-essential cmake ninja-build pkg-config python3 clang lld \
  libsdl2-dev libfreetype-dev zlib1g-dev libvulkan-dev libgtk-3-dev \
  libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev
```

From the repository root, after `git submodule update --init --recursive` and after copying/providing the generated/local files listed above:

```sh
CC=clang CXX=clang++ cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build-linux --target LodRecomp --parallel
```

Run from the repository root so relative `resources/...` paths resolve:

```sh
./build-linux/LodRecomp
```

## Windows

### Running the Windows binary asset

Download `LodRecomp-vX.Y.Z-windows-x64.zip` from the GitHub release, extract the whole folder (keep the DLLs next to the exe), and place your legally dumped stock Castlevania: Legacy of Darkness ROM in the same folder as `rom.z64`:

```text
LodRecomp.exe   (+ SDL2.dll, dxcompiler.dll, dxil.dll, freetype.dll, ...)
rom.z64
```

If no `rom.z64` is found, the app asks you to select your stock LoD ROM once with a native file picker and remembers that path.

The binary is not code-signed yet, so the first launch may show a SmartScreen "Windows protected your PC" prompt — click **More info → Run anyway**. Verify the download against the SHA256 checksum published in the release notes first.

Config files (`graphics.json`, `controls.json`) are stored in `%APPDATA%\LodRecomp`.

### Building from source on Windows

The Windows build **requires clang-cl** (the ClangCL toolset). Plain MSVC `cl` cannot build the project: it hard-errors on the GNU-style warning flags used by the runtime submodules, and COFF builds rely on `/FORCE:MULTIPLE` + link order instead of ELF weak-symbol interposition for the `RECOMP_FUNC` override shims.

Prerequisites:

- Git
- Visual Studio 2022 (or Build Tools) with the **Desktop development with C++** workload **and the "C++ Clang tools for Windows" component** (provides clang-cl and the ClangCL MSBuild toolset)
- CMake 3.20+
- vcpkg-provided ZLIB **and Freetype** (Freetype is required by RmlUi on every platform)

```powershell
# One-time vcpkg setup, if needed
git clone https://github.com/microsoft/vcpkg C:\src\vcpkg
C:\src\vcpkg\bootstrap-vcpkg.bat
C:\src\vcpkg\vcpkg install zlib:x64-windows freetype:x64-windows

# From the repo root — note the ClangCL toolset (-T ClangCL)
cmake -S . -B build-win -G "Visual Studio 17 2022" -A x64 -T ClangCL `
  -DCMAKE_TOOLCHAIN_FILE=C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-win --config RelWithDebInfo --target LodRecomp
```

SDL2 is fetched automatically by CMake on Windows. The RT64 DXC DLLs are copied from the `lib/rt64` submodule and the vcpkg runtime DLLs are deployed next to the exe during the post-build step.

If a parallel build fails with `Permission denied` errors on CMake `*.tmp` stamp files (concurrent per-project re-configures racing), re-run the configure once and build serially:

```powershell
cmake -S . -B build-win
cmake --build build-win --config RelWithDebInfo --target LodRecomp -- /m:1
```

Run from the repository root so `./rom.z64` and relative `resources/...` paths resolve:

```powershell
.\build-win\RelWithDebInfo\LodRecomp.exe
```

### Regenerating RecompiledFuncs on Windows

`tools/regen_recomp.sh` is a bash/macOS pipeline and expects the maintainer's known-good N64Recomp binary. On Windows, the equivalent validated flow (output verified byte-identical to the macOS generation) is:

```powershell
# 1. Build the N64Recomp CLI from the clean, pinned submodule
cmake -S lib\N64ModernRuntime\N64Recomp -B build-n64recomp -G "Visual Studio 17 2022" -A x64
cmake --build build-n64recomp --config Release --target N64RecompCLI

# 2. Prepare the NI-extended ROM from the stock ROM (use python, not python3)
python tools\ni_ovl\step1_dump_table.py rom.z64
python tools\ni_ovl\step2_decompress.py rom.z64
python tools\ni_ovl\step3_boundaries.py
python tools\ni_ovl\step4_functions.py
python tools\ni_ovl\step6_extended_rom.py rom.z64   # needs resources\ to exist

# 3. Run N64Recomp (exits non-zero on the known overlay_system message; that is expected)
.\build-n64recomp\Release\N64Recomp.exe recomp.toml

# 4. Post-processing chain (same order as regen_recomp.sh)
python tools\fix_n64recomp_truncation.py
python tools\ni_ovl\repair_overlay_table.py --fix
python tools\ni_ovl\fix_undeclared_labels.py
python tools\apply_patches.py
python tools\apply_symbols.py
```

The Windows N64Recomp build aborts slightly later than the macOS one, producing two extra artifacts that the fixer chain now handles automatically: a dangling `overlay_sections_by_index` opener in `recomp_overlays.inl`, and a duplicate `static_*_0E0037A0` definition. The N64Recomp submodule source must be clean (`git -C lib/N64ModernRuntime/N64Recomp status`) — never regenerate with a modified tool.

## Audio baseline and useful CMake flags

The default build is intended to produce audible game audio. The current audio baseline is:

- `LOD_USE_RECOMPILED_AUDIO_RSP=ON` — uses the generated LoD audio RSP microcode instead of the timing-only stub.
- `LOD_OVERRIDE_ALENVMIXERPULL=OFF` — keeps the real `alEnvmixerPull` path active. Turning this override on mutes synthesis and is only useful for A/B diagnostics.

These are the defaults, so normal testers should not need to pass audio flags. If you need to state them explicitly:

```sh
cmake -S . -B build -DLOD_USE_RECOMPILED_AUDIO_RSP=ON -DLOD_OVERRIDE_ALENVMIXERPULL=OFF
```

Debug audio traces remain default-off. Use `LOD_ENABLE_AUDIO_TRACE`, `LOD_ENABLE_RSP_AUDIO_TRACE`, `LOD_ENABLE_AUDIO_PULL_TRACE`, or `LOD_ENABLE_AUDIO_RAW_DUMP` only for targeted diagnostics, and include those flags in crash/test reports.

## Controls

Keyboard input remains available:

- Enter = N64 A
- Right Shift = N64 B
- Z = N64 Z
- Space = Start
- Arrow keys = D-pad
- WASD = analog stick

SDL game controllers are also supported. Xbox controllers and DualSense/DualShock controllers should be detected through SDL's standard GameController mappings:

- A / Cross = N64 A / jump
- X / Square = N64 B / attack 1 / primary attack
- Y / Triangle = N64 C-Left / attack 2 / secondary attack
- B / Circle = N64 C-Right / collect / interact
- Start / Options = Start
- Left stick = analog stick
- D-pad = N64 D-pad
- Left stick click / L3 = N64 R / lock-on
- Right bumper / R1 = N64 C-Down / throw item
- Left trigger / L2 = N64 Z
- Right trigger / R2 = N64 L
- Right stick = inverted N64 D-pad / camera

Gamepad controls are configurable by editing `controls.json` in the same config directory as `graphics.json`:

- macOS: `~/Library/Application Support/LodRecomp/controls.json`
- Windows: `%APPDATA%\LodRecomp\controls.json`
- Linux/other local builds: `~/.lodrecomp/controls.json`

The file is created on first launch. Edit it while the game is closed, then relaunch. Valid N64 binding values are:

```text
none, n64_a, n64_b, n64_z, n64_start,
n64_dpad_up, n64_dpad_down, n64_dpad_left, n64_dpad_right,
n64_l, n64_r,
n64_c_up, n64_c_down, n64_c_left, n64_c_right
```

Example:

```json
{
  "gamepad": {
    "buttons": {
      "a": "n64_a",
      "b": "n64_c_right",
      "x": "n64_b",
      "y": "n64_c_left",
      "start": "n64_start",
      "left_stick": "n64_r",
      "right_bumper": "n64_c_down",
      "dpad_up": "n64_dpad_up",
      "dpad_down": "n64_dpad_down",
      "dpad_left": "n64_dpad_left",
      "dpad_right": "n64_dpad_right"
    },
    "axes": {
      "left_trigger": "n64_z",
      "right_trigger": "n64_l",
      "trigger_threshold": 12000
    },
    "right_stick": {
      "mode": "n64_dpad",
      "invert_x": true,
      "invert_y": true,
      "deadzone": 0.5
    }
  }
}
```

## Crash/test reports

When reporting a crash or regression, include:

- Top-level commit SHA: `git rev-parse HEAD`
- Submodule SHAs: `git submodule status --recursive`
- Build flags/CMake options used
- Full log around `[CRASH]`, `[ni_ovl] WARNING`, `[KSEG0-TRACE]`, or `[KSEG0-DMAMGR]`
- Whether the run used low-resolution or high-resolution mode

Known current posture is tracked in [`docs/RECOVERY_TO_GAMEPLAY.md`](docs/RECOVERY_TO_GAMEPLAY.md).
