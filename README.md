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

For source regeneration, do **not** run N64Recomp directly. Use:

```sh
./tools/regen_recomp.sh
```

That script verifies the tool, repairs known truncation issues, reapplies patches, and keeps symbol replacements consistent. If external testers are only validating Windows builds/gameplay, the maintainer should provide a prepared source/resources bundle or generated artifact bundle matching the top-level commit.

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

## Windows build

Prerequisites:

- Git
- Visual Studio 2022 with the **Desktop development with C++** workload
- CMake 3.20+
- A CMake-discoverable ZLIB, for example through vcpkg

Example with vcpkg-provided ZLIB:

```powershell
# One-time vcpkg setup, if needed
git clone https://github.com/microsoft/vcpkg C:\src\vcpkg
C:\src\vcpkg\bootstrap-vcpkg.bat
C:\src\vcpkg\vcpkg install zlib:x64-windows

# From the repo root
cmake -S . -B build-win -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE=C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-win --config RelWithDebInfo --target LodRecomp
```

SDL2 is fetched automatically by CMake on Windows. The RT64 DXC DLLs are copied from the `lib/rt64` submodule during the post-build step.

Run from the repository root so relative `resources/...` paths resolve:

```powershell
.\build-win\RelWithDebInfo\LodRecomp.exe
```

If using a single-config generator such as Ninja, the executable is typically under `build-win\LodRecomp.exe` instead.

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

- A / Cross or Y / Triangle = N64 A / jump
- B / Circle or X / Square = N64 B / attack
- Start / Options = Start
- Left stick = analog stick
- D-pad = N64 D-pad
- Left bumper = N64 L
- Right bumper = N64 R
- Left or right trigger = N64 Z
- Right stick = N64 C-buttons

## Crash/test reports

When reporting a crash or regression, include:

- Top-level commit SHA: `git rev-parse HEAD`
- Submodule SHAs: `git submodule status --recursive`
- Build flags/CMake options used
- Full log around `[CRASH]`, `[ni_ovl] WARNING`, `[KSEG0-TRACE]`, or `[KSEG0-DMAMGR]`
- Whether the run used low-resolution or high-resolution mode

Known current posture is tracked in [`docs/RECOVERY_TO_GAMEPLAY.md`](docs/RECOVERY_TO_GAMEPLAY.md).
