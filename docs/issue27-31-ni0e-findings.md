## Round 18 (2026-07-22): the ring's completion consumer, a silent-drop overflow bug, and func_801682D0's actual repro18 crash

### Task A: enqueue-to-completion pipeline, fully traced

**1) The complete chain, function addresses.** Extending round 17's writer
chain (`func_80145DA8` -> `func_800119CC` -> `func_80011754` ->
`func_800116BC`) all the way to where the data is actually served and where
the caller's `completionPtr` (`&(sys+0x2B28)` for the text-load case) finally
gets written:

- **`func_800116BC`** (`RecompiledFuncs/funcs_7.c:4260`, vaddr `0x800116BC`,
  round 17's writer, read in full this round): `v1 = *(a0+0x34)` is the ring
  **descriptor**, a single global instance reachable from RDRAM `0x800C1600`
  (see below). The ring array is 16 slots of 20 bytes each, base
  `v1+0x848`. `v1+0x844` is the **read index** (base of the oldest pending
  entry, not a monotonic cursor); `v1+0x846` is the **pending count**
  (0-16), not a write index as round 17's phrasing suggested -- refining
  that round's field naming, not its substance. A new entry is appended at
  slot `(readIndex + pendingCount) mod 16`, then `pendingCount += 1`. Each
  20-byte slot layout (relative to the slot's own base, `entry+0x848`):
  `+0x0` romAddr, `+0x4` destBuffer (RAM), `+0x8` size, `+0xC` **fileid**
  (see below), `+0x10` **completionPtr**. **New finding, not in round 17**:
  the `pendingCount < 16` check happens *after* the unconditional
  `*completionPtr = 0` zero-store earlier in the same function -- see the
  stall hypothesis below.
- **Where "fileid" actually comes from**: `func_80011754`
  (`funcs_7.c:4366`-ish, already partly documented by round 14), on its
  "not yet loaded" path, stores the fileid it was called with **directly
  into RDRAM `0x800C1608`** (`funcs_7.c`, right before the `func_800048C4`
  DMA-range-lookup call and a few lines before calling `func_800116BC`).
  `func_800116BC` re-reads that same global fresh (`t4 = MEM_W(0x800C0000,
  0x1608)`) and copies it into the new slot's `+0xC` field. This single
  global is what makes it possible to recover "which file was this ring
  entry for" at both enqueue and completion time without threading an extra
  argument through every call in the chain.
- **The per-frame driver**: `func_80011D10` (`funcs_7.c:5400`) is a generic
  "bump the object's re-entrancy depth at `obj+0xE`, index a per-depth
  `(count, state)` byte pair at `obj+depth*2+8/9`, `jalr` through a
  dispatch table" driver -- structurally identical to the `driver` half of
  round 15's bgState `0x1AB` state machine, but for a **different** object
  (the DMA chunk-manager) and a **different** table (`0x800AF560`, i.e.
  `0x800B0000-0xAA0`, not the bgState table at `0x8018D3B0`). Neither
  `func_80011D10` nor any of the DMA-state functions below are ever called
  via a direct `jal` anywhere in `RecompiledFuncs/*.c`; all are reached only
  through this table, meaning some object's per-frame update drives this
  state machine (the object itself was not identified this round -- see
  open thread below).
- **`DMAMgr_updatePendingFileLoad`** (`funcs_7.c:5596`, vaddr `0x80011E48`,
  already named by the codebase's own symbol map and independently
  cross-referenced against the CV64 engine in
  `docs/RECOVERY_TO_GAMEPLAY.md`): if the ring's pending count
  (`descriptor+0x846`) is 0, returns immediately (nothing queued). Otherwise
  it takes the **head-of-queue** slot (at `readIndex`, i.e. the oldest
  pending entry -- it does not scan the whole ring), copies that slot's
  `{romAddr, destBuffer, size, fileid}` into "active" bookkeeping fields at
  `descriptor+0x828/+0x82C/+0x830/+0x840`, and issues the actual ROM read
  via `DMA_ROMCopy`.
- **Where the recomp actually serves the data**: `DMA_ROMCopy`
  (`RecompiledFuncs/funcs_12.c:5627`, vaddr `0x8001A42C`) ->
  `DMA_readWrite` (`funcs_12.c:5497`, vaddr `0x8001A374`) ->
  `osEPiStartDma_recomp` + a **blocking** `osRecvMesg_recomp` on the same
  message queue (`funcs_12.c` around `0x8001A3FC`/`0x8001A410`). Neither
  `DMA_readWrite`/`DMA_ROMCopy` nor the `osEPi*`/`osRecvMesg` functions they
  call are natively overridden anywhere in `src/main/*.cpp` (confirmed by
  grep) -- unlike `func_80097730`/osMapTLB (round 1), this path runs
  entirely through the recompiled MIPS translation plus the stock
  `librecomp`/`ultramodern` OS-thread shims in
  `lib/N64ModernRuntime`. Because `osRecvMesg` **blocks** the calling
  (game) thread until the PI DMA interrupt posts to the queue, the ROM read
  invoked from `DMAMgr_updatePendingFileLoad` is **synchronous from the
  MIPS thread's point of view** -- the "asynchronicity" in this whole
  design is the game's own cooperative, multi-frame chunking (staging large
  reads through a small fixed physical buffer, consumed a few bytes at a
  time across many calls via `func_80011CB8`, which decrements a per-transfer
  countdown at `descriptor+0x80C`), not a true interrupt-driven completion
  callback.
- **The completion/dequeue consumer, newly identified this round**:
  **`func_800120DC`** (`RecompiledFuncs/funcs_8.c:177`(pre-round-18)/`257`
  (post-round-18 probe insertion), vaddr `0x800120DC`). Reached only via the
  same table (`func_80011D10`'s dispatch mechanism), i.e. it is a *sibling
  state* of `DMAMgr_updatePendingFileLoad` in the same state machine, not a
  function either of them calls directly. Entry gate: reads
  `descriptor+0x80C` (the per-transfer remaining-bytes-derived countdown
  `func_80011FBC`/`func_80011CB8` maintain). **If `> 0`, the entire
  dequeue/completion block below is skipped** and the function instead
  parses 4-5 more header bytes via `func_80011CB8` (site `pump-chunking` in
  the new probe) -- i.e. this single function is both "am I done with the
  head-of-queue transfer yet" and "if so, finish it," reached repeatedly
  across many per-frame calls for one transfer before it ever completes.
  When the countdown reaches `<=0`: reads `readIndex` (`+0x844`), locates
  the head-of-queue slot (`descriptor + readIndex*20`), reads its
  `completionPtr` field (**`entry+0x858`**, i.e. slot-relative `+0x10`,
  exactly the field `func_800116BC` wrote), and **if nonzero, writes the
  just-completed destination buffer pointer through it**
  (`*completionPtr = descriptor+0x82C` -- the "active.ram" field
  `DMAMgr_updatePendingFileLoad` populated). This is the actual "`0 ->
  bufferPtr`" resolution of `sys+0x2B28` for the text-load case (site
  `complete` in the new probe; most dequeues have no completion target at
  all -- ordinary asset loads -- site `no-target`). It then advances
  `readIndex` (wrapping at 16) and decrements `pendingCount`, and calls
  `func_800048EC` as a per-slot cleanup.

**2) The ring's single global instance.** `func_80011D80`
(`funcs_7.c:5466`) stores its own incoming object argument into RDRAM
`0x800C1600` once (an initialization call, presumably at boot); every
enqueue call site (`func_800119CC`, `func_80010EA0`) re-reads that global
fresh rather than threading the pointer through. This confirms there is
exactly **one** DMA/decompress ring for the whole game, not one per NI pair
or per map.

### Task A: the stall hypothesis

Reading `func_800116BC` in full (not just its `zero-2b28` entry point, as
round 17 did) found a **concrete, previously-undocumented silent-drop path**
that is a stronger, more mechanical explanation than "lost wakeup" or
"consumer stopped running":

```
t6 = completionPtr arg
if (t6 != 0) *t6 = 0;                        // ALWAYS runs first, unconditionally
if (pendingCount >= 16) return;               // ring full -- enqueue silently DROPPED here
... otherwise, actually append the slot ...
```

**The `pendingCount < 16` capacity check happens strictly *after* the
zero-store**, and there is no upstream guard anywhere in the chain
(`func_80145DA8`, `func_800119CC`, `func_80011754`) that checks ring
occupancy before calling `func_800116BC`. If the ring already has 16
entries pending at the exact moment `update_a`'s load state tries to queue
fileid `0x3B`/`0x3D`, `*(sys+0x2B28)` gets zeroed exactly as observed, but
**no ring slot is ever written for that request and no error is returned to
any caller** -- there is nothing left anywhere in the system that will ever
write the real buffer pointer back. This is a single, static, sufficient
explanation for "queued but never consumed" that does not require assuming
the consumer itself is stuck, though the two are not mutually exclusive: if
`DMAMgr_updatePendingFileLoad`/`func_800120DC` stop draining the ring for
unrelated reasons (e.g. the object driving `func_80011D10` gets suspended
across the map transition, matching this investigation's recurring
"managers get suspended across transitions" theme), the ring would
naturally fill toward 16 and then start silently dropping new requests --
i.e. "consumer stalled" is a plausible *upstream cause* of "ring full", not
a competing theory.

The new probes (ring-enq's `ACCEPTED`/`DROPPED(ring full...)` outcome,
ring-done's `complete`/`no-target` sites, and ring-pump's
`pump-chunking` site) are built specifically to distinguish, on the next
capture: (a) ring-enq shows `DROPPED` for fileid `0x3B`/`0x3D` -> ring-full
silent drop, confirmed; (b) ring-enq shows `ACCEPTED` but no matching
ring-done `complete` line ever appears for that fileid, while ring-pump
keeps firing for the same descriptor -> consumer is alive but perpetually
stuck mid-transfer on one (possibly different) entry, head-of-line blocking
everything queued behind it; (c) ring-enq `ACCEPTED` and neither ring-done
nor ring-pump appear again at all post-transition -> the consumer stopped
being invoked entirely (its driving object was suspended/destroyed).

### Task A: probes added

All `[NI0E_TRACE]`-prefixed, stderr, under the existing
`LOD_ENABLE_NI0E_TRACE` flag.

- **`ring-enq`** (`RecompiledFuncs/funcs_7.c`, `lod_ni0e_ring_enq_probe`,
  hooked at both of `func_800116BC`'s exit points): logs fileid (re-read
  fresh from RDRAM `0x800C1608`), descriptor, `completion_ptr`,
  `pending_before`, and either `ACCEPTED` with the slot index/romAddr/
  destBuffer/size, or `DROPPED(ring full, 16 pending, no slot queued)`.
  First 60 + every 200th.
- **`ring-done`** (`RecompiledFuncs/funcs_8.c`, `lod_ni0e_ring_done_probe`/
  `_notarget_probe`, hooked in `func_800120DC` right after the actual
  `*completionPtr = destBuffer` store, and inside the `completionPtr == 0`
  branch respectively): logs fileid (read back from the slot's own `+0xC`/
  `entry+0x854` field, the same persisted snapshot from enqueue time),
  descriptor, slot (read index), and for `complete` the `completion_ptr`
  and `value_written`. First 60 + every 200th, own counter per site.
- **`ring-pump`** (`RecompiledFuncs/funcs_8.c`, `lod_ni0e_ring_pump_probe`,
  hooked at `func_800120DC`'s entry, only when `descriptor+0x80C > 0`):
  logs that this invocation skipped the dequeue check entirely because the
  head-of-queue transfer isn't finished yet. First 60 + every 200th.
- **`ring-pending`** (`src/main/main.cpp`,
  `lod_ni0e_ring_pending_watch_vi_callback`, per-VI, change-triggered plus a
  900-frame heartbeat while the ring is non-empty): the task's literal
  ask -- a host-side watch on RDRAM `0x800C160C`. **Read in full this
  round: `0x800C160C` turns out to be a *different*, unrelated global**
  (`func_80012ED0`'s in-flight allocator-rollback scratch, read/rewritten by
  `func_80011754`/`func_800119CC`/`func_80011974`), not part of the ring at
  all -- flagging this as a refinement of the task brief's assumption, per
  this investigation's established practice. It is still watched verbatim
  as asked. More usefully, the same callback also derefs the single global
  ring instance host-side (`0x800C1600` -> `+0x34` -> descriptor) and
  watches `readIndex`/`pendingCount`/the head-of-queue slot's
  `fileid`/`completionPtr` every VI -- this directly gives "the last
  completed entry and the first stuck one" from the host side, without
  needing a fresh game-code probe to fire.

### Task B: func_801682D0's tail, hardened using the actual repro18 crash log

`build/LodRecomp.log` (left over from the repro18 capture that drove this
round's task brief) contains the exact crash this round names. Read in
full rather than re-deriving from the task's summary alone:

```
[NI0E_TRACE] figsrc-bad site=field14 a0=0xD7000002 container=0x80380F04 cursor=0x00000014 index-ish=-1
[NI0E_TRACE] figwalk-bad ptr=0xD7000002 obj_ok=0 field=0x00000000 depth=1
[TEXT_GUARD] func_8016890C rejected out-of-range obj=0xD7000002 (stale text-struct pointer)

[CRASH] Signal 10 at address 0x38707f800 (code=1)
  ...
2   LodRecomp   func_801682D0 + 2872
3   LodRecomp   func_8003F01C + 376
...
  Fault offset from rdram base: 0x8707F800
  KSEG0 mirror access: host_off=0x8707F800 phys=0x707F800 (past 8MB RDRAM)
```

**Reading this precisely**: the round-14 per-call guard on `func_8016890C`
*did* fire and correctly reject the `field14` call's garbage `obj`
(`0xD7000002`) -- `lod_text_guard_field_logged` is a one-shot flag, so any
*further* rejections after this first one print nothing further, but the
guard's early-return behavior still applies every time it fires. Critically,
**the crash's own native frame #2 is `func_801682D0` itself, not
`func_8016890C`** -- the fault did not happen inside that (guarded) callee
at all, and no further `figsrc-bad`/`TEXT_GUARD` line was printed before the
crash. This rules out "the crash is another bad call into `func_8016890C`"
and points at a fault happening directly inside `func_801682D0`'s own
instructions.

**Reading the array loop again (`L_801683D4`, the fourth call site,
`funcs_57.c`) with this in mind finds the mechanism**: each iteration
computes its `a0`/`a1` with

```c
ctx->r24 = ADD32(objArrayBase, cursor);   // objArrayBase = s2->+0x24
ctx->r9  = ADD32(strArrayBase, cursor);   // strArrayBase = s2->+0x38
ctx->r5 = MEM_W(ctx->r9, 0X0);            // a1 = *(strArrayBase + cursor)  -- direct, UNGUARDED
ctx->r4 = MEM_W(ctx->r24, 0X0);           // a0 = *(objArrayBase + cursor) -- direct, UNGUARDED
func_8016890C(rdram, ctx);
```

Both reads happen **in `func_801682D0` itself**, as plain C statements,
*before* `func_8016890C` (and its own per-call guard) is ever entered. The
round-15 workspace guard (`lod_text_guard_workspace_invalid`) only
validates `s2` itself and `s2->+0x10` -- it does not check `s2->+0x24`
(`objArrayBase`) or `s2->+0x38` (`strArrayBase`) at all. Since the crash's
own preceding log line already proves this exact workspace struct
(`s2=0x80380F04`, itself a validly-in-range pointer) has at least one other
stale/corrupt field (`+0x14`), a stale `+0x24`/`+0x38` array-base pointer in
the same struct crashing on the very first iteration's read (`cursor=0`)
is a fully consistent, sufficient explanation for the fault address
(`phys 0x707F800`, past the 8MB RDRAM span) landing inside `func_801682D0`
proper rather than inside a callee.

`func_8016AE10` (the measure-mode twin, already the subject of round 16's
`$s1` fix) was read again and confirmed to have the **identical** unguarded
`objArrayBase`/`strArrayBase` read pattern in its own loop
(`L_8016AEEC`), feeding `func_8016B62C` instead of `func_8016890C` --
same latent bug, same fix needed by symmetry with every prior round's
twin-auditing practice.

**Fix implemented** (`RecompiledFuncs/funcs_57.c`, both functions, under
the existing `LOD_FIX_TEXT_MEASURE_GUARD` flag): a new, narrowly-scoped
check (`lod_text_guard_array_invalid`) validates `s2->+0x24`/`s2->+0x38`
with the same standard range check already used everywhere else in this
guard family, but is deliberately **not** folded into the broader
up-front `lod_text_guard_workspace_invalid` check. Reasoning: the three/
four fixed-offset text-draw calls (each already individually protected by
`func_8016890C`/`func_8016B62C`'s own per-call guard) and the
variable-length array run are independently-populated parts of the same
shared struct (round 3/7's finding: three fixed slots plus a separately
maintained parallel-array "run"); unconditionally rejecting the whole
struct whenever *only* the array portion (or only the fixed portion) is
stale would silently drop legitimate, currently-active draws that have
nothing to do with the corrupted half. The new check is therefore applied
only when the loop is actually about to execute (`count > 0`, mirroring
the existing `blez`/`count<=0` skip) and only gates the loop itself,
jumping to the exact same safe landing spot (`L_80168404` /
`L_8016AF20`) the workspace guard and the `count<=0` case already use.
`$s1`/`$s0` are already zeroed immediately before this new check runs (the
same unconditional inits both functions already perform right before their
own `blez`), so, unlike round 16's fix, no extra register fixup is needed
at this jump. One-shot `[TEXT_GUARD]` logs added for both sites
(`lod_text_guard_array_draw_logged`/`_measure_logged`), each printing the
rejected `objBase`/`strBase`/`count`.

**Caller audit**: unchanged from round 15/16's own audits (this fix does
not alter either function's return value in any way -- it only prevents a
crash partway through the s2-dependent first half; the independent
sqrt-normalize/list-walk tail after `L_80168404`/`L_8016AF20` still runs
exactly as it did before, producing `$v0` exactly as before). No caller
changes needed.

### Open thread for a future round

The object that drives `func_80011D10`'s dispatch of the DMA ring's state
machine (`DMAMgr_updatePendingFileLoad`/`func_800120DC`) was not identified
this round -- both are reached only through the table at `0x800AF560`, and
no direct `jal` call site exists anywhere in `RecompiledFuncs/*.c` for
either. If a future capture shows `ring-pump`/`ring-done` simply stop
appearing entirely after a map transition (scenario (c) above), the next
step is finding that object (likely via the same `obj+0xE`/`obj+depth*2+8`
re-entrancy-depth field pattern already used to identify the bgState
`0x1AB` driver in round 15) and checking whether it gets suspended or
destroyed across Henry map transitions, the same way round 4/5's pause
manager investigation did for id `0x0AB`.

### Files changed (Round 18)

- `RecompiledFuncs/funcs_7.c`: new `lod_ni0e_ring_enq_probe` (tag
  `ring-enq`) added to the existing `LOD_ENABLE_NI0E_TRACE` block, hooked
  at both exit points of `func_800116BC`.
- `RecompiledFuncs/funcs_8.c`: new `LOD_ENABLE_NI0E_TRACE` scaffolding
  (file previously had none) plus `lod_ni0e_ring_done_probe`/
  `_notarget_probe` (tag `ring-done`) and `lod_ni0e_ring_pump_probe` (tag
  `ring-pump`), hooked into `func_800120DC`.
- `src/main/main.cpp`: new `lod_ni0e_ring_pending_watch_vi_callback` (tag
  `ring-pending`/`ring-pending-heartbeat`), hooked into
  `lod_debug_cheats_vi_callback` alongside the existing per-VI watches.
- `RecompiledFuncs/funcs_57.c`: new `lod_text_guard_array_invalid` helper
  and `lod_text_guard_array_draw_logged`/`_measure_logged` flags added to
  the existing `LOD_FIX_TEXT_MEASURE_GUARD` block; new array-loop guard
  hooked into both `func_801682D0` and `func_8016AE10`, right before each
  function's existing `count<=0` loop-skip check.

### Build/deploy (Round 18)

- Both flags this round's code lives under (`LOD_ENABLE_NI0E_TRACE`,
  `LOD_FIX_TEXT_MEASURE_GUARD`) were already `ON` in `build-ni0e`'s cache
  from rounds 14-17 -- no `-D` flags changed, so no cache reconfigure was
  needed for this round's own options.
- `cmake --build build-ni0e --target LodRecomp -j8`: `RecompiledFuncs`
  (including the three changed files, `funcs_7.c`/`funcs_8.c`/`funcs_57.c`)
  built cleanly with zero warnings/errors on the first attempt, but the
  overall `LodRecomp` target hit the known shared-`Makefile2`/spirv-cross
  hazard (`No rule to make target
  spirv-cross/CMakeFiles/spirv-cross-core.dir/depend.make`), same as rounds
  12-17. `build-ni0e/CMakeFiles/Makefile2` backed up, bare `cmake
  build-ni0e` (no `-D`) run per the documented recovery; `diff` against the
  pre-reconfigure backup showed only line-order reshuffling (semantically
  identical dependency sets), and all five cache flags
  (`LOD_ENABLE_NI0E_TRACE`, `LOD_FIX_TEXT_MEASURE_GUARD`,
  `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`, `LOD_FIX_PAIR126_INPUT_RELEASE`,
  `LOD_FIX_HENRY_LOAD_HANDOFF`) confirmed unchanged afterward. Rebuild
  succeeded: clean, only the pre-existing unrelated `sse2neon.h`
  `-W#warnings` warning.
- `cmake --build build --target LodRecomp -j8` (default tree, flags OFF):
  `RecompiledFuncs` (all three changed files) again built cleanly with the
  flags compiled out; hit the identical shared-Makefile2 hazard from the
  `build-ni0e` reconfigure. Same recovery applied (`Makefile2` backed up,
  bare `cmake build`, diff showed only line-order reshuffling, cache flags
  confirmed unchanged -- all round-13+ trace/fix flags `OFF` except the
  pre-existing default-on `LOD_FIX_PAIR126_INPUT_RELEASE`). Rebuild
  succeeded: clean, only the pre-existing unrelated `sse2neon.h` warning.
- `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums of both
  binaries verified identical afterward (`90fd1e9505831c835ee7b2022e3a1528`),
  so `./build/LodRecomp` now runs the Round-18-instrumented build.
- The game was not launched in this pass (per instructions); no live
  capture was taken. The Task B analysis above is instead grounded directly
  in `build/LodRecomp.log`, the actual repro18 crash log left over from the
  capture that drove this round's task brief (read in full, not just
  summarized). No commit was made (per instructions).

## Round 19 (2026-07-22): a second, structurally-twin ring consumer found; the descriptor's one static writer confirmed; probes-only (no unambiguous root cause)

repro19 overturned round 18's model on two points: `func_800120DC`'s own
probes never fired (ring-done count 0) even though the ring's read index
was observed (host-side) advancing 0..8, and the DMA-manager descriptor
pointer itself was observed to change across a map transition
(`0x80331D38 -> 0x80332E38`, the new one filled with `0x11111111`
uninitialized-heap garbage). This round is analysis + probes only: static
reading alone could not settle *why* either of these happens, so no fix was
implemented (Task D was not triggered -- see below).

### Task A: re-identifying the real ring consumer

Round 18's `func_800120DC` guess is disproved by repro19's own capture, not
by anything wrong in round 18's *reading* of that function (re-verified
this round, unchanged). The task's instruction to find "EVERY reader of
+0x844/+0x846 and the writer of a slot's completionPtr" was done as a
literal whole-tree grep (not scoped to funcs_7.c/8.c/12.c) for the exact
instruction patterns (`MEM_H(...0X844)`/`MEM_H(...0X846)` halfword
accesses, `MEM_W(...0X858)` completionPtr reads), narrow enough to avoid
the false-positive noise generic stack-offset numbers like `0x844` produce
everywhere else in a 6,261-function codebase. Across the **entire**
`RecompiledFuncs/*.c` tree these patterns appear in exactly three places,
no more:

1. `func_80011D80` (init, zeroes both fields once -- round 18/19's `desc-life` site).
2. `func_800120DC` (`funcs_8.c`, round 18's find, threshold `descriptor+0x80C <= 0`).
3. **`func_80012B20`** (`funcs_8.c`, vaddr `0x80012B20`, new this round) -- a
   near-exact structural twin of `func_800120DC`: identical head-of-queue
   slot address formula (`descriptor + readIndex*20 + 0x848`), identical
   completionPtr read/write (`entry+0x858`/`*completionPtr =
   descriptor+0x82C`), identical `+0x844`/`+0x846` advance-and-decrement
   sequence, identical `func_800048EC` per-slot cleanup call -- but gated on
   a **different** condition of the *same* `descriptor+0x80C` countdown
   field: `slti $at, $t6, 0x5` (dequeue once the countdown drops **below
   5**), vs. `func_800120DC`'s `bgtzl ..., L_80012184` (skip dequeue
   whenever the countdown is **still above 0**, i.e. only dequeue at
   exactly `<= 0`). This is a materially looser/earlier completion
   condition, not a duplicate of the same function -- confirmed further by
   each function's own state-machine tail: both call into the same
   generic-driver dispatch table (offsets relative to `0x800B0000`, i.e.
   table `D_800AF560` from round 18), but `func_800120DC` transitions to
   table slot `-0xA88` (index 6) while `func_80012B20` transitions to slot
   `-0xA74` (index 11) -- genuinely different "next state" entries, i.e.
   these are two distinct sibling states of the DMA-manager's state
   machine, not the same code reached twice.

**How it's dispatched**: identically to `func_800120DC`
(round 18) and `func_80011D80`/`func_80011D10` (this round) -- **no
`jal`/literal-address call site exists anywhere in `RecompiledFuncs/*.c`**
for `func_80012B20` either (confirmed by grep both for the direct-call form
`func_80012B20(rdram` and for the label in every `asm/*.s` file, which
finds only the function's own `glabel`/`endlabel` lines). It is reached
only through the same runtime function-pointer table
(`D_800AF560`/`0x800AF560`, referenced by `%hi`/`%lo` relocation in
`func_80011D10`'s own dispatch and in the tail-transition jumps above) that
round 18 already established has no `.data`/`.rodata` text representation
anywhere in this checkout's `asm/` dump -- the disassembly here is
text-section-only, so the table's actual runtime *contents* (which
function pointer lives at which state-byte index) cannot be read
statically at all, only inferred from each function's own tail-transition
target offset as done above. This means "per-frame? via which object?" is
not answerable from source alone this round: **`func_80011D10` itself also
has no static caller** (same grep, same result), so even the object that
*drives* the whole state machine (round 18's still-open thread) remains
unidentified. Given repro19's own evidence (`func_800120DC` count 0, ring
demonstrably draining), `func_80012B20` is the far more likely candidate
for whichever real transfer type the capture exercised, but this is an
inference from elimination plus structural symmetry, not a proof -- exactly
why this round ships instrumentation on **both** functions (Task C.2)
rather than asserting one is "the" consumer and silently dropping the
other's coverage.

### Task B: the descriptor swap

Grepping the entire recompiled tree for the literal `sw reg, 0x1600($at)`
pattern (`lui $at, 0x800C` immediately preceding a `0x1600`-offset store to
the *same* register, checked programmatically across every file rather than
by eyeballing individual hits, since `0x1600` collides constantly with
unrelated stack-frame offsets) finds **exactly one static writer of RDRAM
`0x800C1600` anywhere in `RecompiledFuncs/*.c`: `func_80011D80`**, and only
on its allocation-succeeded branch. Reading the two calls this function
makes (both are `jalr`-through-computed-`t9` "trampoline" calls, so neither
shows up as a literal `func_XXX(rdram` call site in the generated C --
worth flagging since it means grepping for direct-call text alone
systematically misses this whole family of cross-overlay calls):

- `func_800027B0(obj, poolId=0, size=0x988, bitIndex=0)` is what actually
  allocates the descriptor: it sets a flag bit at `obj+0x20` (bit index =
  the 4th arg), calls the pool allocator `func_80001514(poolId, size)`, and
  stores the returned pointer at `obj + (bitIndex<<2) + 0x34` -- with
  `bitIndex=0` this is exactly `obj+0x34`, the descriptor field round 18
  named. `0x988` (=0x848 ring-header + 16*20 slot bytes) matches round 18's
  derived ring size exactly, confirming this is genuinely the ring's own
  allocation, not a coincidentally-similar unrelated call.
- Back in `func_80011D80`, if that call's return value (`v0`, the
  allocator's success/failure result, threaded through unchanged since
  `func_800027B0` never reloads `v0` before returning) is nonzero, the
  function writes the global object pointer (`0x800C1600 = obj`) and
  zero-initializes the ring's `readIndex`/`pendingCount`. If it is zero
  (allocation failed), the function instead calls a vtable function at
  `obj+0x10` and returns **without** touching `0x800C1600` or the ring
  fields at all -- an error/cleanup path, not a second writer.

No other function anywhere performs either store (`sw reg, 0x1600($at)`
targeting this global, or `sw v0, 0x34(reg)` reachable from it) --
confirmed by also checking whether `func_800027B0` has any *other* caller
besides `func_80011D80` (none found, same grep methodology). **This proves
the descriptor pointer has exactly one static write site in the whole
binary, but it does NOT prove that site only executes once at runtime**:
like `func_800120DC`/`func_80012B20`/`func_80011D10` above,
`func_80011D80` itself has no static `jal` caller either -- it too is
reached only through a runtime table this repo cannot read statically.
Round 18's "presumably at boot" phrasing for this call was exactly that, an
assumption, never actually traced back to a caller; repro19's own
descriptor-swap observation is direct runtime evidence *against* "boot-only,
persists forever." Whether the game legitimately re-runs this same
allocate-and-init function per map/scene (in which case the real recomp bug
is a timing/ordering gap -- `update_a`'s text-load enqueue landing on the
doomed old descriptor moments before the swap) or the recomp somehow
re-enters it spuriously (e.g. a heap/state-machine bug specific to the
recomp's own dispatch) **cannot be distinguished by static reading alone**
-- there is no table data, no caller, and no second static call site to
compare against. This is exactly the ambiguity Task D's own instructions
anticipated ("implement a fix only if unambiguous"); it is not, so no fix
was written this round. The `desc-life`/`desc-life-host` probes below are
built specifically to settle it on the next capture: if `func_80011D80`'s
own store fires more than once in a session, that proves re-invocation
(and pins down exactly which map transition triggers it, via the `map_rom`
field both probes log); if it fires exactly once yet the host-side watch
still sees the descriptor value change, the swap is happening through some
mechanism neither probe's static hook point covers, which would itself be
a significant new finding for round 20.

### Task C: probes shipped

All under the existing `LOD_ENABLE_NI0E_TRACE` flag (unchanged, already
`ON` in `build-ni0e`/`OFF` in `build`); no new flag was introduced this
round.

- **`desc-life`** (`RecompiledFuncs/funcs_7.c`,
  `lod_ni0e_desclife_probe`, hooked in `func_80011D80` immediately before
  its `0x800C1600` store): reads the pointer's *current* (pre-store) value
  the instant before the overwrite, logs `old_obj -> new_obj` plus the new
  descriptor (`new_obj+0x34`, already resolved by that point in the
  function) and `map_rom`. Tags the line `REINIT(non-null old pointer
  replaced!)` when `old_obj` was already non-null and different from
  `new_obj` (i.e. a genuine second-or-later invocation, not first-boot
  init) vs. `first-init` otherwise. Not rate-limited ("Always (rare)" per
  the task brief) -- own counter, `#%u`.
- **`desc-life-host`** (`src/main/main.cpp`, extending the existing Round-18
  `lod_ni0e_ring_pending_watch_vi_callback`): every VI, independently
  compares the raw global object pointer (`0x800C1600`) and its resolved
  descriptor (`+0x34`) against their previous-VI values; logs `old -> new`
  on any change (`frame`, both old/new obj and desc, `map_rom`), and also
  now folds a descriptor change into the pre-existing `ring-pending` line's
  own change-trigger (previously that line only re-fired on
  readIndex/pendingCount/head-fileid/head-completionPtr changes, which
  could in principle all coincidentally stay the same across a swap).
  Redundant with `desc-life` by design (task brief: "if none static, add a
  host per-VI change-watch" -- kept even though a static writer *was*
  found, since it also catches anything a future DMA-manager rework might
  route through a path the static grep would miss).
- **`ring-done`/`ring-pump` for `func_80012B20`**
  (`RecompiledFuncs/funcs_8.c`, `lod_ni0e_ring_done_probe_b`/
  `_notarget_probe_b`/`_ring_pump_probe_b`): byte-for-byte mirrors of round
  18's three `func_800120DC` probe sites (same tags -- `ring-done`/
  `ring-pump` -- so a plain `grep ring-done`/`grep ring-pump` catches
  output from both functions), with an added `fn=0x80012B20` field so a
  capture can tell which function actually produced each line, and its own
  first-60 + every-200 counters (kept separate from `func_800120DC`'s own
  counters so one function's traffic volume can never starve the other's
  rate-limited output window). Hooked at the three equivalent points in
  `func_80012B20`: the `t6 >= 5` "still pumping" branch (`ring-pump`), the
  `completionPtr == 0` dequeue branch (`ring-done` site `no-target`), and
  the `completionPtr != 0` dequeue branch immediately after the actual
  `*completionPtr = ...` store (`ring-done` site `complete`).
- **`textfile-life`** (`RecompiledFuncs/funcs_7.c` for the enqueue side,
  duplicated verbatim into `RecompiledFuncs/funcs_8.c` for the completion
  side, separate translation units): always-on (no rate limit), watches
  fileid `0x38` (named literally by the task brief) plus the *current*
  map's own text fileid, derived fresh every call rather than hardcoded --
  `record2 = 0x80192160 + map_id*16` (map_id read live from `sys+0x28D0`,
  sys base `0x801C82C0`, confirmed from `func_80145DA8`'s own `$s6` load a
  few lines above its round-17 `record2-gate` site), `record2+0xC`
  (halfword) is the same field that site pushes into the id-array/queue as
  the fileid. For map_id 4 this is `0x80192160 + 4*16 = 0x801921A0`,
  matching the task brief's own example exactly, confirming the derivation.
  Hooked at all four lifecycle points: `func_800116BC`'s two exits
  (`enqueue-dropped`/`enqueue-accepted`, `funcs_7.c`) and both consumer
  candidates' two dequeue outcomes each
  (`complete:func_800120DC`/`complete-notarget:func_800120DC`/
  `complete:func_80012B20`/`complete-notarget:func_80012B20`, `funcs_8.c`).
  Deriving the target dynamically (rather than hardcoding map_id 4's value)
  means the probe stays correct even if the next capture reproduces on a
  different map. Orphan detection (enqueued-but-never-completed) is not
  built as an automatic in-engine check -- deliberately: it would need
  cross-translation-unit shared state for marginal benefit when the same
  conclusion is already directly readable by eye from a capture's ordered
  log (an `enqueue-*` line for the watched fileid followed by a
  `desc-life`/`desc-life-host` swap line with no matching `complete*` line
  for that fileid afterward *is* the orphan proof).

### Task D: not triggered

Per the task brief's own instruction ("implement the minimal root fix...
only if [the root cause is] unambiguous... otherwise ship probes only and
say so"): **no fix was implemented this round.** Task B's own section above
lays out precisely why the ambiguity is real, not just an excuse to skip
work -- there is no static call site, no static table data, and no second
static write site anywhere in this checkout to compare against for either
open question (whether `func_80011D80` legitimately re-runs per scene, and
which of `func_800120DC`/`func_80012B20` is the real per-frame consumer for
a given transfer). `LOD_FIX_HENRY_TEXT_LOAD` was **not** created; no new
flag was added to `CMakeLists.txt` this round.

### Files changed (Round 19)

- `RecompiledFuncs/funcs_7.c`: new `lod_ni0e_desclife_probe` (tag
  `desc-life`) hooked into `func_80011D80`; new
  `lod_ni0e_textfile_watch_fileid`/`_is_watched`/`_life_probe` helpers (tag
  `textfile-life`) hooked into both of `func_800116BC`'s exit points, all
  under the existing `LOD_ENABLE_NI0E_TRACE` block.
- `RecompiledFuncs/funcs_8.c`: added the missing `#include <stdbool.h>` the
  file's `LOD_ENABLE_NI0E_TRACE` block needed (present in `funcs_7.c`
  already but not here; the initial build caught this as a hard compile
  error, fixed before the build reported below); new
  `lod_ni0e_ring_done_probe_b`/`_notarget_probe_b`/`_ring_pump_probe_b`
  (tags `ring-done`/`ring-pump`, `fn=0x80012B20`) hooked into
  `func_80012B20`'s three equivalent sites; new
  `lod_ni0e_textfile_watch_fileid`/`_is_watched`/`_life_probe` (duplicated
  from `funcs_7.c`, separate TU) hooked into both `func_800120DC`'s and
  `func_80012B20`'s two dequeue-outcome sites each.
- `src/main/main.cpp`: extended `lod_ni0e_ring_pending_watch_vi_callback`
  with the `desc-life-host` change-watch on `0x800C1600`/`+0x34` and folded
  that change into the existing `ring-pending` line's own trigger condition.
- `docs/issue27-31-ni0e-findings.md`: this section.

### Build/deploy (Round 19)

- `cmake --build build-ni0e --target LodRecomp -j8`: first attempt hit the
  now-familiar shared-`Makefile2`/spirv-cross hazard (rounds 12-18) before
  any of this round's own files were even reached. `build-ni0e/CMakeFiles/
  Makefile2` backed up to the scratchpad, bare `cmake build-ni0e` (no `-D`)
  run per the documented recovery; `diff` against the pre-reconfigure
  backup was **empty** (byte-identical) this time, and all five cache flags
  (`LOD_ENABLE_NI0E_TRACE`, `LOD_FIX_TEXT_MEASURE_GUARD`,
  `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`, `LOD_FIX_PAIR126_INPUT_RELEASE`,
  `LOD_FIX_HENRY_LOAD_HANDOFF`) confirmed unchanged. Rebuild then hit one
  real compile error from this round's own code: `funcs_8.c:175: error:
  unknown type name 'bool'` (the file's `LOD_ENABLE_NI0E_TRACE` block used
  `bool`/`static bool ...is_watched` without including `<stdbool.h>`,
  unlike `funcs_7.c` which already had it) -- fixed by adding the include
  (see Files changed above). Second rebuild succeeded: clean, only the
  pre-existing unrelated `sse2neon.h` `-W#warnings` warning.
- `cmake --build build --target LodRecomp -j8` (default tree, flags OFF):
  hit the identical shared-Makefile2 hazard again (a `build-ni0e`
  reconfigure does not also fix `build`'s copy). Same recovery applied
  (`Makefile2` backed up, bare `cmake build`, diff showed only line-order
  reshuffling of the same dependency edges -- not byte-identical like
  `build-ni0e`'s but semantically identical, same as every prior round's
  recovery -- cache flags confirmed unchanged: all round-13+ trace/fix
  flags `OFF` except the pre-existing default-on
  `LOD_FIX_PAIR126_INPUT_RELEASE`). Rebuild succeeded: clean except two
  pre-existing unrelated shader warnings (`FbReadAnyFullCS.hlsl.metal`
  unused-variable, `RSPProcessCS.hlsl.metal` unused-const-variable) and the
  pre-existing `sse2neon.h` warning -- none touch this round's files.
- `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums of both
  binaries verified identical afterward (`077ddd5feb9a4bdc823f563391800926`),
  so `./build/LodRecomp` now runs the Round-19-instrumented build.
- The game was not launched in this pass (per instructions); no live
  capture was taken. No commit was made (per instructions).

## Round 20 (2026-07-22): func_8016F310, a second unguarded text-draw pipeline structurally twin to func_801682D0, found and guarded

repro20's async text pipeline now completes end to end on the previously-
broken map; the one remaining crash (Signal 10 at rdram+0x8707F800, stable
across three captures, native stack `func_8016F310 (+964) <-
func_8003F72C <- func_80022BD0 <- func_80023724`) is a second, parallel
text-draw dispatcher never touched by rounds 14/15/18's guards. This round
reads it in full, confirms it is a near-exact structural twin of
`func_801682D0` (funcs_57.c, the function those rounds already hardened),
and applies the identical guard family to it plus its own draw primitive.

### Task 1: what func_8016F310 walks, and where its pointer comes from

**Same global, different path, confirmed by matching the exact instruction
sequence.** `func_8016F310`'s very first two instructions after its stack
frame allocation are `lui $s2,0x801A; lw $s2,-0x1134($s2)`
(`RecompiledFuncs/funcs_58.c`, vaddr `0x8016F31C`-`0x8016F320`) --
byte-for-byte the same load `func_801682D0` uses (`funcs_57.c`, vaddr
`0x801682DC`-`0x80168300`) to read the shared text-workspace pointer from
RDRAM global `0x8019EECC` (`0x801A0000 - 0x1134`). This is the same struct
round 15 named "the workspace" and already guards for `func_801682D0`/
`func_8016AE10` -- `func_8016F310` is a **third, previously-unaudited
reader** of it, reached via a completely different call chain.

**The struct layout it walks is identical to `func_801682D0`'s.** After an
unrelated float-vertex transform of its own `a0` argument (lines
0x8016F354-0x8016F3FC, operating on the caller's own object, not the
workspace) and a call to `func_801720EC`, `func_8016F310` reads the exact
same three fixed fields (`s2+0x10`, `s2+0x14`, `s2+0x18`, paired with
`s2+0x28`/`s2+0x2C`/`s2+0x30`) and the exact same variable-length array run
(`count = s2+0x20`, `objBase = s2+0x24`, `strBase = s2+0x38`, cursor-indexed
`obj = MEM_W(objBase+cursor,0)` / `str = MEM_W(strBase+cursor,0)`, both
reads direct and unguarded in the caller, exactly as round 18 found for
`func_801682D0`'s array loop) that `func_801682D0` reads from the same
struct -- but feeds each obj/str pair to **`func_8016FBB0`** instead of
`func_8016890C`. `func_8016FBB0` (funcs_58.c, vaddr `0x8016FBB0`) has
exactly one caller (`func_8016F310`, confirmed by grep) and only null-checks
its `a0` (`beq $a0,$zero,L_8016FE3C`) before immediately dereferencing it
(`obj+0xC` at `0x8016FBF0`, the first of six unguarded float loads at
`obj+0x8/+0xC/+0x10/+0x14/+0x18/+0x1C`) -- it has **no** range-check
equivalent to `func_8016890C`'s round-14 guard. This is the second half of
"no workspace validation": neither the container (`func_8016F310`'s own
`$s2`) nor the individual elements it hands off (via `func_8016FBB0`) were
checked anywhere in this call chain before this round.

**Where the pointer comes from, upstream.** `func_8016F310`'s only static
caller is `func_8003F72C` (`RecompiledFuncs/funcs_20.c`, vaddr
`0x8003F72C`), read in full. It **re-reads the same global itself**
(`lw $t6,-0x1134($t6)` at `0x8003F780`, a second, independent load of RDRAM
`0x8019EECC`) and null-checks it (`beql $t6,$zero,L_8003F9E0`) before
calling `func_8016F310` at all -- but, like every other null-only guard this
investigation has found guarding this global (round 15's original framing of
`func_801682D0`'s own game-code behavior), it never range-checks it. A stale
non-null pointer -- the same class of corruption rounds 18/19 documented
elsewhere in this investigation (freed/uninitialized heap data surviving a
Henry map transition, e.g. round 19's `0x11111111` descriptor-swap garbage)
-- sails straight through this null check and into `func_8016F310`'s own
unchecked `$s2` load.

**Why the fault lands at the stable address `0x8707F800`.** The crash's
native frame is directly inside `func_8016F310` (offset +964), not any
callee -- the same signature round 18 used to prove `func_801682D0`'s fault
was in its own array-loop reads, not inside `func_8016890C`. `0x8707F800`
being identical across all three captures is consistent with `$s2` itself
(or a field read at a small fixed offset from it, e.g. `$s2+0x10`) holding a
static/stale value near `0x8707F7F0`: `0x8707F7F0 + 0x10 = 0x8707F800`
lines up exactly with the very first workspace-dependent dereference in the
function (the `s2+0x10` field10 read at `0x8016F40C`). This cannot be pinned
to one exact instruction from source alone without disassembling the
compiled binary, but it is fully consistent with, and does not require any
mechanism beyond, "the same `0x8019EECC` global held a stale-but-nonzero
pointer this time," exactly the class of bug already fixed for
`func_801682D0`/`func_8016AE10`. The new `textpipe2` probe (Task 3) is built
to confirm this directly on the next capture.

### Task 2: the guard, and caller-audit notes

Three guards added, all under the existing `LOD_FIX_TEXT_MEASURE_GUARD`
flag, all in `RecompiledFuncs/funcs_58.c` (a separate translation unit from
`funcs_57.c`, so the helper logic is duplicated rather than shared via a
header -- same practice as round 19's `textfile-life` probe):

1. **Workspace guard** (`lod_text_guard_workspace_invalid`, byte-for-byte
   the same check `func_801682D0` uses): hooked immediately after
   `func_8016F310`'s own `$s2` load, right after this function's full
   prologue completes (`ra`/`s4`/`s3`/`s1` stack-saves plus its own
   `a1`/`a2`/`a3` spills -- see register-safety note below), jumping to
   `L_8016F478` on rejection -- the natural post-array-loop landing spot
   this function's own pre-existing `count<=0` skip already uses (it
   immediately overwrites `$s2` with an unrelated global's list head for the
   function's second, independent half). This mirrors `func_801682D0`'s
   round-15 placement and skip target exactly.
2. **Array-loop guard** (`lod_text_guard_array_invalid`, same check
   `func_801682D0` uses since round 18): hooked right before the array
   loop's `count<=0` check, only evaluated when `count>0` (mirroring round
   18's placement precisely), same `L_8016F478` target. `$s1`/`$s0` are
   already zeroed immediately before this check runs, so no extra register
   fixup is needed, exactly as round 18 found for `func_801682D0`.
3. **`func_8016FBB0` per-call pointer-range guard** (new this round, not
   present in the `func_8016890C` family before now because
   `func_8016890C` already had its own round-14 guard and `func_8016FBB0`
   did not): hooked immediately before `func_8016FBB0`'s own
   `beq $a0,$zero` null check, after its full prologue completes. Same
   "standard range check" (nonzero + within the emulated 8MB RDRAM span) and
   same early-return shape (including the `sw $s0,0x14($sp)` stack-save the
   null-check branch also performs, confirmed necessary by checking the
   function's own epilogue restore at `L_8016FE3C`) as `func_8016890C`'s
   guard. This closes the gap the workspace/array-base guards above do not:
   they validate `$s2` and the array run's base pointers, not each
   individual `+0x10`/`+0x14`/`+0x18` field or array element's own value --
   a single stale element could still reach an unguarded `func_8016FBB0`
   even with both container-level guards passing.

**Register-safety note (round-16/18 lesson applied).** Unlike
`func_801682D0`, whose prologue saves every callee-saved register (`ra`,
`s4`, `s3`, `s1`, `s0`, `f20`) *before* its own `$s2` load,
`func_8016F310`'s compiler-scheduled prologue interleaves them: `ra`/`s4`/
`s3`/`s1` are saved to the stack *after* the `$s2` load but *before* the
first `$s2`-dependent call. Placing the workspace guard immediately after
the `$s2` load (mirroring `func_801682D0`'s exact source position) would
therefore have skipped those saves, and the epilogue's restore at
`L_8016F6FC` would load stale/garbage stack bytes into `ra`/`s4`/`s3`/`s1`
instead of their real values -- a return-address-corruption bug that would
not have shown up until whichever caller `ra` pointed to next. The guard
was placed one step later in source order instead, immediately after the
full prologue (including this function's own `a1`/`a2`/`a3` stack spills,
which are pure local scratch never read past `L_8016F478` and so are safe
to skip either way) -- functionally identical skip behavior, register-safe
placement.

**Caller audit.** `func_8016F310`'s only caller, `func_8003F72C`
(funcs_20.c), branches on `$v0` (`beq $v0,$zero,L_8003F9DC`) and, on the
nonzero path, unconditionally dereferences it (`lh $t7,0x60($s1)` at
`0x8003F7D8`, `$s1` loaded from the stored `$v0`). `$v0` is produced
entirely by `func_8016F310`'s second half (from `L_8016F478` onward, an
independent walk of a *different* global at `0x8019E5E0` plus a fixed-size
array at another unrelated global, `0x801A0000-0x9FC`) -- none of it reads
`$s2` or anything the new guards touch. All three new guards leave `$v0`'s
computation path completely untouched, so this pre-existing (and, per the
`beq`, itself only null-checked, not range-checked) caller behavior is
exactly as safe or unsafe after this round's fix as before it -- no caller
change needed, matching every prior round's audit outcome for this guard
family.

### Task 3: the `textpipe2` probe

Added under the existing `LOD_ENABLE_NI0E_TRACE` flag (previously absent
from `funcs_58.c` entirely -- new `#if LOD_ENABLE_NI0E_TRACE` scaffolding
added, including the `<stdbool.h>` include funcs_8.c's round-19 build error
already flagged as necessary). `lod_ni0e_textpipe2_probe` hooks
`func_8016F310`'s entry, right after its own `$s2` load (and before the new
workspace guard's rejection check, so it fires even when the guard would
reject -- matching `func_801682D0`'s `figsrc-entry-fields` probe ordering).
Logs `$s2` plus the same field set `figsrc-entry-fields` already dumps for
`func_801682D0` (`+0x10`/`+0x14`/`+0x18` fixed slots, `+0x20`/`+0x24`/
`+0x38` array-run fields) plus `+0x4` (the very first field this function's
own code dereferences, fed as an arg to its first callee before anything
else runs). Defensively skips the field reads (logging only `$s2` itself)
when `$s2` fails the range check, so the probe cannot itself crash on the
exact bad-pointer case it exists to diagnose. Rate-limited first 40 + every
300th call, own counter (`lod_ni0e_textpipe2_calls`).

### Files changed (Round 20)

- `RecompiledFuncs/funcs_58.c`: new `LOD_FIX_TEXT_MEASURE_GUARD` helpers
  (`lod_text_guard_addr_ok`/`lod_text_guard_workspace_invalid`/
  `lod_text_guard_array_invalid`, duplicated from `funcs_57.c` for this
  separate translation unit) and log flags
  (`lod_text_guard_workspace_8016F310_logged`/
  `lod_text_guard_array_8016F310_logged`/`lod_text_guard_fbb0_logged`);
  workspace guard and array-loop guard hooked into `func_8016F310`;
  pointer-range guard hooked into `func_8016FBB0`. New
  `LOD_ENABLE_NI0E_TRACE` scaffolding (file had none before) plus
  `lod_ni0e_textpipe2_probe` (tag `textpipe2`) hooked into `func_8016F310`'s
  entry.
- `docs/issue27-31-ni0e-findings.md`: this section.

### Build/deploy (Round 20)

- `cmake --build build-ni0e --target LodRecomp -j8`: first attempt hit the
  now-familiar shared-`Makefile2`/spirv-cross hazard (rounds 12-19) before
  reaching this round's own files. `build-ni0e/CMakeFiles/Makefile2` backed
  up to the scratchpad, bare `cmake build-ni0e` (no `-D`) run per the
  documented recovery; `diff` against the pre-reconfigure backup showed only
  line-order reshuffling of the same dependency edges (not byte-identical
  this time, same as round 18's recovery), and all five cache flags
  (`LOD_ENABLE_NI0E_TRACE`, `LOD_FIX_TEXT_MEASURE_GUARD`,
  `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`, `LOD_FIX_PAIR126_INPUT_RELEASE`,
  `LOD_FIX_HENRY_LOAD_HANDOFF`) confirmed unchanged. Rebuild succeeded:
  `funcs_58.c` recompiled clean with zero warnings/errors (re-verified in
  isolation via a targeted `RecompiledFuncs`-only rebuild after `touch`ing
  the file), full `LodRecomp` link succeeded.
- `cmake --build build --target LodRecomp -j8` (default tree, flags OFF):
  `RecompiledFuncs` (including `funcs_58.c` with both flags compiled out)
  built cleanly; hit the identical shared-Makefile2 hazard on the full
  `LodRecomp` link. Same recovery applied (`Makefile2` backed up, bare
  `cmake build`, diff showed only line-order reshuffling of the same
  dependency edges, cache flags confirmed unchanged -- all round-13+
  trace/fix flags `OFF` except the pre-existing default-on
  `LOD_FIX_PAIR126_INPUT_RELEASE`). Rebuild succeeded: clean.
- `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums of both
  binaries verified identical afterward (`85a20540e6f8d44a36f9c0ed5f3c0fea`),
  so `./build/LodRecomp` now runs the Round-20-instrumented build.
- The game was not launched in this pass (per instructions); no live
  capture was taken. No commit was made (per instructions).

## Round 23 (2026-07-22): root-creation backtrace probe

Per round-22's design-doc conclusion (`docs/issue27-31-fix-design.md`
section 11) that the per-map framework build begins with a PARENTLESS root
create (`object_createAndSetChild`/`func_80002410`, `parent=0x00000000
id=0x1AB`, round-6 create probe) that never fires at all on the fatal
loaded-Henry transition, and that this root's caller is the transition
sequencer this investigation has never been able to name statically because
it is reached only via runtime function-pointer dispatch (the same ceiling
documented for the DMA-manager chain in round 21 section 10.4 and for
`GameStateMgr_dispatch` in round 22 section 11.3): `lod_ni0e_create_probe`
(`RecompiledFuncs/funcs_1.c`) now calls a new
`lod_ni0e_create_root_bt_probe` helper whenever `parent==0`, logging a
native backtrace (`create-root-bt`-tagged, frames 1-9, `execinfo.h`/
`backtrace_symbols`, the same mechanism round 10 used to identify the
cutscene launcher) capped at 12 per session. No other game-side change was
made this round. Build: hit the now-familiar shared-`Makefile2`/spirv-cross
hazard in both `build-ni0e` (first attempt) and `build` (first attempt);
recovered per the documented protocol in each case (`Makefile2` backed up
to the scratchpad, bare `cmake build-ni0e`/`cmake build` reconfigure with no
`-D` flags, diffs showed only line-order reshuffling with all cache flags
confirmed unchanged, `LOD_ENABLE_NI0E_TRACE` remaining `ON` in `build-ni0e`
and `OFF` in `build`). Both `LodRecomp` links then succeeded cleanly;
`funcs_1.c` was independently re-verified to compile with zero
warnings/errors via a targeted `RecompiledFuncs`-only rebuild after
`touch`ing the file. `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5
checksums of both binaries verified identical afterward
(`737733f7f836e62378797a0dfdb3a1ff`), so `./build/LodRecomp` now runs the
Round-23-instrumented build. The game was not launched (per instructions);
no live capture was taken. No commit was made (per instructions).

## Round 24 (2026-07-22): `ni_system_handler` state machine mapped end to end

Full writeup, quoted code, and the recommended fix live in
`docs/issue27-31-fix-design.md` section 12 (this section is a pointer/
summary, matching where rounds 21/22's deep analysis went).

**Correction to this round's own starting premise**: the round brief
asserted the fatal loaded-Henry transition skips `func_80011D80` (DMA-pump
re-init). Re-checking `/tmp/ni0e_repro22.log` this round found the opposite:
a `desc-life REINIT` line precedes **every** `MAP_OVL` load in that log,
including the fatal `MAP_OVL #6` (REINIT at line 2814, immediately before
`MAP_OVL #6` at line 2846) — no exceptions. `func_80011D80` is not skipped.

**What the reading found instead**: `ni_system_handler` (`0x8001B9A0`,
`RecompiledFuncs/funcs_13.c`) is one state of the id-`0x009` "NI-system"
object's per-frame state machine (dispatch wrapper `func_8001B718`, table
`0x800B3834`) — a *different* object and table from `func_80011D80`'s own
DMA-pump object (table `D_800AF560`), linked only via the global active-pump
pointer `*(0x800C1600)`. `ni_system_handler`'s own tail unconditionally
advances its state index (via `object_curLevel_goToNextFuncAndClearTimer`,
`func_80001CE8`, read in full this round and found to have no data-driven
branch of its own) and immediately tail-dispatches to the next state in the
same call — so it runs exactly once per object lifetime, then permanently
hands off to `overlay_system_create` (`0x8001BA78`), which re-invokes itself
every frame thereafter via `obj+0x10`, gated solely by `sys+0x2B24`
(`0x801CADE4`).

`sys+0x2B24` is written **exclusively** by `func_80012ED0`
(`RecompiledFuncs/funcs_8.c`), which `ni_system_handler` calls at most once.
Read in full, it has a 3-way branch: align-fail (no effect); an immediate
synchronous `DMA_ROMCopy` that sets `sys+0x2B24` in the same call (healthy
case); or — the identified divergence — creating a **new child object (id
`5`) under the active DMA-pump object** while **explicitly clearing**
`sys+0x2B24`, deferring the completion signal to that child's own future
per-frame dispatch. That child's own driving mechanism was not found this
round (same "function-pointer-only, no static `jal` caller" ceiling rounds
21/22 already hit for the DMA-manager chain and `GameStateMgr_dispatch`) —
but since it is created as a child of the *same* pump object those two
investigations already flagged, this round's finding is a plausible
unification of all three "never completes" symptoms into one root cause:
whatever stops the pump's own child-object broadcast from running after a
loaded-Henry map load.

Corroborating evidence already in `/tmp/ni0e_repro22.log`: the last
`[PIDMA] start`/`posted` pair in the whole log is at line 2339-2340, right
after `MAP_OVL #5` loads — zero further PI-DMA starts occur afterward,
including through the fatal `MAP_OVL #6`, even though `ring-pending` lines
keep advancing by re-reading byte-for-byte identical stale data left over
from a recycled descriptor's first use.

**Probes added** (tag `nisys-state`, `LOD_ENABLE_NI0E_TRACE`, no new CMake
option): `site=ni_system_handler` and `site=overlay_system_create` (rate
limited, first 60 + every 300), and `func_80012ED0`'s three branch exits
`site=align-fail`/`site=immediate-dma`/`site=defer-child` — the last one,
Task 2's identified divergent branch, is always logged, not rate-limited.

**Recommended minimal fix** (not implemented this round, per instructions):
do not patch `sys+0x2B24` or add a timeout to `overlay_system_create`'s
guard (another forced-state band-aid); find and fix why the active pump
object's child-dispatch broadcast stops running on the loaded-Henry path —
likely one root fix that closes out the DMA ring (round 21), the
`GameStateMgr_dispatch` node-pool hypothesis (round 22), and this round's
`ni_system_handler`/`overlay_system_create` gate simultaneously.

**Build**: `build-ni0e/CMakeFiles/Makefile2` hit the familiar
shared-Makefile2/spirv-cross hazard (rounds 12-23); bare `cmake build-ni0e`
recovery produced a byte-identical `Makefile2`, all cache flags unchanged.
One real link failure of this round's own making (three new probe functions
initially declared `static` instead of `extern "C"`) was found and fixed;
both `build-ni0e` and `build` (flag `OFF`, 40-line but pure-reorder
`Makefile2` diff) then linked clean, zero new warnings on the three changed
files. `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums
verified identical (`e44f50e21aafe1ed36f7eafbe848cbb6`). The game was not
launched (per instructions); no live capture was taken. No commit was made
(per instructions).

## Round 25 (2026-07-22): pump-stall Task A resolved decisively; a live hang dump directly contradicts the assumed "lost completion" mechanism; probes-only

Full writeup in `docs/issue27-31-fix-design.md` section 13 (this is a
pointer/summary). Working from a live lldb + 8MB RDRAM dump of an actually
hung session, cross-checked byte-for-byte against the ground truth's own
numbers (readidx/pending/progress/chunks-remaining, `sys+0x2B24`) to
validate the reading methodology before drawing new conclusions from it.

**Task A.1, resolved with full confidence**: `func_80012D24`
(`RecompiledFuncs/funcs_8.c`) is **all-chunks-in-one-call**, not
one-chunk-per-dispatch — a MIPS-level `while` loop that calls `DMA_ROMCopy`
once per `0x800`-byte chunk and only returns to its caller once
`desc+0x83C` (chunks remaining) reaches 0. Its per-chunk callback
(`func_800A761C`) was verified this round to contain no `osRecvMesg`/
`osSendMesg` call anywhere in its own ~970-line body or its two direct
callees, ruling it out as a second blocking-wait site. This directly
confirms the ground truth's own reading: the frozen thread is synchronously
inside one call to `func_80012D24`, not "stopped being dispatched."

**Task A.2**: unresolved, same hard ceiling rounds 21/22/24 already
documented — `func_80012AB0` (the sub-state wrapper) is driven by
`func_80011D10`, which like every function in this chain has zero static
`jal` callers; only reachable via the runtime table `D_800AF560`, whose
contents this checkout's `asm/` dump cannot show.

**New finding that changes Task B**: the exact binary that produced this
hang (MD5-matched to round 24's build) already had `LOD_FIX_DMA_COMPLETION`
(round 21's retry-on-failure fix) and `LOD_ENABLE_PIDMA_TRACE` both `ON`,
and the hang happened anyway. Reading the frozen `OSMesgQueue` struct
(`0x800C5D18`) directly out of the RDRAM dump found it **fully quiescent**:
`validCount=0`, `msgCount=1` (sane), `blocked_on_recv=0x00000000` (NULL —
no thread currently registered as waiting on this queue). This is
inconsistent with a thread genuinely, currently parked inside `do_recv`'s
blocking wait for *this* queue (which unconditionally calls
`thread_queue_insert(blocked_on_recv, self)` as the first statement of each
loop iteration) — round 21's two candidate mechanisms (a stale unclaimed
message / a corrupted queue) are both directly ruled out by this same
reading, since neither would produce this exact quiescent state. Writing
another guessed variant of round 21's fix against a queue the dump shows is
not stuck, full, or corrupt would likely be a no-op or a band-aid, so per
the task's own explicit fallback, no host-layer fix was made this round.

**Probes shipped instead** (both `LOD_ENABLE_NI0E_TRACE`-gated, both new to
`RecompiledFuncs/funcs_8.c`): `pump-tick` (hooked in `func_80012AB0`, logs
obj/state-count/state-index plus the pump's ring descriptor's
readidx/pending/progress/chunks-remaining, first 80 + every 200) and
`pump-chunk` (hooked at `func_80012D24`'s entry and single exit, logs
descriptor/progress/chunks-remaining before and after). A future capture
filtered to `grep 'pump-tick\|pump-chunk'` will show directly whether the
fatal transfer's `func_80012D24` call is entered-but-never-exits (confirms
§13.1, points to a specific-queue argument-register dump as the next step)
or never entered at all (points to §13.2's dispatch-stop scenario and the
`D_800AF560` ceiling instead).

**Side finding (not fixed, cosmetic)**: `LOD_NI0E_RING_READIDX_OFF`/
`PENDING_OFF` (`src/main/main.cpp`, `0x844`/`0x846`) are swapped relative to
their actual semantics — confirmed by reproducing the ground truth's own
`head slot fid=0x33` only when using `+0x846` as the read index, not
`+0x844`. Pre-existing probes using these constants are unaffected in
*behavior* (only the printed field *labels* are swapped) and were left
untouched as out of scope for a minimal patch; this round's own new probes
use the corrected mapping.

**Build**: `build-ni0e/CMakeFiles/Makefile2` reconfigure produced a
byte-identical diff (no spirv-cross hazard this round); `build`'s reconfigure
produced a 40-line but pure-reorder diff (sorted diff empty), same benign
pattern as rounds 19/22/24. Both trees linked clean; `funcs_8.c` compiled
with zero warnings/errors of its own (verified via full build and a
targeted rebuild). `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5
checksums verified identical (`1279988d901bdeabc7a2169a76a6076e`). The game
was not launched (per instructions); no commit was made (per instructions).
