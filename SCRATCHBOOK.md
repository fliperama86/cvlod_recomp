2026-02-20 23:54:11 -0300
- User constraints: no non-read-only actions without prior discussion/approval; proceed one step at a time; no commits unless explicitly requested.
- Task: create up-to-date N64 recompilation documentation for bootstrapping a Castlevania: Legacy of Darkness (N64) recomp project.
- Read-only work completed: inspected local `Zelda64Recomp` reference (pipeline, TOML configs, runtime integration, overlays/patches/mod paths), validated local LoD ROM metadata.
- Local ROM facts: `resources/castlevania_legacy_of_darkness.z64` header title `CASTLEVANIA2`, game code `ND4E`; SHA1 `879ead98f197fd05edda867655da5b1ce25aa5b8`; SHA256 `89e15df6042defddc48c61f7408f99d06fffdb845e2422cf8b5ba8e73d4d70fb`; MD5 `25258460f98f567497b24844abe3a05b`.
- Online verification completed: checked current upstream repos/releases (N64Recomp org, N64Recomp tool, N64ModernRuntime, Zelda64Recomp, dino-recomp) and identified newer `N64Recomp/RecompFrontend` repository.
- Current hypothesis: safest LoD path is Zelda64Recomp-style baseline (N64Recomp + N64ModernRuntime + RT64) with early strict ROM/version gating and symbol/patch pipeline established before feature work.
- Next step: write `docs/N64_Recompilation_Guide_2026.md` with source-linked 2026 architecture + Castlevania bootstrap plan.

2026-02-20 23:55:46 -0300
- Write step completed: added `docs/N64_Recompilation_Guide_2026.md` (kept old guide intact).
- New doc includes: 2026 ecosystem snapshot, concrete end-to-end recomp pipeline, config behavior from parser code, LoD-specific bootstrap gates, risk list, and local/online source lists.
- Verified consistency check: file readback succeeded.
- Recommended next step: compute LoD XXH3_64 hash and lock first `GameEntry` target revision before scaffolding code.
