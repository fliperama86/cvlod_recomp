## Guidelines

- Don’t change any code or run any non-read-only-commands without informing me and discussing it first.
- Be extremely methodical, performing one step at a time and ensuring it works properly before continuing.
- Do not commit code unless explicitly told otherwise.

## Session Memory (Required)

- Maintain `SCRATCHBOOK.md` as persistent cross-session memory.
- Update `SCRATCHBOOK.md` on nearly every user interaction with concise entries.
- Always record:
  - User preferences and workflow constraints.
  - What was attempted, what worked, and what failed/regressed.
  - Current hypotheses, safe/unsafe patterns, and next recommended steps.
- Keep entries factual, timestamped, and short (append-only log style).
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

### Hand-written patches
- Patches live in `tools/apply_patches.py`, keyed by MIPS VRAM address (never changes across regens).
- Mark patches with `// --- PATCH: description ---` and `// --- END PATCH ---` markers.
- To add a new patch: add a `@patch("description")` function in `apply_patches.py`.
- `patches/recompiled_funcs.patch` is a backup diff — the real patches are in `apply_patches.py`.

### N64Recomp truncation bug
- N64Recomp crashes on the overlay_system section, truncating `funcs.h`, `recomp_overlays.inl`, and `funcs_102.c`.
- `tools/fix_n64recomp_truncation.py` fixes this automatically (called by `regen_recomp.sh`).

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

### Running LodRecomp safely
- Always codesign before running: `codesign -s - --entitlements .github/macos/entitlements.plist -f build/LodRecomp`
- Use bounded runs: `timeout 20 build/LodRecomp` — never background with `&`.
- Monitor RSS: past incident caused 100GB memory leak from forced patches.
- After each run, verify no stale processes: `pgrep -la LodRecomp`

### CV64 cross-reference
- CV64 decomp at `../references/cv64_decomp/` — same Konami engine, ~60-70% code overlap with LoD.
- CV64 ROM at `resources/castlevania.z64` (ND3E).
- `include/game/object.h` has the Object struct layout (0x74 bytes).
- `include/ultra64/PR/sched.h` has OSSched/OSScTask structs.
- Addresses differ between ROMs — use instruction fingerprinting, not address matching.
