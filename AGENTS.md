## Guidelines

- Don’t change any code or run any non-read-only-commands without informing me and discussing it first.
- Be extremely methodical, performing one step at a time and ensuring it works properly before continuing.
- Commit and push cohesive progress once the scope is understood; the user prefers a clean baseline over dangling local files.
- Never commit unrelated changes silently. Summarize what was committed/pushed and keep commits intentional.

## Project Tracking (Required)

- `SCRATCHBOOK.md` is retired. Do not recreate it or use append-only per-turn session memory.
- Keep durable project state in focused docs instead:
  - `docs/RECOVERY_TO_GAMEPLAY.md` — live gameplay-recovery tracker. Update it whenever recovery work changes the baseline, hypotheses, experiments, milestones, validation status, or next actions.
  - `FUNCTION_MAP.md` — identified functions and naming notes.
  - `docs/controller-pak-approaches.md` — Controller Pak/PFS-specific notes.
- Update focused docs only when durable project knowledge changes, not on every interaction.
- Keep tracking entries concise, factual, and actionable:
  - Current baseline and validation status.
  - What was attempted, what worked, and what failed/regressed.
  - Current hypotheses, safe/unsafe patterns, and next recommended steps.
- Prefer introducing risky or experimental behavior behind compile-time feature flags (`#ifdef`/macros).
- Default policy for experiments: flag-gated, off by default, easy rollback path, avoid direct regression risk.

## Reverse Engineering Guidelines (Required)

### When to change recompiled / external code
- Only change recompiled code or external libs if:
  1. Adding logs (fprintf etc)
  2. It is a temporary hack to test something
  3. It is absolutely necessary and will be required for the final code to work

### Symbol naming workflow
- `castlevania2.syms.toml` is the **single source of truth** for function names.
- `symbol_addrs.txt` is secondary (feeds splat for asm output). Keep in sync manually.
- When adding/renaming functions, update `castlevania2.syms.toml` then run `./tools/regen_recomp.sh`.
- The regen script: saves patches, runs N64Recomp, fixes truncation bugs, re-applies hand-written patches.
- CV64 decomp (`../references/cv64_decomp/`) is the primary reference for shared engine function names.
- `tools/match_cv64.py` matches CV64 functions to LoD by instruction fingerprinting (requires both ROMs).
- **ALWAYS update function names whenever progress is made.** Every debugging session, code trace, or investigation that identifies a `func_XXXXXXXX` must result in a rename. Do not leave identified functions unnamed — name them immediately in `castlevania2.syms.toml` and `symbol_addrs.txt`.

### Hand-written patches
- Patches live in `tools/apply_patches.py`, keyed by MIPS VRAM address (never changes across regens).
- Mark patches with `// --- PATCH: description ---` and `// --- END PATCH ---` markers.
- To add a new patch: add a `@patch("description")` function in `apply_patches.py`.
- `patches/recompiled_funcs.patch` is a backup diff — the real patches are in `apply_patches.py`.

### RecompiledFuncs regeneration (CRITICAL)
- **NEVER run N64Recomp by itself without the fixer chain.** On macOS/Linux always use `./tools/regen_recomp.sh`. It verifies binary integrity, backs up before overwriting, fixes truncation bugs, re-applies patches, and fixes cross-function gotos. On Windows, follow the step-by-step equivalent documented in README "Regenerating RecompiledFuncs on Windows" — run N64Recomp **and then the full post-processing chain in order**; skipping the fixers produces broken output.
- **NEVER modify N64Recomp submodule source** (`lib/N64ModernRuntime/N64Recomp/src/`). On macOS the known-good binary is at `../lod_recomp_failed/tools/N64Recomp/build/N64Recomp` and `regen_recomp.sh` verifies its SHA-256 hash before use. On Windows that binary does not exist; building `N64RecompCLI` from the **clean, pinned** submodule (out-of-tree, e.g. `build-n64recomp/` at the repo root) is the validated alternative — its output was verified byte-identical to the macOS generation. Always confirm the submodule is clean (`git -C lib/N64ModernRuntime/N64Recomp status`) before regenerating.
- **NEVER create build directories inside the N64Recomp submodule.** This makes the submodule appear dirty and risks using a modified binary. Build out-of-tree at the repo root instead.
- `RecompiledFuncs/` is gitignored — if corrupted, there is no git revert. The only recovery is re-running the regen pipeline from a clean state.
- Past incident: a locally-modified N64Recomp binary (turning a fatal error into a warning) was used to regenerate, producing corrupted overlay tables that caused `Failed to find function at 0x3C198000` at runtime.

### N64Recomp truncation bug
- N64Recomp crashes on the overlay_system section, truncating `funcs.h`, `recomp_overlays.inl`, and the last `.c` file being written.
- The abort point differs per platform. The Windows tool build gets a few lines further than the macOS one and additionally emits: (a) a dangling `overlay_sections_by_index[] = {` opener with no body in `recomp_overlays.inl`, and (b) a complete duplicate `static_*_0E0037A0` definition in its own `funcs_*.c` on top of the headerless inline copy. `fix_n64recomp_truncation.py` detects and repairs both variants.
- `tools/fix_n64recomp_truncation.py` fixes this automatically (called by `regen_recomp.sh`).
- `tools/ni_ovl/fix_undeclared_labels.py` fixes cross-function gotos (also called by `regen_recomp.sh`).
- `tools/ni_ovl/repair_overlay_table.py` validates/repairs NI section metadata; if `tools/ni_ovl/out/vram_classification.json` is missing (untracked analysis artifact), it derives the 0x0E pair set from `src/main/ni_ovl_data.h` automatically.
- All RecompiledFuncs-writing tools emit LF line endings on every platform so generated output stays hash-comparable across macOS/Windows.

### File roles
| File | Role | Generated? | Tracked? |
|------|------|-----------|----------|
| `castlevania2.syms.toml` | Function names + addresses + sizes | No | Yes |
| `recomp.toml` | N64Recomp config (stubs, paths) | No | Yes |
| `symbol_addrs.txt` | splat symbol database | No | Yes |
| `RecompiledFuncs/funcs_*.c` | Recompiled MIPS-to-C | Yes (N64Recomp) | 10 patched files only |
| `RecompiledFuncs/funcs.h` | Forward declarations | Yes (N64Recomp) | No (auto-fixed) |
| `RecompiledFuncs/recomp_overlays.inl` | Overlay section table | Yes (N64Recomp) | No (auto-fixed) |
| `tools/apply_patches.py` | Hand-written patch definitions | No | Yes |
| `tools/fix_n64recomp_truncation.py` | Truncation auto-fixer | No | Yes |
| `tools/regen_recomp.sh` | Full regen pipeline | No | Yes |
| `tools/match_cv64.py` | CV64-to-LoD function matcher | No | Yes |
| `tools/apply_symbols.py` | Post-regen symbol renaming (hex→named constants) | No | Yes |
| `include/lod_symbols.h` | Named constants (structs, enums, offsets) from CV64+LoD RE | No | Yes |

### Running LodRecomp safely
- macOS: always codesign before running: `codesign -s - --entitlements .github/macos/entitlements.plist -f build/LodRecomp`
- Use bounded runs: `timeout 20 build/LodRecomp` — never background with `&`.
- Monitor RSS: past incident caused 100GB memory leak from forced patches.
- After each run, verify no stale processes: `pgrep -la LodRecomp`
- Windows: no codesigning step is needed. Run `.\build-win\RelWithDebInfo\LodRecomp.exe` from the repo root; verify/stop stale processes with `Get-Process LodRecomp` / `Stop-Process -Name LodRecomp`. Do not pipe the exe's output through head-style truncation (`Select-Object -First N`) — closing the pipe kills the game mid-run.

### CV64 cross-reference
- CV64 decomp at `../references/cv64_decomp/` — same Konami engine, ~60-70% code overlap with LoD.
- CV64 ROM at `resources/castlevania.z64` (ND3E).
- `include/game/object.h` has the Object struct layout (0x74 bytes).
- `include/ultra64/PR/sched.h` has OSSched/OSScTask structs.
- Addresses differ between ROMs — use instruction fingerprinting, not address matching.
