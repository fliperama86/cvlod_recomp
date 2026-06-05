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
lib/rt64              20d75f59c5fd67f20063aec056dd2da08479815e
lib/RmlUi             7a06f27db04fe5d13a5dacc19b2b4544673a4eca
lib/lunasvg           4166d0cccfc059b39d5ecfc372524375e59448f9
```

Use `git submodule status --recursive` to report the actual SHAs when filing build or crash reports. If these differ after checking out the maintainer-provided top-level SHA and running `git submodule update --init --recursive`, the checkout is not on the expected baseline.

## Generated/local files

ROMs and generated artifacts are intentionally not fully distributed through Git.

A build/test checkout needs, at minimum, the generated `RecompiledFuncs/` outputs and runtime resources expected by the project, including:

```text
RecompiledFuncs/funcs.h
RecompiledFuncs/recomp_overlays.inl
RecompiledFuncs/*.c              # full generated set, not only tracked patched files
resources/castlevania_legacy_of_darkness.z64
resources/castlevania2_decompressed.z64
resources/castlevania2_ni_extended.z64
```

For source regeneration, do **not** run N64Recomp directly. Use:

```sh
./tools/regen_recomp.sh
```

That script verifies the tool, repairs known truncation issues, reapplies patches, and keeps symbol replacements consistent. If external testers are only validating Windows builds/gameplay, the maintainer should provide a prepared source/resources bundle or generated artifact bundle matching the top-level commit.

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

## Crash/test reports

When reporting a crash or regression, include:

- Top-level commit SHA: `git rev-parse HEAD`
- Submodule SHAs: `git submodule status --recursive`
- Build flags/CMake options used
- Full log around `[CRASH]`, `[ni_ovl] WARNING`, `[KSEG0-TRACE]`, or `[KSEG0-DMAMGR]`
- Whether the run used low-resolution or high-resolution mode

Known current posture is tracked in [`docs/RECOVERY_TO_GAMEPLAY.md`](docs/RECOVERY_TO_GAMEPLAY.md).
