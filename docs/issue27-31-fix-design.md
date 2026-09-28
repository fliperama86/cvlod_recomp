# Issue #27/#31 Fix Design: Load-Path Scenario-Controller / Exec-Flags Root Cause

Round 9 (2026-07-21). Reading-only round; no source changes except this file.
Builds on `docs/issue27-31-ni0e-findings.md` and the round-8 differential capture
summarized in project memory (`issue27-portal-crystal.md`). Where this round's
static/log evidence sharpens or contradicts a round-8 hypothesis, that is called
out explicitly rather than silently restated.

## 0. Executive summary

Two largely-independent problems are tangled together under "issue #27/#31":

1. **The scenario-controller creation gap** (object id `0x083` and siblings
   `0x062`/`0x196`/`0x0F0`/`0x0F1`, never created on Henry-loaded sessions).
   Round 8 attributed this to NI pair 104 (object `0x00F`) "skipping a
   create-0x083 step" on the load path. **This round's exhaustive static read
   of pair 104 refutes the specific attribution**: pair 104 contains exactly
   two direct call sites to `object_createAndSetChild`, both creating id
   `0x190`, never `0x083`. The real creator of `0x083` is still unidentified
   (it is demonstrably data-driven, not a code immediate, anywhere in
   `RecompiledFuncs/`), but dynamic log evidence pins its creation to a
   one-shot event on the **title map**, immediately before the
   title -> gameplay map swap - i.e. it is very unlikely to be part of any
   *per-frame* NI-pair-104 state at all. See §2.

2. **The exec-flags activation-chain stall** (`sys+0x2908` stuck at
   `0x20000000`, forced to `0x38000000` by `LOD_FIX_PAIR126_INPUT_RELEASE`).
   This round fully maps the mechanism: a 4-field "activation control block"
   at `0x8019D180` gates a single consumer, `bgState_activate`
   (`func_80146240`), which only sets `exec |= 0x10000000` when **both**
   `*(0x8019D194)` and `*(0x8019D198)` are nonzero. Two independent producer
   families feed those two fields, and **NI pair 126's own state machine has
   a data-table-driven early-exit branch** (`ni_ovl_126_func_0F000070`,
   `0x0F0000C4`/`0x0F0000D0`) that skips building pair 126's fade/child
   structure entirely when a per-entry "expected scene" table lookup fails to
   match `sys+0x28D2`. This exactly matches the pre-existing Issue #26
   evidence ("`pair126.destroy` never fired... no fade child/link") and is
   *distinct* from the pre-existing Issue #23 evidence ("`pair126.destroy`
   fired... but did not restore the normal flags"), meaning **there are two
   separable failure modes already documented in `RECOVERY_TO_GAMEPLAY.md`,
   and this round's static read explains the mechanism of one of them
   precisely and narrows the other to a specific 3-producer gate.** See §3-4.

These are plausibly the same underlying "load path never rebuilds what a
title -> gameplay handoff builds" defect wearing two faces, but the evidence
does not yet prove a single shared root cause. The fix proposals in §5 are
scoped accordingly: a narrow, high-confidence fix for the pair-126 branch,
plus an instrumentation-gated path to close out the exec-flags gate and the
`0x083` creator with a following round's dynamic capture (this round could not
add build changes).

## 1. Terms and addresses used throughout

| Symbol | Address | Source |
|---|---|---|
| sys base | `0x801C82C0` | established, `game-engine-architecture.md` |
| `sys+0x2908` exec flags | `0x801CABC8` | established |
| `sys+0x28D0` / `sys+0x28D2` | `0x801CAB90` / `0x801CAB92` | "scene"/"scene2" fields, confirmed this round (§3.1) |
| Activation control block | `0x8019D180` (24 bytes, cleared as one `bzero(0x24)`) | confirmed this round, `func_80145BD0` (`bgState_init`) |
| - `+0x14` = `0x8019D194` | | incremented by `ovl47_gateProducer` (§4.2) |
| - `+0x18` = `0x8019D198` | | incremented by `func_80141E80` and `func_80141968` (§4.3) |
| `func_80001CE8` | `0x80001CE8` | generic per-object "advance state list + clear timer" finalize helper (called from dozens of state handlers, including all producers below) |
| `bgState_*` family | `0x80145AF0`-`0x801462A4` | dispatch chain for object id `0x1AB` (root of the "Henry root family" 0x1AB..0x1D0), table at `0x8018D3B0`; named/split by pre-existing `LOD_ENABLE_BGSTATE_TRACE` infra (`src/main/ignored_func_stubs.cpp:6607-7250`), which predates this issue (built for a 2026-06 "gs=5 recovery" investigation) but happens to cover exactly this chain |

## 2. Task 1: object 0x00F / pair 104 - refined, not confirmed

### 2.1 What was checked

Pair 104's recompiled functions live in `RecompiledFuncs/funcs_212.c:6072-13071`
(`ni_ovl_104_schedule_dispatch` through `ni_ovl_104_func_0F002574`; pair 105
starts cleanly at line 13072). Object `0x00F`'s dispatch-table entry is
`ni_ovl_104_func_0F000B60` (`funcs_212.c:8160`), confirmed by the recursion-depth
wrapper pattern (`obj+0xE` nesting counter, `obj+8`/`obj+9` per-level
call-count/state-index bytes, jump through a table at `0x0F002780`) that is
identical to `ni_ovl_104_schedule_dispatch` at `0x0F000000` (table `0x0F0026F8`,
presumably object id `0x00E`, matching the memory note that 0x00E and 0x00F
"share the chain").

`object_createAndSetChild` is `funcs_1.c:1081` (vaddr `0x80002410`, confirmed by
reading its body: `a0`=parent, `a1`=id word). Every direct call to this
function anywhere in the recompiled code shows up as the literal text
`0X2410` at the call-target computation, regardless of how the `a1` id
argument itself is produced (immediate or memory load) - the call-site
literal is independent of the argument. A scoped grep of
`funcs_212.c:6072-13072` for that literal found exactly two hits:

- `funcs_212.c:6472` (vaddr `0x0F000230`, inside `ni_ovl_104_func_0F000130`,
  reached from state index 2): `a1 = 0x2190` (immediate) → id `0x190`.
- `funcs_212.c:8427` (vaddr `0x0F000CF8`, inside `ni_ovl_104_func_0F000C8C`,
  a camera-zoom/fade sub-state machine of object `0x00F` itself): `a1 = 0x2190`
  (immediate) → id `0x190`.

Both create id `0x190`, with `a0` = the calling object itself (`s0`/self).
**No literal `0x83` (with or without a `0x2000` class-flag bit, in any base
register) appears anywhere in `RecompiledFuncs/*.c`** - confirmed by an
unscoped repo-wide grep, not just within pair 104. This rules out a
hardcoded-immediate creation of id `0x083` anywhere in the recompiled codebase,
not only in pair 104.

### 2.2 The generic template-array creation mechanism, and why it is not involved

`object_executeChildObject` (`funcs_7.c:2122`, vaddr `0x80010B84`) and
`object_activateChildren` (`funcs_7.c:2295`, vaddr `0x80010C80`) implement the
documented "GSS_SLOT template array" mechanism (`obj+0x34..0x74`, 16 words):
for each nonzero slot, `id = word & 0x7FF`; if the NI file-descriptor table
`0x800AEA8C[id]` is nonzero (NI-class object), the slot is marked deferred
(`|= 0x40000000`) and *not* created yet; `object_activateChildren` later
finishes deferred slots. Both functions already carry
`LOD_ENABLE_NI0E_TRACE` round-4/5/6 probes (`lod_ni0e_manager_exec_probe`,
`lod_ni0e_manager_create_probe`) that log **unconditionally for the first 400
evaluations of the session**, independent of id.

Cross-referencing the existing capture log `/tmp/ni0e_fresh_henry.log`
(fresh-Henry-save-then-load session, the exact scenario round 8 analyzed):

```
747:[NI0E_TRACE] create #232 parent=0x80326854 id=0x083 result=0x80327334 map=0x008363B0
750:[NI0E_TRACE] ensure-map id=0x083 s4=0x19D0 obj=0x80327334 call=5039 (new)
```

`0x80326854` is exactly object `0x00F`'s own address (confirmed by the earlier
`create #116 parent=0x80326528 id=0x00F result=0x80326854`). **There is no
`manager-exec`/`manager-create` line for `obj=0x80326854` anywhere in the log**
(`grep -c "manager-exec.*obj=0x80326854"` → 0), which - given the
unconditional-for-first-400 logging policy and that create `#232` occurs well
inside that window - proves the `0x083` creation did **not** go through
`object_executeChildObject`/`object_activateChildren` for object `0x00F`.

### 2.3 Conclusion for Task 1

`0x083` is created by a **direct** call to `object_createAndSetChild` with
`a0` (parent) resolving to object `0x00F`'s address and `a1` (id) supplied by a
memory read, not an immediate. Since pair 104's own code contains no other
direct call site to `0x80002410` besides the two id-`0x190` sites above, **the
caller is not pair 104's own code**. The parent address most likely reached
the caller via a global/cached pointer or a fresh
`object_findFirstObjectByID(0x00F)` lookup, not via `self`. The creation
happens as the **last object created on the title map** before the
`[MAP_OVL]` swap to the first real gameplay map (`create #232` is
immediately followed by `create #233 parent=0x00000000 id=0x1AB`, the very
first object of the new map's root scene) - i.e. it looks like a one-shot
"commit new game, hand off to gameplay" trigger rather than a per-frame
pair-104 state decision.

**This round could not identify the actual caller** without either (a) adding
new instrumentation (out of scope this round) or (b) inspecting the ROM's
`.data` segments directly for the id, which is not visible via
`RecompiledFuncs/*.c` grep since GSS_SLOT/child-template tables are embedded
binary data, not disassembled instructions. Flagging this as the concrete
open item for the next round: **do not re-chase pair 104 as the site of a
missing branch; instead find what runs on the title map immediately before
the map-load swap in a fresh "new game" session** (candidate: the
new-game-confirm handler off the title/character-select menu, or a
`object_findFirstObjectByID(0x00F)` call site anywhere in common code -
neither was located this round).

## 3. Task 2: pair 126's own state machine - the found divergent branch

### 3.1 Function map

`ni_ovl_126_*` functions live in `RecompiledFuncs/funcs_218.c:4451-5542`:

| Function | vaddr | Line | Shim hook name |
|---|---|---|---|
| `ni_ovl_126_func_0F000000` | `0x0F000000` | 4451 | `pair126.entry` |
| `ni_ovl_126_func_0F000070` | `0x0F000070` | 4517 | `pair126.init` / `pair126.0F000070` |
| `ni_ovl_126_func_0F000318` | `0x0F000318` | 4937 | `pair126.0F000318` (fade_in) |
| `ni_ovl_126_func_0F000490` | `0x0F000490` | 5191 | `pair126.0F000490` (wait) |
| `ni_ovl_126_func_0F000590` | `0x0F000590` | 5381 | `pair126.0F000590` (fade_out) |
| `ni_ovl_126_func_0F00064C` | `0x0F00064C` | 5515 | `pair126.destroy` |

`0x0F000000` is the same recursion-depth dispatch wrapper pattern as pair
104's `0x0F000B60` (table at `0x0F000A88`). `0x0F00064C` (state_destroy) is
itself trivial - it just re-invokes `obj+0x10` (the object's own current
dispatch pointer) - so it is a convenient **hook point** for "pair 126 is
about to tear down," not where meaningful destroy logic lives.

### 3.2 The exact divergent branch

`ni_ovl_126_func_0F000070` (state_init), `funcs_218.c:4517-4936`:

1. `funcs_218.c:4528-4545` (`0x0F000080`-`0x0F000098`): if `obj+0x70`
   ("entry_source", a linked creation-template pointer) is nonzero,
   `obj+0x34` ("entry_index") is set from `entry_source+0x18` (u16); otherwise
   `obj+0x34` keeps whatever value it already had.
2. `funcs_218.c:4547-4560` (`0x0F00009C`-`0x0F0000B4`): computes
   `table_off = 0x844 + entry_index*12` and reads
   `table[table_off+4]`/`table[table_off+5]` (bytes `match_a`/`match_b`) from
   pair 126's **own embedded data**, physical address `0x0F000844 + ...`
   (12-byte stride, so `entry_index` indexes up to `(0x?? - 0x844)/12`
   entries - the existing `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN` shim
   guards this with `entry_index >= 23` → skip, so the table has ≤23 rows).
3. `funcs_218.c:4551` (`0x0F0000A4`): reads `stage_global = *(int16_t*)(0x801D0000 - 0x546E)`.
   Arithmetic: `0x801D0000 - 0x546E = 0x801CAB92 = sys+0x28D2` - i.e. this is
   the **"scene2"/entrance-index field**, confirmed by cross-referencing the
   pre-existing `lod_bgstate_trace_snapshot`'s `scene2` field (same base
   arithmetic, `ignored_func_stubs.cpp:6686-6687`).
4. `funcs_218.c:4562-4582` (`0x0F0000B8`-`0x0F0000D4`, the divergent branch
   itself):
   ```
   t2 = table[table_off + 4]      // match_a
   if (stage_global == t2) goto L_0F0000F0     // MATCH -> continue full init
   t3 = table[table_off + 5]      // match_b
   if (stage_global == t3) goto L_0F0000F0     // MATCH -> continue full init
   // NEITHER MATCHES (fallthrough, funcs_218.c:4585-4602 / vaddr 0x0F0000D8):
   t9 = obj->0x10  (obj's own current dispatch fn ptr)
   call t9(a0=obj)
   goto L_0F000308   // EARLY RETURN - the rest of init (0x0F0000F8 onward) never runs
   ```
5. `L_0F0000F0` (both match cases) also calls `obj->0x10` once, but then
   **falls through** into the rest of the function
   (`funcs_218.c:4612` onward, `0x0F0000F8`-`0x0F000304`), which builds a
   window/fade child structure and stores it at `obj+0x24`
   (`funcs_218.c:4837`, `MEM_W(0x24, obj) = v0`) - the exact field the
   existing `lod_pair126_init_returned_without_fade` shim heuristic checks
   as `child` (its offset `0x24` read matches `game-engine-architecture.md`'s
   `figures[0]`/child-link documentation).

**This is precisely the mechanism `lod_pair126_init_returned_without_fade`
(`src/main/ni_overlay_loader.cpp:3856-3892`) exists to detect**: when the
mismatch branch is taken, `obj+0x24` (`child`), `obj+0x54` (`linked`),
`obj+0x38`/`0x48`/`0x4C`/`0x50` (`timer`/`limit`/`frame`/`done`) all stay at
their post-`object_allocate` zero state, `entry_index` (`obj+0x34`) is
nonzero/in-range (it was set in step 1 regardless of the match outcome), and
`state0_count`/`state0_index` (`obj+0x08`/`0x09`) show exactly one visit to
state 0. This is a byte-for-byte match to the shim's guard condition, and to
the pre-existing **Issue #26** evidence in `RECOVERY_TO_GAMEPLAY.md:373`
("Pair `126` init selected entry `14`, found no fade child/link... `pair126.destroy` never fired").

### 3.3 The already-built (but disabled) narrower fix

`LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN` (default OFF,
`src/main/ni_overlay_loader.cpp:3784-3853`) already implements exactly the
correct-shaped root fix for this branch: before calling state_init, it
re-derives `entry_index` the same way state_init does, reads the same
`match_a`/`match_b` table row, and if `stage_global` (`sys+0x28D2`) matches
**neither**, it **overwrites `sys+0x28D2` with `match_b` (or `match_a` if
`match_b==0`)** so the game's own comparison at step 4 succeeds naturally.
This is a real fix candidate, not merely a diagnostic - but it is (a)
default-off, (b) has no capture log in the repo demonstrating it was ever
validated end-to-end, and (c) mutates a global (`sys+0x28D2`) rather than
explaining *why* it diverges from the table's expectation, which is itself a
band-aid risk (see §5).

### 3.4 Two distinct failure modes already on record

`RECOVERY_TO_GAMEPLAY.md:355-378` (Issue #23 / #26, 2026-06-27/07-02)
documents **two different symptom shapes** for the same `sys+0x2908` stuck
pattern, both fixed by the same `LOD_FIX_PAIR126_INPUT_RELEASE` shim but at
its two different hook points:

- **Issue #26** (Outer Walls elevator): `pair126.destroy` *never fired*; init
  selected a valid entry but found no fade child/link → this is exactly §3.2's
  early-bail branch.
- **Issue #23** (Henry coffin): `pair126.destroy` *did fire* (full state
  machine ran: init → fade_in → wait → fade_out → destroy, fade child
  presumably built and torn down normally) but exec flags were **still**
  stuck at `0x20000000` afterward.

Issue #23's evidence proves the exec-flags stall has a **second, independent
cause** that is not fixed by making pair 126's own state machine complete
successfully. That second cause is the subject of Task 3 / §4.

## 4. Task 3: why exec sticks at 0x20000000 - the activation-chain gate

### 4.1 The consumer: `bgState_activate`

`funcs_45.c:6814-6884`, vaddr `0x80146240`:

```
v0 = 0x8019D180                       // activation control block
t6 = *(v0 + 0x14)                     // 0x8019D194
if (t6 == 0) return;                  // GATE 1
t7 = *(v0 + 0x18)                     // 0x8019D198
if (t7 == 0) return;                  // GATE 2
sys->0x2908 |= 0x10000000;            // exec flags
sys->0x2B10  = 1;
call func_80001CE8(a0+8, a1+0xE);     // finalize/advance own state
```

Both gates (`0x8019D194` and `0x8019D198`) must be nonzero. Neither is set
anywhere except by the two producer families below plus the one-time
`bgState_init` (`func_80145BD0`, `funcs_45.c:5739`) that `bzero`s the whole
24-byte block (confirmed at `funcs_45.c:5769`, vaddr `0x80145C04`,
`bzero_recomp(0x8019D180, 0x24)`).

Note `LOD_PAIR126_MISSING_GAMEPLAY_FLAGS` in the existing shims is
`0x38000000 & ~0x20000000 = 0x18000000` - i.e. the shims restore **both**
`0x10000000` (this gate's bit) and `0x08000000` (set by some other,
unidentified path not traced this round). This round only pinned down the
`0x10000000` producer chain; the `0x08000000` producer was not located.

### 4.2 Producer of `0x8019D194` (`+0x14`): `ovl47_gateProducer`

`funcs_47.c:8209-...`, vaddr `0x8014B370` (despite the pre-existing name
implying NI overlay 47, this is **common code**, always resident - the name
appears to be a leftover heuristic guess from whoever originally split/named
this function, not evidence it is tied to NI pair 47's TLB-resident content).
Body increments `0x8019D194` at `funcs_47.c:8537` (vaddr `0x8014B598`) only
after a large block of floating-point work
(`funcs_47.c:8260`-`8530`) that reads a per-object "track/rail" structure
(`obj+0x34+0x4`, fields at `+0x18`/`+0x1C`/`+0x20`, i.e. 3D points) and does
distance/division math consistent with a **camera path / fly-through
interpolation** completing. This function is reached via
`ovl47_localDispatch` (`funcs_47.c:8143`, vaddr `0x8014B300`), the same
recursion-wrapper pattern seen everywhere else, meaning it is itself one
state of some other object's state machine - the owning object id was not
identified this round.

**Working hypothesis (not confirmed dynamically this round):** if this
"camera path" is specifically the new-game opening fly-through/cutscene
camera (which never plays when continuing a save - the player is dropped
straight into the saved room), then `0x8019D194` would legitimately never be
incremented on **any** load-game path, by design, in the recomp's current
object graph - which would make this a structural, always-on divergence for
every load-game transition, not a Henry-specific bug. This is consistent
with the observation that the destroy-hook shim fires unconditionally
whenever exec flags are found stuck, including in the Issue #23 case where
pair 126 itself completed normally.

### 4.3 Producers of `0x8019D198` (`+0x18`): two call sites

- `func_80141E80` (`funcs_44.c:758-853`, vaddr `0x80141E80`): calls
  `func_801424C4` (a "still pending?" test, name/purpose not traced this
  round) on `obj+0x34` and, doubly-indirected, on `*(obj+0x34)+0x38`; only if
  **both** calls return 0 (not pending) does it increment `0x8019D198`
  (`funcs_44.c:828-838`, vaddr `0x80141EC8`-`0x80141EE0`) and finalize via
  `func_80001CE8`. Shape strongly suggests "wait for a child/background load
  to finish, then signal."
- `func_80141968` (`funcs_43.c:14259-...`, vaddr `0x80141968`): this is
  object id `0x1AE`'s own init state (one of `bgState_create`'s six
  `0x1AC/0x1AD/0x1B2/0x1B1/0x1D0/0x1AE` children, confirmed by cross-matching
  this function creating child id `0x1B5` - `funcs_43.c:14352`, vaddr
  `0x801419E8` - against the capture log's `create #240 parent=<0x1AE's addr>
  id=0x1B5`). It looks up a small **per-scene table**
  (`0x8019...` base offset by `sys+0x28D0`\*16, i.e. keyed by the current
  map/scene index) and only increments `0x8019D198`
  (`funcs_43.c:14495-14443`, vaddr `0x80141AEC`-`0x80141AF8`) when a record
  read from that table has **both** its `+0x0` and `+0x4` fields zero;
  otherwise it takes an alternate branch that finalizes via `func_80001CE8`
  *without* incrementing `0x8019D198` this call (implying the increment is
  meant to happen on a **later** pass once the per-scene record has been
  consumed - that later pass was not located this round).

**Working hypothesis:** since this producer is explicitly keyed off the
current scene/map index, a loaded save's arbitrary resume scene is far more
likely to hit the "record has data, defer" branch than a fresh new-game's
fixed, well-exercised starting scene - and if whatever "later pass" is
supposed to complete the deferred increment never runs on the load path
(plausibly for the same reason as `0x083`'s missing creator - some one-shot
title-map-only trigger that a load transition skips entirely), `0x8019D198`
would stay zero forever, independent of pair 126's own state.

### 4.4 Answer to "why does exec stick at 0x20000000"

`bgState_activate`'s AND-gate (`0x8019D194 && 0x8019D198`) requires **two
independent producer chains** to both fire. This round identified the gate
and both producer call sites precisely, but did **not** dynamically confirm
which producer (or both) fails to fire on the load path - that requires a
live capture (reading `0x8019D194`/`0x8019D198`/the two producers' guard
inputs across a fresh vs. loaded transition), which is out of scope for this
reading-only round. The Issue #23 evidence (destroy fires, flags still stuck)
proves at least one of these two producers is broken **independent of**
pair 126's own success/failure - i.e. fixing §3's branch alone will not fully
retire the shims.

## 5. Fix proposals, ranked

### 5.1 Rank 1 - Root-fix the pair-126 stage-table branch (§3.2), keep the exec-flags shim narrowed to only the still-unexplained gate

Replace `LOD_FIX_PAIR126_INPUT_RELEASE`'s `state_init` hook
(`pair126.init-no-fade`, the Issue #26 half) with the already-built
`LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN` logic, promoted from
diagnostic-only to default-on, **after** validating it end-to-end (no capture
log currently proves it works - it was left default-off and unvalidated).
This makes the game's own comparison at `funcs_218.c:4567`/`4577` succeed via
the game's own downstream logic (build fade child, run fade_in/wait/fade_out,
reach destroy normally) instead of skipping straight to a forced flag write
after the fact.

Keep the `state_destroy` hook (`pair126.destroy`,the Issue #23 half)
unchanged for now, since §4.4 shows it addresses a **separate**, still
partially-unexplained gate (`bgState_activate`'s producers). Do not remove
it until a follow-up round confirms both `0x8019D194` and `0x8019D198` are
naturally nonzero by the time pair 126 reaches destroy.

**Risk:** Low-medium. `sys+0x28D2` (`entry_index`'s comparison target) is
mutated by the fix, same as the disabled shim already does - the risk is
identical to the existing disabled code's risk, this proposal just asks to
prove it out and flip it on. Main uncertainty: *why* `sys+0x28D2` disagrees
with the table in the first place is still not understood (§3 only explains
the consequence, not the ultimate cause of the mismatch) - writing over it
is a targeted, well-scoped patch of a single field at a single, precisely
identified guard, which is a meaningfully narrower intervention than the
current shim's blanket exec-flags OR, but it is *not* a full root-cause fix
of "why does the recomp compute the wrong `sys+0x28D2`/`entry_index` for a
loaded save." That deeper question (what determines `obj+0x70`'s "entry
source" descriptor, and what should set `sys+0x28D2` correctly on a load
path) needs a follow-up round with `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`
turned on and logging, played through several distinct save/room
combinations, to confirm the realignment target is *correct* and not merely
"whatever value makes the branch pass."

### 5.2 Rank 2 - Trace and root-fix the two `0x8019D198` producers, keyed on the per-scene table (§4.3)

Add (next round, not this one) a capture that dumps, once per pair-126
transition: `0x8019D194`, `0x8019D198`, the `func_80141E80` "still pending"
test inputs/outputs, and the `func_80141968` per-scene table record for the
current `sys+0x28D0`. Compare a fresh new-game transition against a
Henry-load transition. If (as hypothesized in §4.3) the per-scene record is
what differs, the root fix is almost certainly "make the deferred-increment
follow-up pass for `0x8019D198` run on the load path too" - likely another
one-shot title/handoff-only trigger analogous to the missing `0x083`
creator, which raises the possibility that **§2's missing creator and §4.3's
missing follow-up pass are the same missing trigger**, worth checking first
before writing two separate fixes.

**Risk:** Medium - requires new instrumentation and a play-through capture,
i.e. cannot be scoped/sized until that data exists. Likely low risk once the
actual producer is found (same shape as 5.1: a targeted state advance, not a
new host-side write), but unknown until investigated.

### 5.3 Rank 3 (fallback, band-aid, explicitly not preferred) - Keep both existing shims as-is, unmodified

If 5.1/5.2 cannot be completed in a reasonable timeframe, the current
default-on `LOD_FIX_PAIR126_INPUT_RELEASE` (both hooks) plus the follow-up
`PAIR126_POST_HANDOFF_RELEASE_FIX` window is the status quo and is known
user-validated for Issue #23/#26 and (per round 8) fires routinely on Henry
loads too. This is explicitly **not** a root-cause fix - it forces exec flags
open without the game's own state ever legitimately reaching that point -
and per the project's `feedback_no_artificial_progress` memory, should not be
treated as a resting point; it is listed only as the fallback if 5.1/5.2
research stalls. It also does not address the `0x083`/day-controller gap at
all (§2 is a fully separate symptom family - day banners, pause, Edward - not
touched by exec flags), so keeping only 5.3 leaves the primary user-visible
Issue #27/#31 symptoms (missing banner, dead pause, invisible actors) broken
regardless of what exec flags do.

## 6. Validation plan if 5.1 is implemented

Regression risk is specifically that promoting
`LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN` to default-on and narrowing
`LOD_FIX_PAIR126_INPUT_RELEASE`'s init hook changes behavior for the exact
scenarios that shim was built and user-validated for:

1. **Issue #23 replay** (Henry coffin, source map `0x007D4420` → dest
   `0x0082E330`, NI pair 129 focus): confirm `pair126.destroy` still fires
   (this path's failure mode is untouched by 5.1 per §3.4) and the screen is
   not black. Existing default-off `LOD_ENABLE_ISSUE23_TRACE` diagnostics
   (`ni_overlay_loader.cpp:44-56`, `:344-410`) are already scoped to this map;
   reuse them rather than adding new ones.
2. **Issue #26 replay** (Outer Walls elevator, reporter save, pair 126 entry
   `14`): confirm the stage-table lookup now matches naturally (log the
   realign shim's own diagnostic line, already implemented at
   `ni_overlay_loader.cpp:3837-3846`, and confirm it fires **less** often or
   not at all if the underlying `sys+0x28D2` computation is also fixed
   upstream - if it still fires every time, the fix is only masking the same
   symptom one layer earlier, which should be flagged, not treated as done).
3. **Henry fresh-save-then-load repro** (the round-8/9 focus case,
   `~/Projects/recomp/issue31_artifacts/castlevania2.n64.us.bin` and the
   user's own fresh-Henry save referenced in project memory): confirm `0x083`
   /`0x062`/`0x196`/`0x0F0`/`0x0F1` creation logs appear (once §2's follow-up
   round identifies and fixes the actual creator - 5.1 alone does **not**
   fix this per §3.4/§4.4, only the exec-flags half) and that the days
   banner/pause/Edward symptoms are gone.
4. **Fresh new-game control**: confirm no regression - the realign shim's own
   guard (`loaded_0f_pair != 126` / `lod_current_map_overlay_rom() !=
   LOD_ISSUE27_PRE_HANDOFF_MAP_ROM` early-outs) should make it a no-op
   whenever the natural stage match already succeeds, but this must be
   confirmed with a capture, not assumed.

## 7. Quick file/vaddr reference

- `RecompiledFuncs/funcs_212.c:8160` - `ni_ovl_104_func_0F000B60`, object
  `0x00F` dispatch entry (pair 104).
- `RecompiledFuncs/funcs_212.c:6270`, `:8342` - the two id-`0x190` creation
  sites (`ni_ovl_104_func_0F000130`, `ni_ovl_104_func_0F000C8C`).
- `RecompiledFuncs/funcs_1.c:1081` - `object_createAndSetChild` (`0x80002410`).
- `RecompiledFuncs/funcs_7.c:2122`, `:2295` - `object_executeChildObject`
  (`0x80010B84`), `object_activateChildren` (`0x80010C80`).
- `RecompiledFuncs/funcs_218.c:4517-4936` - `ni_ovl_126_func_0F000070`
  (pair-126 state_init), divergent branch at lines 4562-4602 (vaddr
  `0x0F0000B8`-`0x0F0000E8`).
- `RecompiledFuncs/funcs_218.c:5515` - `ni_ovl_126_func_0F00064C`
  (state_destroy hook point).
- `RecompiledFuncs/funcs_45.c:6814` - `bgState_activate` (`0x80146240`),
  the exec-flags AND-gate consumer.
- `RecompiledFuncs/funcs_45.c:5739` - `bgState_init` (`0x80145BD0`), clears
  the `0x8019D180` block.
- `RecompiledFuncs/funcs_47.c:8209` - `ovl47_gateProducer` (`0x8014B370`),
  `0x8019D194` producer, increment at `funcs_47.c:8537`.
- `RecompiledFuncs/funcs_44.c:758` - `func_80141E80`, one `0x8019D198`
  producer, increment at `funcs_44.c:828`.
- `RecompiledFuncs/funcs_43.c:14259` - `func_80141968` (object `0x1AE` init),
  other `0x8019D198` producer, increment at `funcs_43.c:14495`.
- `src/main/ni_overlay_loader.cpp:3685-3951` - `LOD_FIX_PAIR126_INPUT_RELEASE`
  shim (state_init/state_destroy hooks) and the disabled
  `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN` narrower fix.
- `src/main/ignored_func_stubs.cpp:1848-1946` - `PAIR126_POST_HANDOFF_RELEASE_FIX`
  follow-up shim (hooks `ni_system_handler`, `0x8001B9A0`,
  `RecompiledFuncs/funcs_13.c:2921`).
- `src/main/ignored_func_stubs.cpp:6607-7250` - pre-existing
  `LOD_ENABLE_BGSTATE_TRACE` infra covering the `bgState_*`/`0x1AB` family
  (built for an earlier, separate "gs=5 recovery" investigation, reusable
  for the follow-up work in §5.2).
- `docs/RECOVERY_TO_GAMEPLAY.md:355-378` - Issue #23/#26 history that this
  round's §3 mechanism explains (the #26 half) and does not fully explain
  (the #23 half, see §3.4/§4.4).

## 8. Round 12 (2026-07-21 night): the root fix

Builds on the "COMPLETE PROVEN CHAIN" established by rounds 8-11 (project
memory `issue27-portal-crystal.md`): object `0x016`'s cutscene launcher
(`func_80189164`, common code, `RecompiledFuncs/funcs_66.c:11962`) runs every
frame and searches a 93-entry table at `0x80197170` (10 bytes/record,
halfword `+0x2` = cutscene id) for a record matching `sys+0x2BC8`
("requested cutscene"). Cutscene `0x61` (Henry's opening/briefing) is the
record whose spawn list creates the scenario controllers (`0x083`/`0x084`/
`0x062`/`0x196`) that the per-room days-banner, pause manager, and Edward
spawn logic all depend on. Fresh Henry sessions set `sys+0x2BC8=0x61`;
Henry-loaded sessions never do.

### 8.1 Task A: the fresh-path writer and the load-path candidates

**Fresh-path writer, traced this round.** NI pair 104's object `0x00F`
menu-flow state machine (`ni_ovl_104_func_0F0005E8`,
`RecompiledFuncs/funcs_212.c:7154-8159`, dispatched off `obj+0x38`) never
writes `sys+0x2BC8` directly; all four of its sub-states (`obj+0x38` in
`{1,2,3,4}`) call the shared NI-window library routine at local offset
`0x0F00082C` (canonical C definition `static_158_0F00082C`,
`RecompiledFuncs/funcs_243.c:726-800`; byte-identical bytes are also emitted
as unreachable dead code inside `RecompiledFuncs/funcs_212.c:7582-7649`
because pair 104 embeds the same compiled library routine — this is why the
task's `funcs_212.c:7635` line and `funcs_243.c:782` are the same
instruction). That routine takes `(index, immediate_commit_flag)`, reads a
10-byte record from its own pair's data table at `0x0F002680`, and:
- always stores the record's `+0x2` halfword into `sys+0x2BCC` (a "queued
  cutscene" staging field);
- **only when `immediate_commit_flag != 0`** also stores the same value into
  `sys+0x2BC8` directly and sets `sys+0x2BD0 |= 0x10`.

All four of pair 104's own call sites pass `immediate_commit_flag = 0`
(queue-only). The actual **commit** step — copying `sys+0x2BCC` into
`sys+0x2BC8` unconditionally — is common code: `func_8001A8C4`
(`RecompiledFuncs/funcs_13.c:5-270`), a state handler in the same
`GameStateMgr`-adjacent dispatch family as `func_8001AA48`/`func_8001ACB0`
(both immediately follow it in the same file and appear in the round-2 crash
backtrace), confirmed to run once per "enter gameplay" transition. Guard:
none beyond being reached as that state — the copy is unconditional whenever
the function runs, so the real guard is *whether pair 104's menu-flow queued
anything into `sys+0x2BCC` before this handler runs*, which only happens on
the fresh new-game confirm flow (`obj+0x38` sub-states 1-4 are menu-screen
steps that a "Continue" load never visits).

**Load-path candidate, refuted this round.** Round 11 hypothesized that
widening `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`'s map gate to the
Henry-load route would restore controller creation, reasoning that pair
126's early-bail (§3.2) was the load path's analogous break. This round read
NI pair 126's **entire** state machine in full —
`ni_ovl_126_func_0F000000` (entry), `_0F000070` (state_init, all branches
including a previously-undocumented third early-return gate at
`funcs_218.c:4612-4626`, `sys+0x2BD0` bit `0x1`), `_0F000318` (fade_in),
`_0F000490` (wait), `_0F000590` (fade_out), and `_0F00064C` (state_destroy,
confirmed trivial — it only re-invokes `obj+0x10`) — and found **zero**
references to `sys+0x2BC8`/`0x2BCC`/`0x2BD0` anywhere in any of them. Pair
126's matched (non-early-bail) init path calls only `func_8001C71C`
(`RecompiledFuncs/funcs_13.c:5449`, a 0x164-byte `memory_copy` of ROM player
save-init data — unrelated) and the object's own `obj+0x10` dispatch
pointer. **This refutes the round-11 hypothesis**: pair 126's early-bail
feeds the *separate* `bgState_activate` exec-flags gate (§3-4 of this doc,
the Issue #23/#26 mechanism), not the cutscene-request producer. Fixing pair
126's stage-table branch alone would not have made `func_80189164` see a
nonzero `sys+0x2BC8`.

Of the other `sys+0x2BC8` writers listed in the task brief
(`funcs_110/115/13/147/16/171/188/183`), all except `funcs_13.c` are
per-pair NI overlay code (`ni_ovl_000/004/024/047/066/070` respectively) —
each is a different NI pair requesting its *own* specific in-map cutscene,
not a Henry-load-specific handler. `funcs_13.c:25/29` is `func_8001A8C4`
itself (the commit step above, already covered). None of the load-path
candidates surveyed contains a save/load-selector check (`sys+0x7C`) gating
a `sys+0x2BCC`/`0x2BC8` write; the actual "queue the resume cutscene on
load" call site was not pinned to a single instruction this round — doing so
with full certainty would need a live dynamic capture correlating exact
state-machine transitions on a fresh-load run (the same kind of capture
rounds 1-11 relied on throughout), which was out of scope for this
implementation round per the task's build/deploy budget.

### 8.2 Task A: the divergence point

Given the above, the precise, evidenced divergence is: **the fresh path's
`sys+0x2BCC` producer (pair 104's `obj+0x38` menu-flow, reached only through
the title/new-game-confirm sequence) has no load-path equivalent that runs
before common code's `func_8001A8C4` commit step**, and pair 126's own
sequencing is not that equivalent (§8.1). The exact MIPS instruction that
*should* queue the load-path's resume cutscene was not identified this
round; what is proven is that no code anywhere in `RecompiledFuncs/*.c`
writes `sys+0x2BCC` or `sys+0x2BC8` with a value derived from the current
save/load state (`sys+0x7C`) or reachable from a Henry-load-only branch — a
repo-wide grep of every `sys+0x2BC8`/`0x2BCC` writer (task's own list, cross
verified) accounts for all of them, and none is load-path-specific.

### 8.3 Task B: the implemented fix

Rather than force a guess at the missing MIPS branch (which risks a
band-aid indistinguishable from a host-side force-write), the fix corrects
the same *kind* of divergent input the existing dormant stage-realign shim
corrects for `sys+0x28D2` — a single request/queue field the game's own
downstream logic (`func_80189164`'s native table search and spawn) fully
consumes — narrowly scoped and guarded so it can only ever fire in the exact
differential signature rounds 6-11 established for the bug (Henry root
family present, gameplay running, cutscene request never once seen
nonzero).

**New flag:** `LOD_FIX_HENRY_LOAD_HANDOFF` (CMake option, default OFF,
`CMakeLists.txt`, wired next to `LOD_FIX_PAIR126_INPUT_RELEASE`).

**Where:** `src/main/ni_overlay_loader.cpp`, new block before
`ni_index_to_pair` (`lod_fix_henry_load_handoff_tick` and its helper
`lod_henry_handoff_root_family_present`). Called once per VI from
`src/main/main.cpp`'s existing `lod_debug_cheats_vi_callback` (does not
depend on pair 126 being involved in any particular room's transition, since
§8.1 proved pair 126 is not the producer).

**Guard (all must hold before any mutation):**
1. `gamestate == 3` (gameplay running; read via the existing
   `lod_ni_telemetry_gamestate` helper).
2. A grace window (`LOD_HENRY_HANDOFF_GRACE_FRAMES` = 90 frames, ~1.5s) after
   gameplay is first observed running, during which `sys+0x2BC8`/`0x2BCC`
   are sampled every frame; if either is *ever* seen nonzero during the
   window (or after), the native path already made its own request (fresh
   new game, or any legitimate producer) and the fix permanently stands down
   for that gameplay-entry window.
3. After the grace window, only if neither field was ever observed nonzero:
   confirm this is genuinely a Henry session (not Reinhardt/normal, whose
   sessions never reach `func_8001A8C4`'s commit needing this correction) by
   reading the per-id live-instance counter table at `0x801B3D60` (index
   `(id-1)*2`, confirmed against both `object_createAndSetChild`'s increment
   and `func_800020E8`'s decrement in `RecompiledFuncs/funcs_1.c`) for id
   `0x1AB`, the Henry root family object round 6's differential capture
   showed is created on every loaded-Henry session and never on non-Henry
   sessions. If the Henry family hasn't appeared by a give-up deadline
   (`LOD_HENRY_HANDOFF_GIVEUP_FRAMES` = 300 frames), the fix stands down
   without mutating anything.
4. One-shot per gameplay-entry window: re-arms only if `gamestate` leaves 3
   and returns (covers retry/game-over/reload-a-different-save within one
   process run), so it cannot re-fire every frame or fight a later,
   legitimate zero-request idle moment (`sys+0x2BC8` legitimately returns to
   0 between ordinary in-game cutscenes on a healthy session).

**What it does when it fires:** writes `sys+0x2BCC = 0x61`,
`sys+0x2BC8 = 0x61`, and `sys+0x2BD0 |= 0x10` — exactly the three fields
`static_158_0F00082C`'s own `immediate_commit_flag != 0` branch writes for
every other in-game "commit now" cutscene request (§8.1), so the write
shape mirrors a real, already-existing native mechanism rather than
inventing new behavior. From that point on, `func_80189164` (unmodified,
runs natively every frame) finds the `0x61` record in its own table on its
very next invocation and performs 100% of the actual work itself: table
search, scenario-controller creation (`0x083`/`0x084`/`0x062`/`0x196`),
cutscene state machine, and the downstream day-banner/pause/Edward
follow-on. No object is created from host code, no exec flag is forced, and
no state machine is bypassed — only the one request field pair a legitimate
producer would have written is corrected.

**Log tag:** `[HENRY_LOAD_HANDOFF]`, one line, only on the actual mutation
(by construction this fires at most once per gameplay-entry window):
```
[HENRY_LOAD_HANDOFF] requested cutscene 0x61 after <N> gameplay frames with
no native request seen (map=0x%08X map#%d gs=%d)
```

**Not acceptable shapes avoided:** no direct creation of objects
`0x083`/`0x062`/etc., no exec-flag force, no state-machine bypass — the fix
touches only the same two/three fields the game's own library routine
touches for the identical "commit a cutscene request now" action elsewhere
in the ROM, gated to fire only in the exact zero-evidence-of-any-request
window a genuinely broken load produces.

### 8.4 Files changed (Round 12)

- `CMakeLists.txt` — new `LOD_FIX_HENRY_LOAD_HANDOFF` option (default OFF).
- `src/main/ni_overlay_loader.cpp` — flag default definition (§ near other
  `#ifndef`/`#define` fallbacks), and the fix implementation block before
  `ni_index_to_pair`.
- `src/main/main.cpp` — flag default definition, extern declaration, and the
  per-VI call from `lod_debug_cheats_vi_callback`.

### 8.5 Build/deploy (Round 12)

- `build-ni0e/CMakeFiles/Makefile2` backed up before reconfiguring (per the
  recovery procedure); `cmake -D LOD_FIX_HENRY_LOAD_HANDOFF=ON build-ni0e`
  (cache-only) did **not** actually clobber the hand-patched spirv-cross
  redirect this time — `SPIRV-Cross_BINARY_DIR` is cached as a `STATIC`
  entry pointing at `.../build/spirv-cross`, and a cache-only reconfigure
  (as opposed to a from-scratch one) preserves `STATIC` cache entries, so
  the regenerated `Makefile2` still referenced the shared `build/spirv-cross`
  directory (confirmed: 0 occurrences of `build-ni0e/spirv-cross` before and
  after, and the subsequent `build-ni0e` build finished in ~18s without
  rebuilding spirv-cross). No repair action was needed, but the backup was
  taken first as instructed and the outcome was verified rather than
  assumed.
- `cmake --build build-ni0e --target LodRecomp -j8` (flag ON, plus the
  pre-existing `LOD_ENABLE_NI0E_TRACE`/`LOD_FIX_PAIR126_INPUT_RELEASE`/
  `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN` already ON in that tree): clean
  build, only the pre-existing unrelated `sse2neon.h` `-W#warnings` warning;
  touched `main.cpp`/`ni_overlay_loader.cpp` and rebuilt to confirm no
  warnings from either changed file.
- `cmake build` (bare, no `-D`) run afterward to restore the default tree's
  own `Makefile2`/shared spirv-cross references, per the recovery procedure.
- `cmake --build build --target LodRecomp -j8` (flag OFF, confirmed
  `LOD_FIX_HENRY_LOAD_HANDOFF:BOOL=OFF` in `build/CMakeCache.txt`): clean
  build, only the same pre-existing `sse2neon.h` warning.
- `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums verified
  identical afterward (`3b0f81f71dda5e0a1bb2cf150985de5b`).
- The game was not launched (per instructions). No commit was made (per
  instructions).

## 9. Round 14 (2026-07-22): the days-banner text-measure stack overflow

Separate defect from §1-8 (which cover the scenario-controller/exec-flags
gap): this round investigates the crash where a Henry map transition leaves
the days-banner text struct global (RDRAM `0x8019EECC`) pointing at stale
data, and the self-recursive text-measure walk (`func_8016B878`,
`RecompiledFuncs/funcs_57.c`) recurses over garbage until stack overflow
(native SIGBUS; see `docs/issue27-31-ni0e-findings.md` rounds 2/3/7/8/14 for
the full evidence chain).

**Loader found**: `func_80011754` (`RecompiledFuncs/funcs_7.c:4256`), the
"ensure NI/data file loaded" helper. Key structural fact: it writes
`file_ptr_array[fileid]` **as soon as the buffer is allocated**, before the
DMA/decompress read that fills the buffer's contents completes (a separate,
later step in the same function). A nonzero slot therefore is not proof the
file's bytes are ready.

**Builder re-confirmed**: `func_80145FD4` (`RecompiledFuncs/funcs_45.c:6238`)
resolves `struct_ptr = file_ptr_array[fileid] + offset` from a per-map record
table (`0x80192160 + map_id*16`) and stores it unconditionally to
`0x8019EECC` whenever the record's flag word is nonzero. **Correction to the
previously given root-cause text**: static reading of this function found no
check on `file_ptr_array[fileid]` being nonzero before the store (the only
early-out is the record's own flag being zero, which is a different
condition than the file slot being empty) — see
`docs/issue27-31-ni0e-findings.md` Round 14 §2 for the full comparison and
the alternative explanation (a stale `sys+0x28D0` read on early
post-transition re-entries into this bgState state) that better fits the
observed "plausible-looking garbage" symptom.

**Task A resolution**: inconclusive from static reading alone (matches this
document's own precedent of flagging open items rather than guessing); two
probes shipped instead of a guessed fix — `fileptr-write`
(`RecompiledFuncs/funcs_7.c`) and `textreg-resolve`
(`RecompiledFuncs/funcs_45.c`), both under the existing `LOD_ENABLE_NI0E_TRACE`
flag. One capture across a Henry map transition, filtered to fileids
`0x3B`/`0x3D`, resolves which of the candidate divergence shapes is real.

**Task B implemented (always-on requirement, independent of Task A)**: new
CMake option `LOD_FIX_TEXT_MEASURE_GUARD` (default OFF). Two additive,
data-valid-no-op guards in `RecompiledFuncs/funcs_57.c`:
1. Recursion-depth cap (static counter, abort past depth 128, one-shot
   `[TEXT_GUARD]` log) on `func_8016B878`'s two self-recursive call sites.
2. Pointer-range check on `func_8016890C`'s entry, extending the existing
   null-only check to reject nonzero-but-out-of-range `obj` values the same
   way the existing null check is already handled (same early-return target,
   one-shot `[TEXT_GUARD]` log).

Full detail, code locations, and build/deploy log:
`docs/issue27-31-ni0e-findings.md`, Round 14.

## 10. Round 21 (2026-07-22): Async completion loss — lowest-level analysis

Deep-dive round per explicit user direction: as few patches as possible,
understand the recomp's own async plumbing (host code, not game code) at the
lowest level. No game-side probes/guards added this round (per instructions);
all reading and all instrumentation in this section is host-level
(`lib/N64ModernRuntime/librecomp`, `lib/N64ModernRuntime/ultramodern`).
Builds on rounds 18/19's exhaustive game-side mapping of the async ring
(`docs/issue27-31-ni0e-findings.md`), which is the game-side half of the same
pipeline this round completes on the host side.

### 10.1 The full host-side serve/completion mechanism

The game-side chain (round 18) is: `func_800116BC` (ring enqueue,
`funcs_7.c:4260`) → `DMAMgr_updatePendingFileLoad` (`funcs_7.c:5596`, vaddr
`0x80011E48`) → `DMA_ROMCopy` (`funcs_12.c:5627`, vaddr `0x8001A42C`) →
`DMA_readWrite` (`funcs_12.c:5497`, vaddr `0x8001A374`). Reading
`DMA_readWrite`'s own compiled body in full this round (not just naming its
two callees, as prior rounds did) finds it uses a **single, fixed, global**
OSIoMesg/OSMesgQueue pair, hardcoded as MIPS immediates in every call, not
threaded through any argument or the per-session ring descriptor:

```
a1 = 0x800F13E0                    // OSIoMesg  (funcs_12.c:5555-5580, vaddr 0x8001A3CC-0x8001A400)
mesg->hdr.retQueue (+0x4) = 0x800C5D18   // OSMesgQueue — CONSTANT every call
mesg->devAddr (+0x8) = romAddr
mesg->size    (+0xC) = size
mesg->dramAddr(+0x10)= dramAddr
osEPiStartDma_recomp(handle, mb=0x800F13E0, direction)   // funcs_12.c:5578
osRecvMesg_recomp(mq=0x800C5D18, msg=NULL, flags=OS_MESG_BLOCK)  // funcs_12.c:5588
```

This `0x800C5D18` global is the classic libultra "one static synchronous DMA
channel" pattern (`dmaMessageQ`/`dmaIOMessageBuf`) — it is **not** the
per-session ring descriptor round 18/19 found at `0x800C1600`→`+0x34` (that
struct holds the *software ring*'s 16 queued requests; `0x800C5D18` is the
*hardware-DMA-completion* queue for whichever single request is currently
being physically copied). Every synchronous DMA anywhere in the whole ROM —
not just the async ring — funnels through this one fixed queue, one transfer
at a time.

Host-side (`lib/N64ModernRuntime`), the actual serve path:

- `osEPiStartDma_recomp` (`librecomp/src/pi.cpp:381-399`) reads the handle,
  `OSIoMesg`, and direction, then calls `do_dma()` **synchronously, inline,
  on the calling (game) thread** — there is no separate PI-DMA thread or
  interrupt in this emulation; `osCreatePiManager_recomp`
  (`librecomp/src/pi.cpp:61-63`) is a literal no-op, confirming no PI-manager
  thread exists to race with.
- `do_dma()` (`librecomp/src/pi.cpp:311-362`, read in full): for a ROM read,
  calls `recomp::do_rom_read()` (`librecomp/src/pi.cpp:65-76`, a plain
  `memcpy`-style byte loop from the loaded ROM image into RDRAM — instant,
  no yield) and then
  `ultramodern::enqueue_external_message_src(mq, 0, false,
  EventMessageSource::Pi)` (`pi.cpp:327`, this round's numbering after the
  new trace code — logically the same line round 18 already cited). The copy
  and the enqueue both happen before `do_dma()`/`osEPiStartDma_recomp`
  return — the "DMA" is 100% complete, data and all, by the time
  `DMA_readWrite`'s next instruction (`osRecvMesg_recomp`) runs.
- `enqueue_external_message_src` (`ultramodern/src/mesgqueue.cpp:55-64`)
  does **not** write into the OSMesgQueue struct at all. It pushes a
  `{mq, msg, jam, requeue_if_blocked}` record onto a separate, lock-free,
  multi-producer host-side staging queue (`external_messages`, a
  `moodycamel::BlockingConcurrentQueue<QueuedMessage>`,
  `mesgqueue.cpp:9-16,41`). `requeue_if_blocked` is looked up right here from
  a global `requeue_enabled` bitset, indexed by `EventMessageSource::Pi`
  (`mesgqueue.cpp:63`) — this is the value `ultramodern::set_message_queue_control`
  (`mesgqueue.cpp:19-28`) populated at startup from the game's own
  `Configuration::message_queue_control` (see §10.2).
- The staging queue is only drained — i.e. actually written into a real
  OSMesgQueue's ring buffer, and any blocked receiver woken — by
  `dequeue_external_messages()` (`mesgqueue.cpp:78-89, `92-107` after this
  round's instrumentation`) or `wait_for_external_message[_timed]`
  (`mesgqueue.cpp:91-97`/`99-105`, similarly renumbered), which call
  `do_send(..., block=false)` (`mesgqueue.cpp:130-147`→`156-215` after
  instrumentation) for each staged message. `osSendMesg`/`osJamMesg`/
  `osRecvMesg` (`mesgqueue.cpp:181-240`→`262-321`) each call
  `dequeue_external_messages()` **first**, before doing their own
  send/receive, but **only when called by the game thread**
  (`assert(ultramodern::is_game_thread())` in `osRecvMesg`).

Putting this together for the specific `DMA_readWrite` call sequence: the
same game thread that just posted the PI completion into the staging queue
(via `do_dma`, inside `osEPiStartDma_recomp`) immediately calls
`osRecvMesg_recomp` → `osRecvMesg` → `dequeue_external_messages()` (drains
the staging queue, including the message just posted, into `0x800C5D18`'s
real `validCount`/ring buffer via `do_send`) → `do_recv(block=true)`
(`mesgqueue.cpp:217-247`, finds `MQ_IS_EMPTY` false since `do_send` just
succeeded, returns immediately). **No yield, no thread switch, and no wait
occurs in the success case** — this whole round-trip is fully synchronous
from the game thread's point of view, which matches round 18's own
"asynchronicity is the game's own multi-frame chunking, not a real async
completion" finding, now confirmed at the OS-message-queue level too.

### 10.2 The lost-completion mechanism found

`ultramodern::MessageQueueControl` (`ultramodern/include/ultramodern/ultramodern.hpp:65-74`)
has one field per `EventMessageSource`, each controlling whether a message
that fails `do_send()` on its first delivery attempt gets silently discarded
or pushed back onto the staging queue for another attempt later
(`requeue_if_blocked`, threaded through from §10.1). The **upstream
defaults** are asymmetric:

```cpp
bool requeue_timer = true;
bool requeue_sp    = true;
bool requeue_si    = true;
bool requeue_ai    = false;   // documented rationale: periodic, OK to miss one
bool requeue_vi    = false;   // documented rationale: periodic, OK to miss one
bool requeue_pi    = false;   // <-- no such rationale exists for PI
bool requeue_dp    = true;
```

LoD's own `src/main/main.cpp` (line ~4131, `recomp::Configuration` literal)
**already explicitly overrides four of these seven fields** —
`.requeue_timer = false, .requeue_sp = true, .requeue_si = true, .requeue_dp
= true` — but never mentions `requeue_ai`, `requeue_vi`, or `requeue_pi`, so
all three silently fall through to the upstream struct's default-member
initializers, i.e. `requeue_pi` was `false` in every LoD build to date. This
override was written to make SP (RSP/audio task completion) and SI
(controller input) reliable, which is exactly the same "keep the one-shot
notification a blocking thread is waiting on" property the PI/DMA-ring path
needs — `requeue_pi` reads as an omission, not a considered choice, since PI
completions have the identical one-shot-and-irreplaceable shape as SP's
(unlike AI/VI, whose own code comments explicitly justify dropping messages
because "another one is coming soon" — `ultramodern/src/events.cpp:252-253,
259-260` — there is no "another one is coming soon" for a specific ROM
read's completion; nothing else will ever repost it).

**The mechanism, precisely:** `do_send()`'s only two failure modes
(`mesgqueue.cpp:130-136` pre-instrumentation) are (a) the queue's own sanity
check (`validCount<0 || validCount>msgCount || msgCount<=0 || msgCount>1000`
— a guard explicitly written for "uninitialized queues have garbage values")
and (b) `MQ_IS_FULL`. **If either ever fires for `0x800C5D18` at the exact
moment `dequeue_external_messages()` tries to deliver a just-posted PI
completion, and `requeue_pi` is `false`, that completion is gone forever** —
nothing re-posts a PI message, unlike SP (RSP re-runs its task loop every
frame) or SI (polled every frame) or Timer/DP. The game thread's own
`do_recv(block=true)` then finds `MQ_IS_EMPTY` true and enters
`do_recv`'s `while` loop (`mesgqueue.cpp:224-231` pre-instrumentation),
parking itself on `run_next_thread_and_wait` — a real wait, since nothing
will ever again call `do_send` for this specific message. This is the exact,
mechanically precise shape of "the game then waits forever by design" from
the task brief.

**Why this is asymmetric-but-not-obviously-racy under the observed
single-active-game-thread model.** Reading `ultramodern/src/threads.cpp` and
`scheduling.cpp` in full this round: `osCreateThread` spawns one **real
native OS thread per emulated N64 thread** (`threads.cpp:264-266`), but only
one is ever actually executing MIPS code at a time — all others are parked
on a semaphore (`UltraThreadContext::running`, `wait_for_resumed`/
`resume_thread`, `threads.cpp:153-187`), and control only ever transfers at
specific, enumerable yield points (`check_running_queue`, called from
`osSendMesg`/`osJamMesg`/`osRecvMesg`/`osStartThread`/`osSetThreadPri` —
`scheduling.cpp:18-30`). Since `osEPiStartDma_recomp` → `do_dma()` never
calls any of those, and the very next instruction in `DMA_readWrite` is
`osRecvMesg_recomp` (which drains and consumes its own just-posted message
before ever reaching a yield point), **no other emulated game thread can
interleave between posting and consuming a single `DMA_readWrite` call's own
completion** — this specific sequence is not racy against *itself*. The two
structurally real windows for `do_send` to fail on first attempt are
therefore external to this one call:
1. **A stale/unclaimed message already sitting in `0x800C5D18`'s
   `validCount`** from some *other* `DMA_readWrite` call whose own
   `osRecvMesg` was, for whatever reason, never reached to drain it (e.g. a
   caller that starts a DMA and then takes an early-return/error path before
   its own receive — this shared global is used by every synchronous DMA in
   the whole ROM, not only the async ring, so any such caller anywhere
   could poison it for the *next* caller, including the ring).
2. **Memory corruption of the `0x800C5D18`/`0x800F13E0` structs themselves.**
   Both are fixed low-RDRAM globals in the same `0x800C....`/`0x800F....`
   address family as the async ring's own descriptor (`0x800C1600`) and the
   generic per-object event-dispatch cursor pair (`0x800C15D0`, §10.3).
   Rounds 14/18/19/20 of this same investigation already found **multiple,
   independently confirmed** Henry-load-specific memory-corruption bugs
   (stale/uninitialized-heap pointers surviving map transitions — round 19's
   `0x11111111`-filled descriptor swap; stack-overflow-adjacent stale
   pointer walks — rounds 14/20's `func_8016890C`/`func_8016F310` family).
   If *any* of these already-documented corruption sources ever touches
   `0x800C5D18`'s `validCount`/`msgCount` fields (even transiently), `do_send`'s
   sanity check fails for that one call, and — because of the asymmetry
   above — that single transient hit becomes a **permanent** hang instead of
   a self-healing glitch. This is consistent with, and does not require any
   mechanism beyond, evidence this investigation already has; it was not
   newly proven this round (see §10.4 for what would prove it).

This asymmetry is the "genuine emulation defect in the recomp's own async
plumbing" the task asked to find: it does not, by itself, explain *why*
`do_send` first fails (candidate windows above), but it **does** explain why
a failure — however it originates — becomes an unrecoverable, silent,
permanent loss specifically for PI/DMA completions and not for the other
one-shot sources (SP/SI/Timer/DP), which is exactly the intermittent,
save-load-correlated, "enqueued but never completes" signature the task
describes. Because the fix (§10.5) makes PI messages behave the same
self-healing way SP/SI/Timer/DP already do, it closes the gap regardless of
which of the two candidate windows above (or another not yet found) is the
true first-failure trigger — this is why it is the right minimal host-level
fix even without a live capture pinning the exact trigger.

### 10.3 What `0x8000DC68` is

Read in full (`RecompiledFuncs/funcs_5.c:1527-1608`). It is **not** an
idle/yield/message-poll loop — it never calls `osRecvMesg`/`osSendMesg`, any
`wait_for_external_message*`, or touches any OSMesgQueue/the async ring at
all. Its actual shape: `func_8000DC68(obj, filterMask, arg)` — if
`filterMask` is nonzero, ANDs it against `obj+0x2` (a per-object flags
halfword) and returns immediately if the flags don't match; otherwise it
calls `func_8000D510(obj+0x5C, *(0x800C15D0), (0x800C15D0)[0], (0x800C15D0)[1])`
and advances a **different** shared global cursor pair at RDRAM `0x800C15D0`
by 4 bytes each (`funcs_5.c:1581-1596`). `0x800C15D0` is read/written by a
whole family of sibling functions in the same file
(`0x8000D9B4`-`0x8000DDBC`, all `%hi(0x800C)`/`+0x15D0` accesses) — the
shape (flags-mask-gated conditional dispatch, advancing cursor pair,
`obj+0x5C` sub-struct) matches a generic **per-object event/message
broadcast helper**, unrelated to the DMA ring's own descriptor
(`0x800C1600`) or completion queue (`0x800C5D18`).

Like every function in the DMA-manager chain (round 18/19), `func_8000DC68`
has **no static `jal` caller anywhere in `RecompiledFuncs/*.c`** — it is
reached only via an indirect (`jalr`) call through some runtime function
pointer. This turns out to explain the observation precisely: `get_function()`
(`lib/N64ModernRuntime/librecomp/src/overlays.cpp:384-386,429-432`) is the
**only** place indirect call targets are resolved, and it records every
resolved address into a fixed 32-entry ring buffer (`trace_ring`,
`TRACE_RING_SIZE=32`) — but ordinary compiled `jal`s (the vast majority of
all calls, including every direct call in this entire investigation's
function chains) never go through `get_function()` at all and never touch
this ring. `trace_ring` is dumped by the native crash handler
(`src/main/main.cpp:3649-3708`) and by a targeted diagnostic in
`src/main/ignored_func_stubs.cpp:4306-4319` — i.e. "the last-lookup ring"
from the task brief is specifically **the last 32 indirect-call targets**,
not a general execution trace, and not evidence of a spin loop by itself. A
function with no direct callers (like every DMA-manager-chain function, and
like `func_8000DC68`) is *only ever visible* through this ring, so if it is
invoked many times in a row for many objects (its own shape — a per-object
flags-gated dispatch helper — is exactly the kind of thing called once per
list entry from an outer per-frame broadcast loop) it will naturally
dominate a 32-entry window, hang or no hang.

**Read this way, "repeated `0x8000DC68`" is a *correction* to the task
brief's own hypothesis, not confirmation of it**: it shows the game's normal
per-frame indirect-dispatch traffic was still actively running at the moment
of the snapshot (the ring is not frozen on a single stale entry, and none of
the entries are DMA-manager-chain functions like `func_80011D10`/
`DMAMgr_updatePendingFileLoad`/`func_800120DC`/`func_80012B20`) — i.e. the
overall game thread is **not** parked inside a blocking OS call at all
during the hang window this snapshot came from. This favors round 18/19's
scenario (c) over (b): "the DMA-manager's driving object stopped being
invoked" (never dispatched again) rather than "the consumer is alive but
stuck mid-transfer inside a blocking receive" — though this remains an
inference from what the trace ring *can and cannot* show, not a proof; a
capture that also shows zero `func_80011D10`/`DMAMgr_updatePendingFileLoad`/
`func_800120DC`/`func_80012B20` entries in `trace_ring` at the moment of a
hang (impossible to add without game-side probes this round, per
instructions) would confirm it directly.

### 10.4 Task 2 (driving object) — still not resolvable from static reading alone

Re-confirmed this round, not newly re-derived: `func_80011D10` (the DMA
ring's per-frame dispatch driver), `func_80011D80` (descriptor allocator),
`func_800120DC`/`func_80012B20` (the two dequeue-consumer candidates from
round 19) all have **zero static `jal` callers anywhere in
`RecompiledFuncs/*.c`**, confirmed again this round by grep
(`RecompiledFuncs/funcs_7.c:396-402`'s own pre-existing comment already
documents this exact finding from round 19). They are reached only through
the runtime function-pointer table `D_800AF560`, whose actual *contents*
(which function lives at which state index, and therefore which object type
owns the driving dispatch) live in a `.data`/`.rodata` region this
checkout's `asm/` dump has no text representation for — this is a hard
ceiling on what static reading can determine, not a gap this round's
analysis technique could close. §10.3's `trace_ring` finding gives one new,
concrete way a *future* capture could settle scenario (b) vs (c) without
adding a new probe (the ring is already-built host infrastructure), which is
offered here as the most promising next step rather than re-attempting the
same static search rounds 18/19 already exhausted.

### 10.5 Fix implemented: `LOD_FIX_DMA_COMPLETION` (default OFF, ON in build-ni0e)

Minimal, one-field change at the exact site LoD already customizes message
queue behavior (`src/main/main.cpp`, the `recomp::Configuration` literal):
add `.requeue_pi = (LOD_FIX_DMA_COMPLETION != 0)` alongside the existing
`requeue_timer`/`requeue_sp`/`requeue_si`/`requeue_dp` overrides, gated by a
new default-`0` macro (`#ifndef LOD_FIX_DMA_COMPLETION #define
LOD_FIX_DMA_COMPLETION 0 #endif`, matching every other flag in this file).
No submodule change was needed for the fix itself — `ultramodern`'s
`requeue_pi` field and requeue-on-failure logic already exist and are
already exercised correctly for SP/SI/Timer/DP; LoD's own config simply
never opted PI into it. When a `do_send()` failure occurs for a PI
completion with the fix on, `dequeue_external_messages()` pushes the message
back onto the `external_messages` staging queue (`mesgqueue.cpp:100-102`)
instead of discarding it, and the next `dequeue_external_messages()` call
(the very next `osSendMesg`/`osJamMesg`/`osRecvMesg` from any game thread —
typically within the same frame) retries delivery. This cannot make a
healthy system worse (a message that would have delivered immediately still
does; only a message that would previously have vanished now gets retried),
and it makes PI behave exactly like the three other one-shot sources
(SP/SI/DP) LoD's own config already trusts for this exact property.

**What this fix does *not* claim to do:** it does not identify or repair
whichever memory-corruption source (§10.2, candidate 2) or stale-message
source (§10.2, candidate 1) causes the *first* `do_send` failure — it makes
that failure survivable instead of fatal, which is the correct minimal
intervention when the trigger is one of several already-documented,
still-being-hunted corruption bugs rather than a single fixable instruction.
If a future round's capture (via the new `[PIDMA]` logging below) shows
`send-fail`/`dropped` never fire even on the unfixed build during a
reproduced hang, that would disprove this mechanism and point back to
§10.4's "never dispatched again" scenario instead — the fix is safe to leave
on in that case (it simply never triggers) while that investigation
continues.

### 10.6 Logging added: `LOD_ENABLE_PIDMA_TRACE` (default OFF, ON in build-ni0e)

Since the exact first-failure trigger (§10.2) is not proven, only
plausible from existing evidence, host-level `[PIDMA]`-tagged logging was
added at the four points the task requested, all rate-limited (first 40 +
every 500th, independent counters per site so one noisy site cannot starve
another) and all gated by a new flag using the same local-fallback pattern
already established in this submodule copy for
`LOD_ENABLE_DIAG_SKIP_AUDIO_TASKS` (`ultramodern/src/events.cpp:21-23`):

- **`start`**/**`posted`** (`librecomp/src/pi.cpp`, in `do_dma()`'s ROM-read
  branch): DMA start (rom phys/dram/size/mq) and the completion message post,
  inherently PI-specific since this branch only handles PI ROM reads.
- **`send-fail`** (`ultramodern/src/mesgqueue.cpp`, in `do_send()`, both
  failure branches): fires only when `block==false` (i.e. only for the
  non-blocking external-message-drain callers — `dequeue_external_messages`/
  `wait_for_external_message*` — never for an ordinary game-issued
  non-blocking `osSendMesg`), logging `mq`/`validCount`/`msgCount`/reason.
  This is new visibility the code had **none** of before this round — a
  `do_send` failure was previously completely silent.
  Not filtered to a specific `mq`, since the generic `QueuedMessage` record
  no longer carries the originating `EventMessageSource` past enqueue; this
  is expected to be low-noise regardless since Timer/SP/SI/DP already
  requeue successfully in practice and AI/VI failures, while filtered out of
  nothing, are the intentionally-tolerated "miss one, another comes soon"
  case documented in `events.cpp`.
- **`dropped`** (`ultramodern/src/mesgqueue.cpp`, in
  `dequeue_external_messages`/`wait_for_external_message[_timed]`, right
  after each `do_send` call): fires only when a message both failed
  `do_send` **and** will not be requeued — i.e. the exact, previously
  invisible moment of permanent one-shot message loss this whole
  investigation is chasing.
- **`blocked`** (`ultramodern/src/mesgqueue.cpp`, in `do_recv()`, at entry to
  the blocking wait loop): fires when a game thread's blocking receive finds
  the queue still empty *after* `dequeue_external_messages()` already ran —
  i.e. the thread is genuinely about to park. Logs `mq` and the calling
  thread id.

A capture with this flag on, filtered to `grep '\[PIDMA\]'`, is decisive: if
a hang reproduces and shows a `blocked` line for `mq=0x800C5D18` with no
matching later `send-fail`/`dropped` for the same queue, the blocking
mechanism itself is not where the loss happens (points back to §10.4's
"never dispatched again" instead); if it shows `send-fail`/`dropped` for
`0x800C5D18` shortly before the hang's last `start`/`posted` pair, that
directly confirms §10.2's mechanism and narrows which of the two candidate
windows (stale unclaimed message vs. memory corruption) produced it, based
on the logged `reason`/`validCount`/`msgCount` values (`reason=full` points
at candidate 1; `reason=insane-queue` with garbage-looking `validCount`/
`msgCount` values points at candidate 2).

### 10.7 Files changed (Round 21)

- `src/main/main.cpp` — `LOD_FIX_DMA_COMPLETION` fallback define (near the
  existing `LOD_FIX_HENRY_LOAD_HANDOFF` one) and `.requeue_pi = ...` added to
  the `Configuration::message_queue_control` literal.
- `CMakeLists.txt` — new `LOD_FIX_DMA_COMPLETION` option (target
  `LodRecomp`, default OFF) next to `LOD_FIX_HENRY_LOAD_HANDOFF`; new
  `LOD_ENABLE_PIDMA_TRACE` option (targets `librecomp`+`ultramodern`,
  default OFF) next to `LOD_ENABLE_DIAG_SKIP_AUDIO_TASKS`.
- `lib/N64ModernRuntime/librecomp/src/pi.cpp` — `LOD_ENABLE_PIDMA_TRACE`
  local-fallback define; `start`/`posted` trace hooks in `do_dma()`'s
  ROM-read branch.
- `lib/N64ModernRuntime/ultramodern/src/mesgqueue.cpp` —
  `LOD_ENABLE_PIDMA_TRACE` local-fallback define; `send-fail` hooks in
  `do_send()`; `dropped` hooks in `dequeue_external_messages`/
  `wait_for_external_message[_timed]` (via a shared
  `lod_pidma_note_drop_if_permanent` helper); `blocked` hook in `do_recv()`.
  This is the submodule's second local modification, alongside the
  pre-existing `LOD_ENABLE_DIAG_SKIP_AUDIO_TASKS` block in
  `ultramodern/src/events.cpp` (`git -C lib/N64ModernRuntime diff` read in
  full before touching anything, per instructions — that pre-existing diff
  is unrelated diagnostic scaffolding for audio RSP tasks and was left
  untouched).
- `docs/issue27-31-fix-design.md` — this section.

### 10.8 Build/deploy (Round 21)

- `build-ni0e/CMakeFiles/Makefile2` backed up before reconfiguring, per the
  documented recovery procedure; `cmake -D LOD_FIX_DMA_COMPLETION=ON -D
  LOD_ENABLE_PIDMA_TRACE=ON build-ni0e` (cache-only reconfigure) produced a
  **byte-identical** `Makefile2` (empty diff) — no spirv-cross hazard this
  time. All five pre-existing cache flags
  (`LOD_ENABLE_NI0E_TRACE`, `LOD_FIX_PAIR126_INPUT_RELEASE`,
  `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`, `LOD_FIX_HENRY_LOAD_HANDOFF`,
  `LOD_FIX_TEXT_MEASURE_GUARD`) confirmed unchanged; the two new flags
  confirmed `ON`.
- `cmake --build build-ni0e --target LodRecomp -j8`: succeeded. A targeted
  rebuild (touching only the three changed files) confirmed all three
  compile with **zero warnings/errors** of their own — the only warning
  anywhere in the full build is the pre-existing unrelated `sse2neon.h`
  `-W#warnings`.
- `build/CMakeFiles/Makefile2` backed up; bare `cmake build` (no `-D`, flags
  OFF) also produced a byte-identical `Makefile2`; both new flags confirmed
  `BOOL=OFF` in `build/CMakeCache.txt`.
- `cmake --build build --target LodRecomp -j8` (default tree, both new flags
  compiled out): succeeded, clean except the same pre-existing `sse2neon.h`
  warning — confirms the new code is fully inert (no dead-code warnings, no
  behavior change) when both flags are off.
- `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums of both
  binaries verified identical afterward (`03d63d66285c3a617e6bccff6d75d578`),
  so `./build/LodRecomp` now runs the Round-21 build (fix + trace both ON).
- The game was not launched in this pass (per instructions). No commit was
  made (per instructions).

## 11. Round 22 (2026-07-22): GameStateMgr node-pool hypothesis — geometry corrected, "scene-build" claim refuted, probes shipped

Analysis+probe round targeting repro22's internal A/B hypothesis (project
memory `issue27-portal-crystal.md`, "ROUND-21/22 GRAND SYNTHESIS"): that the
scene build after a map load is commanded through a GameStateMgr command
queue whose node pool has only 4 slots at RDRAM `0x801B1D60`, that
loaded-path transitions leak pool nodes (their cleanup never completing, the
exact thing the pair-126 force-release shims skip), and that the pool
eventually exhausts and silently drops the fatal transition's scene-build
command. Per instructions, this round is reading-only plus probes — no
behavior-changing patch.

### 11.1 The real pool geometry (task 1) — the "4 slots" premise is wrong

Read in full: `cmdNodeTable_clear` (`func_80001940`, RecompiledFuncs/funcs_0.c)
and `cmdNodeTable_alloc` (`func_80001968`, same file). Both compute their
table bounds as the literal MIPS immediates `0x801B1D60` (base) and
`0x801B3D60` (end) — an `0x2000`-byte region of 8-byte slots:

```
slot count = (0x801B3D60 - 0x801B1D60) / 8 = 1024
```

**Not 4 slots — 1024.** Each slot is 8 bytes: `+0` is a "next" link (zeroed
by `cmdNodeTable_alloc` on claim, otherwise used to chain sub-list nodes),
`+4` is the payload/occupancy word (a slot is free iff this word is `0`;
`cmdNodeTable_alloc` writes the caller's payload argument here on claim,
which is *also* what marks the slot occupied — an edge case worth noting is
that a caller storing a genuinely-zero payload would look like a free slot,
but no call site was found doing that).

This is independently corroborated by a **pre-existing** comment already in
the codebase (`RecompiledFuncs/funcs_1.c`, the round-5
`lod_ni0e_manager_destroy_probe` writeup, from the earlier and separate
0x0E-window investigation): it documents `0x801B3D60` as the *start of a
different, adjacent table* (a per-id live-instance counter incremented by
object creation and decremented by `func_800020E8`). Two independently
motivated investigations landing on the exact same boundary address is
strong confirmation the 1024-slot geometry above is correct, not an
artifact of this round's own reading.

`0x801B1D6C`/`0x801B1D84` (the addresses project memory flagged as "DMA
completion targets inside the pool") are simply slot 1's and slot 4's
payload words (`base + 1*8 + 4` and `base + 4*8 + 4`) — i.e., early-address
slots that happened to be occupied at the moment those addresses were
observed by a linear first-fit allocator, not evidence of a 4-entry
structure or of the DMA ring aliasing this pool's slots.

### 11.2 Allocation policy, "full," and silent-failure semantics (task 1)

`cmdNodeTable_alloc(payload)` does a linear first-fit scan of all 1024 slots
(free test: word at `slot+4 == 0`). On success it writes `payload` into
`slot+4`, zeroes `slot+0`, and returns the slot pointer. **"Full" means
literally all 1024 slots are simultaneously occupied** — the scan only stops
early on finding a free slot, otherwise runs the whole table. On exhaustion
it returns `NULL` (`0`) with **no signal of any kind**: no error code
distinct from "slot 0," no flag, no log (before this round), nothing.

Every caller checks the return value and, on `NULL`, **silently skips the
link step** — confirmed in all three push-helper functions that wrap the
allocator (`func_800019B0`, `func_800019E8`, both RecompiledFuncs/funcs_0.c)
and in `GameStateMgr_enqueue`'s own direct call
(RecompiledFuncs/funcs_1.c). There is no retry, no blocking wait, no
fallback allocation, and no error propagated to the ultimate caller anywhere
in the call chain reachable from static reading. **Enqueue failure is
exhaustively silent**, confirming that half of the hypothesis.

The pool is also **not GSM-exclusive**: besides `GameStateMgr_enqueue`
(funcs_1.c), `func_80010EFC` (RecompiledFuncs/funcs_7.c, in the same
object-execution function family as `object_execute`/
`object_activateChildren`) independently calls the same two push-helpers,
making it a second, unrelated consumer of pool capacity. The pool is
general-purpose engine infrastructure, not a GSM-private ring.

### 11.3 Who frees nodes, and node lifetime (task 1)

Nodes are freed **synchronously by the same consumer that processes them**,
not by any deferred/explicit "release" step separate from execution:

- `GameStateMgr_dispatch` (`func_80002E3C`) walks its two-level list (master
  entries at `obj+0x34`, each pointing to a private per-target sub-list) and,
  immediately after dispatching each sub-list node to one of its three
  action handlers (§11.4), writes `0` into that node's `+4` word — the exact
  test `cmdNodeTable_alloc` uses for "free." This happens inline, in the same
  call, for every node it visits; 3 of the pool's 4 total free sites are
  here.
- `func_80001B00` is the 4th free site: called from `GameStateMgr_dispatch`
  when a master entry's own per-target node-count field decrements to zero,
  it finds and frees that master entry's own slot.

So node lifetime is normally alloc → dispatch → free, all within a single
`GameStateMgr_dispatch` call — there is no "handoff" a node is waiting on
that could fail to complete and leave it allocated. **The only way pool
nodes accumulate is if `GameStateMgr_dispatch` itself stops running** for a
stretch while `GameStateMgr_enqueue` keeps being called. Like every function
in round 18/19/21's DMA-manager chain, `GameStateMgr_dispatch` has **zero
static `jal` callers anywhere in `RecompiledFuncs/*.c`** — it is reached only
through a runtime function pointer, so whether it keeps running across a
loaded-Henry transition is not resolvable from static reading alone (the
identical ceiling round 21 §10.4 already documented for the DMA-manager
chain). This is the one part of the leak mechanism this round's static
reading could not settle either way.

### 11.4 Who enqueues the "scene-build" command (task 2) — none found; this queue is a deferred-DESTROY dispatcher

`GameStateMgr_dispatch`'s three possible actions on a dequeued command,
selected by two tag bits in the command word (`0x40000000`/`0x20000000`,
matching project memory), are:

1. `func_800020E8` — frees an object's 16 `alloc_data` slots and clears its
   figure pointers. **Independently confirmed** by the pre-existing round-5
   probe comment cited in §11.1 as "the object-destroy entry point" (it also
   decrements the per-id live-instance counter object creation increments).
2. `sceneDataPool_free` — frees a scene-data-pool entry.
3. `func_80001920` (no tag bits set) — a small default pointer-clear.

**None of these three actions create an object.** Every static caller of
`GameStateMgr_enqueue` found this round —
`object_executeChildren`/`func_80004F44` (a recursive children-destroy
walk)/`object_dispatchChild`'s per-field release block, 14 call sites total
across RecompiledFuncs/funcs_1.c and funcs_2.c — is itself object/resource
**teardown** code, not scene-build code.

**Verdict: no evidence was found, and considerable contrary evidence was
found, that the 0x1AB framework family's creation is routed through this
queue at all.** This queue is a deferred DESTROY/RELEASE dispatcher. The
0x1AB family's own creation is driven by its `bgState` state machine (table
`0x8018D3B0`, per rounds 15/16 in `docs/issue27-31-ni0e-findings.md`) via
the ordinary per-object per-frame dispatch loop — a completely separate
mechanism from `GameStateMgr_enqueue`/`GameStateMgr_dispatch`. The literal
"the fatal transition's scene-build command is silently dropped" claim is
therefore **refuted** by direct reading: this specific queue does not build
scenes, so it cannot be *the* mechanism that drops one.

What silently dropping a command from *this* queue would actually cause is
a lost deferred object/resource release — consistent with, and a plausible
contributor to, the already-documented Henry-load memory-corruption family
(rounds 14/18/19/20: stale/uninitialized-heap pointers surviving map
transitions), but several inferential steps removed from "scene build never
starts," and not proven this round (§11.3's open question — whether
`GameStateMgr_dispatch` itself keeps running — would need to be resolved
first, and even then the pool's 1024-slot size makes gradual exhaustion from
ordinary per-transition teardown traffic look unlikely on its own; it would
take either a very long stretch of `GameStateMgr_dispatch` never running, or
unrelated memory corruption directly overwriting the `0x801B1D60`-`0x801B3D60`
region, matching the two candidate windows round 21 §10.2 already documented
for the structurally identical PI/DMA-completion asymmetry).

### 11.5 Probes shipped (task 3)

All behind the existing `LOD_ENABLE_NI0E_TRACE` flag (no new CMake option
needed), all `[NI0E_TRACE]`-tagged:

- **`gsm-alloc`** (`lod_ni0e_gsm_alloc_probe`, `src/main/ni_overlay_loader.cpp`,
  hooked at both of `cmdNodeTable_alloc`'s return points,
  RecompiledFuncs/funcs_0.c): slot address/index, payload, occupancy after,
  map/gamestate. Always on a `NULL` (exhaustion) return; rate-limited (first
  100 + every 500th) for ordinary successes, per instructions.
- **`gsm-free`** (`lod_ni0e_gsm_free_probe`, same file, hooked at all 4 free
  sites — `GameStateMgr_dispatch`'s 3 inline frees plus `func_80001B00`'s 1,
  both RecompiledFuncs files): slot address/index, the payload it held
  before clearing (read directly from memory immediately before the store,
  not from a register — two of the three `GameStateMgr_dispatch` sites call
  into `func_800020E8`/`sceneDataPool_free` first, which are free to clobber
  the caller-saved register a naive register-based read would have used).
  Same rate limit as `gsm-alloc`.
- **`gsm-enq`** (`lod_ni0e_gsm_enq_probe`, same file, hooked at
  `GameStateMgr_enqueue`'s single exit point, `L_80002E30`): obj/cmd/target
  (snapshotted into locals at function entry, before any callee can clobber
  the argument registers) and a `dropped`/`ok` verdict. `dropped` is derived
  from a sticky flag (`lod_ni0e_gsm_enq_begin`/set by `gsm-alloc` on any
  `NULL` return during the call) rather than a direct return-code check,
  since a logical enqueue's underlying pool alloc can happen either directly
  or via `func_800019B0`/`func_800019E8`, neither of which propagates
  `cmdNodeTable_alloc`'s return value back to the caller. Always for drops,
  rate-limited for successes, per instructions.
- **`gsm-pool`** (`lod_ni0e_gsm_pool_watch_vi_callback`, `src/main/main.cpp`,
  registered in `lod_debug_cheats_vi_callback`): passive per-VI scan of all
  1024 slots, change-only logging of the total occupied count. Cheapest
  possible occupancy signal (a 4KB read once per VI), independent of the
  three call-site probes above — catches occupancy drift even if it happens
  through some path those probes don't cover (e.g. direct memory
  corruption, per §11.4's second candidate window).

A future capture filtered to `grep 'gsm-'` is decisive either way: if
occupancy stays near-idle (single digits) across loaded-Henry transitions
with no `gsm-alloc DROPPED`/`gsm-enq ... DROPPED` lines, that closes out this
hypothesis empirically as well as by static reading; if occupancy climbs
monotonically across transitions and a drop eventually fires, that would
mean `GameStateMgr_dispatch` really is going idle on the loaded path
(§11.3's open question resolved) — still not "scene build dropped" per
§11.4, but a real, related leak worth its own fix.

### 11.6 If a leak is ever confirmed: the correct minimal fix (task 4)

Per instructions, nothing beyond probes was implemented this round, but the
shape of the correct fix *if* a future capture confirms
`GameStateMgr_dispatch` goes idle on loaded-Henry transitions (§11.3/§11.5)
is worth recording now: **do not enlarge the pool.** It is already 1024
slots — 256x the size the hypothesis assumed — so a size-based "fix" would
either do nothing (if the real cause is `GameStateMgr_dispatch` never
running, more slots just delays the same eventual exhaustion) or actively
mask a real bug (if the real cause is memory corruption stomping the region,
a bigger pool is a bigger target, not a smaller one). This is the same
"don't add band-aids that mask bugs" principle project memory already
records (`feedback_no_artificial_progress`).

The correct fix, matching this document's own §5.1 methodology for the
pair-126 stage-table branch, would be to find and fix **why
`GameStateMgr_dispatch`'s driving object stops being dispatched** on the
loaded path — i.e. the same class of fix as §5.1 (root-fix the specific
stage/state mismatch that causes a natural per-frame consumer to stop
running), not a capacity increase, and not another forced flag/state write
of the kind the pair-126 shims already use as a stopgap. Concretely, that
would mean: (a) confirm via `gsm-pool`/`gsm-free` capture that
`GameStateMgr_dispatch` really does stop running (zero `gsm-free` lines
during a stretch where `gsm-alloc` keeps firing); (b) find the driving
object/table-index for `GameStateMgr_dispatch`, the same kind of search
round 21 §10.4 already attempted and hit the same static-reading ceiling on
for the DMA-manager chain (both are reached only via `D_800AF560`-style
runtime function-pointer tables whose contents live in a `.data`/`.rodata`
region this checkout's `asm/` dump has no text representation for); (c) once
found, determine whether its stall has the same root shape as the pair-126
branch (a data-table lookup that disagrees with the loaded-Henry state) and
fix that specific mismatch, rather than papering over the symptom.

### 11.7 Files changed (Round 22)

- `src/main/ni_overlay_loader.cpp` — new `gsm-alloc`/`gsm-free`/`gsm-enq`
  probe functions and the shared pool-geometry constants/occupancy-scan
  helper they and the per-VI watch share, all under the existing
  `LOD_ENABLE_NI0E_TRACE` block (extending the pre-existing
  `lod_ni0e_check_dispatch` section rather than adding a new one).
- `RecompiledFuncs/funcs_0.c` — new `LOD_ENABLE_NI0E_TRACE` block (this file
  had none before) with `extern` declarations for `gsm-alloc`/`gsm-free`;
  `gsm-alloc` hooked at both of `cmdNodeTable_alloc`'s return points;
  `gsm-free` hooked at `func_80001B00`'s one free site.
- `RecompiledFuncs/funcs_1.c` — `extern` declarations for
  `gsm-free`/`gsm-enq` added to the existing `LOD_ENABLE_NI0E_TRACE` block;
  `GameStateMgr_enqueue` instrumented at entry (argument snapshot into
  locals) and its single exit point (`gsm-enq`); `GameStateMgr_dispatch`
  instrumented at its 3 inline free sites (`gsm-free`).
- `src/main/main.cpp` — new `lod_ni0e_gsm_pool_watch_vi_callback` (`gsm-pool`,
  change-only per-VI occupancy scan), registered in
  `lod_debug_cheats_vi_callback` alongside the existing round-18/19 ring-
  pending watch.
- `docs/issue27-31-fix-design.md` — this section.

No CMake changes were needed — all new code reuses the existing
`LOD_ENABLE_NI0E_TRACE` flag, already `ON` in `build-ni0e` and `OFF` in
`build` from round 21.

### 11.8 Build/deploy (Round 22)

- `build-ni0e/CMakeFiles/Makefile2` backed up before reconfiguring, per the
  documented recovery procedure (the first build attempt hit the
  now-familiar shared-Makefile2/spirv-cross hazard, rounds 12-21); bare
  `cmake build-ni0e` (no `-D`, cache-only) produced a **byte-identical**
  `Makefile2` (empty diff) — no spirv-cross hazard needing repair this time.
  All seven pre-existing cache flags (`LOD_ENABLE_NI0E_TRACE`,
  `LOD_FIX_PAIR126_INPUT_RELEASE`, `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`,
  `LOD_FIX_HENRY_LOAD_HANDOFF`, `LOD_FIX_TEXT_MEASURE_GUARD`,
  `LOD_FIX_DMA_COMPLETION`, `LOD_ENABLE_PIDMA_TRACE`) confirmed unchanged.
- `cmake --build build-ni0e --target LodRecomp -j8`: succeeded, clean except
  the same pre-existing unrelated `sse2neon.h` `-W#warnings` warning.
- `build/CMakeFiles/Makefile2` backed up; bare `cmake build` (no `-D`, flags
  OFF) produced a 40-line diff against the backup — inspected in full and
  confirmed to be pure line-order reshuffling of the same dependency edges
  (each changed line's counterpart is present elsewhere in the diff with
  identical content), the same benign pattern round 19 documented for this
  tree; `LOD_ENABLE_NI0E_TRACE`/`LOD_FIX_DMA_COMPLETION`/
  `LOD_ENABLE_PIDMA_TRACE` all confirmed `BOOL=OFF`.
- `cmake --build build --target LodRecomp -j8` (default tree, all trace
  flags compiled out): succeeded, clean except the same `sse2neon.h`
  warning — confirms the new probe code is fully inert when the flag is off.
- `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums of both
  binaries verified identical afterward (`0c9ce94220a3714a280ddf6aae22f2d2`),
  so `./build/LodRecomp` now runs the Round-22 build (all prior fixes/traces
  plus this round's four new probes, all ON).
- The game was not launched in this pass (per instructions). No commit was
  made (per instructions).

## 12. Round 24 (2026-07-22): `ni_system_handler` state machine mapped end to end; the true divergent branch found (corrects the round's own starting hypothesis)

Reading-only round (probes only, no behavior change), per instructions.
Builds on round 21 section 10 (host-side async completion) and round 22
section 11 (GameStateMgr node pool) — this round completes the third leg of
the same triangle: the NI-system object (id `0x009`) whose `ni_system_handler`
state is the thing that actually calls `overlay_system_create`.

### 12.1 Correction to this round's own starting premise

The round brief's "Established" section asserted the fatal loaded-Henry
transition "skips the FRONT step: no `func_80011D80` re-init", citing
`/tmp/ni0e_repro22.log`. Re-checking that exact log this round: `grep -n
"desc-life\|MAP_OVL"` shows a `desc-life #N site=func_80011D80 ...
REINIT(...)` line immediately preceding **every** `MAP_OVL #N` load in the
file, including the fatal one (`desc-life #10` at line 2814, immediately
before `MAP_OVL #6` at line 2846 — both present, in the expected order, same
as the four earlier healthy transitions). **`func_80011D80` is not skipped
on the fatal transition; this part of the round's premise does not hold up
under the log it cites.** This is noted here rather than silently
overridden, per project practice of validating assumptions before removing/
revising them. The real divergence, found by reading (not by re-reading the
log), is downstream of `func_80011D80` entirely — see 12.4.

### 12.2 Function map and the state machine, precisely

All addresses in `RecompiledFuncs/funcs_13.c` unless noted.

| Function | vaddr | Role |
|---|---|---|
| `func_8001B718` | `0x8001B718` | Recursion-guarded per-frame **dispatch wrapper** for the id-`0x009` "NI-system" object, table `0x800B3834` (confirmed by the pre-existing `lod_dispatch_table_for_obj_id` map in `src/main/ignored_func_stubs.cpp:6832-6846`: `case 0x009: return 0x0B3834`). Named `sys_obj009_driver` by a prior (2026-06 "gs=5") investigation's already-built `LOD_ENABLE_BGSTATE_TRACE` wrapper list. |
| `ni_system_handler` | `0x8001B9A0` | One state reached via that dispatch; see 12.3. |
| `overlay_system_create` | `0x8001BA78` | Another state, reached via code adjacency + the same pre-existing wrapper list (its neighbor in that list, `sys_obj00a_driver` at `0x8001BC5C`, is a different object id's driver, confirming `overlay_system_create` is grouped with the id-`0x009` family, not id `0x00A`). Builds the parentless `0x1AB` root. |
| `func_80011D80` | `0x80011D80` (`RecompiledFuncs/funcs_7.c:5615`) | A **different** recursion-guarded dispatch wrapper, table `D_800AF560` (`0x800B0000 - 0xAA0`, round 21 section 10.4's already-named DMA-manager table), on a **different, separate object** — the "DMA-pump" object rounds 18/19/21/22 already investigated, not the id-`0x009` NI-system object. Links to the NI-system side only through the global active-pump pointer `*(0x800C1600)`. |
| `func_80012ED0` | `0x80012ED0` (`RecompiledFuncs/funcs_8.c`) | Generic "resolve or defer a scene-keyed asset" utility. **The sole writer of `sys+0x2B24`** (`0x801CADE4`), the only field `overlay_system_create`'s guard checks. Called from `ni_system_handler` and 3 sibling scene-keyed state handlers (`func_8001B530` etc. — has ordinary static `jal` callers, unlike the DMA-manager-chain family). |
| `object_curLevel_goToNextFuncAndClearTimer` (`func_80001CE8`) | `0x80001CE8` (`RecompiledFuncs/funcs_0.c:4613`) | The per-depth-level state-advance primitive both `func_8001B718`'s callees and `bgState`'s producers use (already named in section 1's table). Read in full this round: **unconditionally** increments the current depth-level's 16-bit (count\|index) word by 1, then unconditionally clears the count byte back to 0. No data-driven branch of its own — whichever state runs next is decided entirely by which function's own address sits at `table[old_index+1]`, a `.data` fact this checkout's `asm/` dump cannot show (same ceiling rounds 21/22 already hit). |

**The generic per-object state-record mechanism** (shared by `func_8001B718`,
`func_80011D80`, pair 126's own `0x0F000000` wrapper, and `bgState`): each
object has a small transient "depth" counter at `obj+0xE` (pushed by the
wrapper before dispatch, popped after the callee returns) and a **persistent**
array of 2-byte `(count, state_index)` records starting at `obj+8`, one
record per depth level (`obj + 8 + depth*2`). The wrapper reads
`table[state_index*4]` and calls it; a callee that wants to "advance" calls
`object_curLevel_goToNextFuncAndClearTimer(obj+8, obj+0xE)`, which
increments `state_index` by exactly 1 for the **current** depth level.

### 12.3 `ni_system_handler`'s body, and why it runs only once

Read in full (`funcs_13.c:2933-3066` post-instrumentation). Every branch
quoted:

```
v1 = sys_base (0x801C82C0)
scene = sys+0x28D0                                  // "scene" field, established
table_entry = 0x800B2DE8 + scene*8                   // per-scene table, 8-byte stride
a2 = table_entry[0]                                  // field0 (word)
obj = a0  (the id-0x009 object itself)

if (a2 == 0):                                        // "idle-bump" branch
    sys+0x2B24++                                     // plain counter bump, arbitrary value
    goto advance
else:                                                 // "scene-active" branch
    t9 = table_entry[4]                              // field1 (word)
    delta = t9 - a2
    func_80012ED0(a0=*(0x800C1600) /* active pump obj */,
                  a1=&(sys+0x2B24), a2=a2, a3=0x802E3B70,
                  [sp+0x10]=delta, [sp+0x14]=0)
    // return value of func_80012ED0 is NEVER checked here
    goto advance

advance:
    object_curLevel_goToNextFuncAndClearTimer(obj+8, obj+0xE)   // ALWAYS runs
    // immediately, in the SAME call (tail-chain, not return-then-reinvoke):
    dispatch table_0x800B3834[ obj's now-advanced state_index ](obj)
```

Both branches converge unconditionally on `advance`, which **always** bumps
the state index and **immediately** re-enters `table_0x800B3834[new index]`
in the same call frame (confirmed: it re-reads the record at the same
`obj + depth*2` address `func_8001B718`'s wrapper just used, using the
*already-incremented* depth value, not a fresh dispatch from the wrapper).
**Consequence: `ni_system_handler` executes for exactly one pass, ever, per
id-`0x009` object lifetime**, then permanently hands control to whatever sits
at the next table slot. Given the adjacency and wrapper-naming evidence in
12.2, that next slot is `overlay_system_create`. Unlike `ni_system_handler`,
`overlay_system_create` does **not** call
`object_curLevel_goToNextFuncAndClearTimer` at all — it re-invokes itself via
the object's own `obj+0x10` "current dispatch pointer" field instead (the
same self-loop idiom section 3.1 already documented for pair 126's
`state_destroy`). So once state reaches `overlay_system_create`'s slot, it is
what runs **every subsequent frame**, gated solely by:

```
overlay_system_create(obj):
    if (sys+0x2B24 == 0): goto tail        // skip -- guard not satisfied
    created = object_createAndSetChild(parent=NULL, id=0x1AB)
    sys+0x2924 = created                    // overlay_system_ptr
    ...init two fixed 0x1A4/0x28-byte structs, object_dispatch2(created)...
    tail:
    obj->(obj+0x10)(obj)                    // self-redispatch, every frame
```

### 12.4 The actual divergent branch: `func_80012ED0`'s 3-way branch (Task 2)

Read in full (`RecompiledFuncs/funcs_8.c`). `sys+0x2B24` — the only thing
`overlay_system_create` ever checks — is written **exclusively** inside this
one function, and only along one of its three paths:

```
func_80012ED0(a0=pump_obj, a1=completion_ptr, a2, a3, delta, flag):
    if (a2 & 7) || (a3 & 7) || (delta & 7):        // alignment sanity check
        return 0                                    // site=align-fail: no side effect

    if (delta == 0):
        threshold = *(0x800AF5A0)
        if (SIGNED(a2) < SIGNED(threshold)):
            goto defer_child
        // site=immediate-dma:
        DMA_ROMCopy(romAddr=a2, dramAddr=a3)         // SYNCHRONOUS, completes same call
                                                      // (round 21's fully-synchronous DMA finding)
        if (completion_ptr != 0):
            *completion_ptr = a3 + a2                // <-- THE ONLY sys+0x2B24 WRITE
        return 0

    defer_child:
    // site=defer-child (delta != 0, OR a2 below threshold):
    child = object_createAndSetChild(parent=pump_obj, id=5)
    child->0x34.ptr0    = completion_ptr             // &sys+0x2B24 stashed for later
    if (completion_ptr != 0): *completion_ptr = 0    // <-- EXPLICITLY CLEARS sys+0x2B24
    child->0x34.field4  = a2
    child->0x34.field8  = a3
    child->0x34.field0C = delta
    child->0x10 (dispatch fn) = *(0x800AF5A0)
    return child
```

**`site=defer-child` is the divergence**: it creates a new **child object
(id `5`) under the currently active DMA-pump object** (`*(0x800C1600)`,
round 18/19/21/22's own subject), stashes the address of `sys+0x2B24` inside
that child's own fields for later, and **actively zeroes `sys+0x2B24`**,
deferring the completion signal entirely to that child's own future
per-frame dispatch (its own `obj+0x10` entry — never itself found this
round; it has no static `jal` caller anywhere in `RecompiledFuncs/*.c`, the
identical "function-pointer-only" ceiling rounds 21 §10.4 and 22 §11.3 already
hit for the DMA-manager chain and `GameStateMgr_dispatch`).

Since `ni_system_handler`'s `a2!=0` branch — the only branch that ever calls
`func_80012ED0` — executes **at most once** per id-`0x009` object lifetime
(12.3), if `site=defer-child` is taken on that one occasion, `sys+0x2B24` has
exactly one remaining path to ever become nonzero: **the id-`5` child object
must itself be dispatched at some later frame.** If the same "driving object
stopped being invoked" failure already suspected for the DMA-manager chain
(round 21 §10.4) and `GameStateMgr_dispatch` (round 22 §11.3) also applies to
this child (a very natural hypothesis, since it is created as a child of the
*exact same* pump object those two investigations already flagged), the
child never runs, `sys+0x2B24` never leaves 0, and `overlay_system_create`
loops forever finding its guard false — precisely the observed hang shape.

**This reframes, and does not contradict, the pre-existing round 18/19/21
finding.** Corroborating evidence already in `/tmp/ni0e_repro22.log`: the
last `[PIDMA] start`/`posted` pair anywhere in the entire log is at line
2339-2340 (`dram=0x80336C88`, right after `MAP_OVL #5` loads) — **zero**
further PI-DMA starts occur for the rest of the session, including through
and past the fatal `MAP_OVL #6` at line 2846, even though `ring-pending`
lines keep incrementing `readidx` afterward (lines 2821-2843) by re-reading
**byte-for-byte identical stale `value_written` data** left over from
descriptor `0x80332E38`'s *first* use (map #4) — i.e. those are not real new
completions, they are stale ring-buffer bytes from the previous owner of a
recycled physical descriptor block being misread as fresh completions. This
independently supports "something downstream of the pump stops actually
issuing/consuming new work after `MAP_OVL #5`," the same shape as the
`func_80012ED0`/id-`5`-child mechanism found here.

### 12.5 Task 3: `func_80011D80`'s own dispatch context

`func_80011D80` (`funcs_7.c:5615-5744`, read in full) does **not** touch the
id-`0x009` object at all — it operates purely on the DMA-pump object. Its own
body: calls `func_800027B0(a0=<its own a0 param>, a1=0, a2=0x988, a3=0)`
(an allocate/find helper); on success, installs its `a0` object as the new
`*(0x800C1600)` (the `desc-life REINIT` trace line), zeroes/initializes
fields inside that object's own descriptor sub-struct (`+0x838`, `+0x844`,
`+0x846`), then **explicitly force-sets** its own per-depth state index to a
fixed value (`1`, via `object_curLevel_goToFunc`/`func_80001E30` — an
absolute set, not a relative "next" advance) before dispatching
`D_800AF560[1]`. Because this reset is absolute and unconditional, whatever
runs after `func_80011D80` is not sensitive to that object's own prior
history — confirming the divergence is not "stale leftover state on the pump
object" but is downstream of it, at the level of "does anything keep
invoking the pump's (or its new child's) own per-frame dispatch after
handoff," matching 12.4.

### 12.6 Probes added (`nisys-state`, `LOD_ENABLE_NI0E_TRACE`, no new CMake option)

- **`site=ni_system_handler`** (`lod_ni0e_nisys_handler_probe`, hooked in
  `ni_system_handler` right after the per-scene table lookup): obj, the
  object's own persistent state word (`obj+8`), `a2`/`field1` (the per-scene
  table row), which branch will run, and `sys+0x2B24`'s value at that moment.
  Rate-limited (first 60 + every 300).
- **`site=overlay_system_create`** (`lod_ni0e_ovlsys_create_probe`, hooked
  right after the guard value is read): obj, `sys+0x2B24`, and whether this
  call will create or skip. Always logged on `CREATE` (rare, at most a
  handful per session); rate-limited otherwise.
- **`site=align-fail` / `site=immediate-dma` / `site=defer-child`**
  (`lod_ni0e_nisys_future_probe`, three call sites inside `func_80012ED0`):
  pump object, completion pointer, `a2`/`a3`/`delta`, and `sys+0x2B24`'s
  value at that moment. **`site=defer-child` is always logged, unconditionally
  (not rate-limited)** — this is Task 2's identified divergent/skip branch,
  per instructions. The other two sites are rate-limited (first 60 + every
  300).

A future capture filtered to `grep nisys-state` is decisive: if the fatal
transition's `ni_system_handler` pass logs `branch=scene-active` followed by
`site=defer-child`, and **no `gsm-free`/`ring-done`/any other per-frame
activity ever again references the resulting child object**, that directly
confirms this round's mechanism; if `ni_system_handler` never even logs for
the fatal map (meaning the id-`0x009` object itself was never freshly
re-armed for that map — an open question this round's static reading could
not settle, symmetric with 12.5's finding that only the *pump* object is
provably reset each transition), that would point one level further
upstream instead.

### 12.7 Recommended minimal root fix

**Do not patch `sys+0x2B24` directly, and do not add a retry/timeout to
`overlay_system_create`'s guard** — both would be another forced-state
band-aid of exactly the kind `feedback_no_artificial_progress` already warns
against, on top of the two `LOD_FIX_PAIR126_INPUT_RELEASE` shims this
investigation has been trying to retire. The minimal, non-band-aid fix, once
12.6's capture confirms the mechanism:

1. Find the id-`5` child object's own driving mechanism (its `obj+0x10`
   dispatch pointer, and whatever per-frame broadcast is supposed to reach
   it) — the same search rounds 21 §10.4 and 22 §11.3 already need for the
   DMA-manager chain and `GameStateMgr_dispatch`. Given all three are
   children/consumers of the *same* active-pump object, this is very likely
   **one fix, not three**: whatever stops the pump's own child-object
   broadcast from running after a loaded-Henry `MAP_OVL` load is probably the
   single root cause behind the DMA ring's own stalled completions (round 21),
   `GameStateMgr_dispatch`'s hypothesized idle state (round 22), and this
   round's stuck `overlay_system_create` gate.
2. If that broadcast turns out to be gated by the same stage-mismatch shape
   already found and root-fixed for pair 126's own state machine (section
   3.2 — a data-table lookup keyed on `sys+0x28D2`/`scene2` that disagrees
   with the loaded-Henry state), fix that specific mismatch at its source
   (the already-built but disabled `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`
   shim, section 3.3, is the closest existing candidate, though it currently
   patches the symptom — the *comparand* — rather than explaining why
   `scene2` disagrees with the table in the first place).
3. Only after (1)/(2): if the pump's child broadcast is confirmed healthy but
   the id-`5` child specifically is still never created/attached correctly on
   the loaded path, look at `func_80012ED0`'s own `delta`/`a2` inputs (the
   per-scene table row at `0x800B2DE8 + scene*8`) for a loaded-path-specific
   data mismatch, the same class of bug as pair 126's.

This keeps the fix scoped to "make the natural per-frame consumer run/agree
with the data it's given," not "force the downstream flag/gate," consistent
with §5.1's methodology and round 22 §11.6's identical recommendation for
`GameStateMgr_dispatch`.

### 12.8 Files changed (Round 24)

- `RecompiledFuncs/funcs_13.c` — new `LOD_ENABLE_NI0E_TRACE` block (this file
  had none before) with `extern` declarations for
  `lod_ni0e_nisys_handler_probe`/`lod_ni0e_ovlsys_create_probe`;
  `site=ni_system_handler` hooked into `ni_system_handler` right after the
  per-scene table lookup; `site=overlay_system_create` hooked into
  `overlay_system_create` right after its guard value is read.
- `RecompiledFuncs/funcs_8.c` — `extern` declaration for
  `lod_ni0e_nisys_future_probe` added to the existing `LOD_ENABLE_NI0E_TRACE`
  block; `func_80012ED0` instrumented at entry (argument snapshot into
  locals, before its own prologue touches `sp`/`a0`-`a3`) and at all three of
  its branch exits (`L_80012F00`/align-fail, `L_80012F60`/immediate-dma,
  `L_80012F68`/defer-child).
- `src/main/ni_overlay_loader.cpp` — new `lod_ni0e_nisys_handler_probe`/
  `lod_ni0e_ovlsys_create_probe`/`lod_ni0e_nisys_future_probe` functions
  (`extern "C"`, matching the existing `gsm-*` probes' linkage convention),
  all under the existing `LOD_ENABLE_NI0E_TRACE` block, no new CMake option.
- `docs/issue27-31-fix-design.md` — this section.
- `docs/issue27-31-ni0e-findings.md` — Round 24 summary section.

No CMake changes were needed — all new code reuses the existing
`LOD_ENABLE_NI0E_TRACE` flag, already `ON` in `build-ni0e` and `OFF` in
`build`.

### 12.9 Build/deploy (Round 24)

- First `cmake --build build-ni0e --target LodRecomp -j8` attempt hit the
  now-familiar shared-`Makefile2`/spirv-cross hazard (rounds 12-22) after
  `RecompiledFuncs` (including this round's `funcs_13.c`/`funcs_8.c`) had
  already compiled cleanly. `build-ni0e/CMakeFiles/Makefile2` backed up to
  the scratchpad; bare `cmake build-ni0e` (no `-D`) reconfigure produced a
  **byte-identical** `Makefile2` (empty diff); all seven pre-existing cache
  flags confirmed unchanged. Rebuild then hit one real link failure of this
  round's own making (the three new probe functions in
  `ni_overlay_loader.cpp` were initially declared `static` instead of
  `extern "C"`, so the linker couldn't see them from the `.c` translation
  units); fixed, rebuilt clean.
- Targeted rebuild (`touch`ing only the three changed files) confirmed all
  three compile with **zero warnings/errors** of their own.
- `build/CMakeFiles/Makefile2` backed up; bare `cmake build` (no `-D`, flags
  OFF) hit the same spirv-cross hazard on first attempt, then produced a
  40-line diff on reconfigure — inspected in full and confirmed pure
  line-order reshuffling (sorted diff against the backup is empty); all seven
  flags confirmed `BOOL=OFF` except the pre-existing default-on
  `LOD_FIX_PAIR126_INPUT_RELEASE`.
- `cmake --build build --target LodRecomp -j8` (default tree, flag OFF):
  succeeded, clean, zero warnings on the three changed files — confirms the
  new probe code is fully inert when the flag is off.
- `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums of both
  binaries verified identical afterward (`e44f50e21aafe1ed36f7eafbe848cbb6`),
  so `./build/LodRecomp` now runs the Round-24-instrumented build.
- The game was not launched in this pass (per instructions); no live capture
  was taken. No commit was made (per instructions).

## 13. Round 25 (2026-07-22): pump-stall Task A resolved (all-in-one, blocking); Task B's assumed mechanism directly contradicted by the hang dump itself; probes-only

Given ground truth this round: a live lldb attach + 8MB RDRAM dump
(`/tmp/recomp_hang_rdram.bin`) of an actually-hung session, compared to a
hardware savestate in the same room. The dump's own binary was confirmed
(MD5 `e44f50e21aafe1ed36f7eafbe848cbb6`) to be the exact Round-24 build
already copied to `./build/LodRecomp` — i.e. `LOD_FIX_DMA_COMPLETION` and
`LOD_ENABLE_PIDMA_TRACE` (round 21) were both already `ON` in the binary
that produced this hang.

### 13.1 Task A.1: `func_80012D24` is ALL-CHUNKS-IN-ONE-CALL (blocking), not one-chunk-per-dispatch

Read in full (`RecompiledFuncs/funcs_8.c`, vaddr `0x80012D24`-`0x80012EC4`).
Its caller `func_80012C2C` pre-computes the total chunk count once
(`desc+0x83C = ceil((desc+0x838)/0x800)`, i.e. `ceil(chunkSize... total/0x800)`)
and calls `func_80012D24` exactly once per logical transfer. Inside,
`func_80012D24` is a C-level (MIPS-level) `while` loop, not a state machine
that returns between chunks:

```
while (progress < total-4) {           // v0 < s5, the loop's own condition
    ...
    DMA_ROMCopy(...)                   // jal 0x8001A42C -- synchronous, one 0x800-byte chunk
    ...
    func_800A761C(...)                 // per-chunk post-copy callback (verified this round:
                                        // no jal to osRecvMesg/osSendMesg anywhere in its own
                                        // ~970-line body or its two direct callees -- not a
                                        // second blocking-wait site)
    chunks_remaining = desc[0x83C] - 1;
    desc[0x83C] = chunks_remaining;
    if (chunks_remaining > 0) continue; // L_80012E68, loops back to the top -- SAME call frame
    else break;                         // falls through to the function's single return
}
```

(`L_80012DA8` is the loop top; `L_80012E70`'s `bnel $at,$zero,L_80012DA8` is
the back-edge; `L_80012E40`-`L_80012E68` is the `desc+0x83C` decrement and
continue/break test — every line quoted matches the compiled source exactly,
see `RecompiledFuncs/funcs_8.c` lines ~2589-2884 post-instrumentation.) The
function only returns to its caller (`func_80012C2C`) once `desc+0x83C`
reaches 0 (or an align-fail/early-exit path). **This directly answers the
task's own decisive fork: since it is all-in-one, the ground truth's
`chunks_remaining=1` (mid-transfer, not 0) means the thread is synchronously
*inside* this one call to `func_80012D24`, not "stopped being dispatched" —
consistent with the ground truth's own call-stack read
(`DMA_ROMCopy -> DMA_readWrite -> osEPiStartDma_recomp + osRecvMesg_recomp`).**

### 13.2 Task A.2: the dispatch wrapper, and the ceiling already documented by rounds 21/22/24

`func_80012AB0` (`funcs_8.c`, vaddr `0x80012AB0`) is the generic
recursion-guarded sub-state dispatch wrapper the ground truth describes:
it reads `obj+0xE` (a 16-bit depth counter), increments it, computes
`record = obj + depth*2`, reads `(count, state_index)` at `record+8`/
`record+9`, increments `count` in place, and `jalr`s
`table_0x800AF58C[state_index]` (`0x800B0000-0xA74`) — confirmed byte-for-byte
against the ground truth's own address arithmetic. Its own caller (who ticks
the pump object, id 4, each frame) is `func_80011D10`, already named by
round 21 §10.4/round 22 §11.3/round 24 §12.5 as having **zero static `jal`
callers anywhere in `RecompiledFuncs/*.c`** — reachable only through the
runtime function-pointer table `D_800AF560`, whose *contents* (which object
IDs map to which dispatch table, and what ticks `D_800AF560` itself) live in
a `.data`/`.rodata` region this checkout's `asm/` dump has no text
representation for. This is the same hard ceiling every prior round in this
investigation hit for this exact chain; nothing in this round's reading
closes it. Whether the pump object could leave the live dispatch set on the
loaded-Henry transition therefore remains unresolved by static reading
alone, same as before.

### 13.3 New evidence that complicates Task B's assumed mechanism: the frozen queue's own struct is quiescent, not lost/full/corrupt

Task B's second branch (all-in-one call, stuck in `osRecvMesg`) directs
extending `LOD_FIX_DMA_COMPLETION` to fix "the exact lost-message window."
Before writing that fix, this round read the actual `OSMesgQueue` at
`0x800C5D18` directly out of the hang dump (byte-swap convention verified
first against a known-good ground-truth value, `sys+0x2B24` at
`0x801CADE4` reading `0x802E7B60` exactly; struct layout taken from
`ultramodern/include/ultramodern/ultra64.h:113-120`):

```
blocked_on_recv (+0x0): 0x00000000   <- NULL: no thread registered as waiting on this queue
blocked_on_send (+0x4): 0x00000000
validCount      (+0x8): 0            <- empty, not full
first           (+0xC): 0
msgCount       (+0x10): 1            <- sane (matches the classic depth-1 dmaMessageQ)
msg ptr        (+0x14): 0x800c5d30   <- sane (immediately after the struct)
```

This is a **fully quiescent, well-formed** queue: not `MQ_IS_FULL` (round 21
candidate 1, a stale unclaimed message), not failing the `validCount`/
`msgCount` sanity check (round 21 candidate 2, memory corruption), and
critically **no thread is currently registered in `blocked_on_recv`**. Per
`ultramodern/src/mesgqueue.cpp`'s `do_recv` (read again this round together
with `threadqueue.cpp`'s `thread_queue_insert`, which really does write
straight into this RDRAM field — `GET_MEMBER(OSMesgQueue, mq, blocked_on_recv)`
is literal address arithmetic, `TO_PTR` in this build converts it straight
into a pointer inside the same `rdram` buffer the dump captured, verified via
`ultramodern/include/ultramodern/ultra64.h:31-43`), a thread genuinely
parked inside `do_recv`'s blocking `while` loop for *this* queue **must**
have called `thread_queue_insert(blocked_on_recv, self)` as the unconditional
first statement of that same loop iteration — so `blocked_on_recv` should
read non-NULL for as long as that specific wait is still outstanding, unless
something already popped it back out (a successful `do_send`, which would
also leave `validCount` at 1, not 0, until the popped thread's own `do_recv`
runs to completion and decrements it back down — which in turn means that
thread is no longer suspended in this call at all).

**In short: this queue's own state is inconsistent with "a thread is right
now blocked here waiting for a completion that was lost."** Either the
completion for this exact receive already arrived and was consumed (in
which case the freeze is not here, and is downstream — e.g. still inside
`func_800A761C`'s callback, or after `func_80012D24` returns, back in
`func_80012C2C`/`func_80012AB0`/whatever drives it per §13.2), or the
snapshot's read of "which call is live" from a symbol-only native backtrace
is closer than not but not, at the OS-message-queue level of detail, a
generic "any `osRecvMesg_recomp` frame implies the classic
`0x800C5D18`-DMA_readWrite one" reading may not resolve which specific queue
address (`ctx->r4` at that exact frame) is actually being waited on. Round
21's own already-shipped `LOD_FIX_DMA_COMPLETION`/`LOD_ENABLE_PIDMA_TRACE`
were both already `ON` in the exact binary that produced this hang (§13,
above) and the hang still occurred — which is evidence the *specific*
lost-completion mechanism round 21 fixed is not (or is no longer, now that
it's fixed) what this particular hang is. Writing another guessed variant of
"retry/requeue harder" against a queue that the dump shows is not stuck,
full, or corrupt would very likely be a no-op at best and a
`feedback_no_artificial_progress`-violating band-aid at worst.

**Side finding (cosmetic, not fixed — out of scope for a minimal patch):**
cross-checking the descriptor's ring fields against the ground truth's own
`head slot fid=0x33` (computed via `desc + readidx*20 + 0x848 + 0xC`) found
`src/main/main.cpp`'s `LOD_NI0E_RING_READIDX_OFF`/`LOD_NI0E_RING_PENDING_OFF`
constants (`0x844`/`0x846`, also used in `funcs_8.c`'s own round-19
"consumer-hunt" comment) are swapped: `+0x844` is actually the pending
*count* and `+0x846` is actually the read *index* (using `+0x846` as the
index reproduces `fid=0x33` exactly; using `+0x844` does not). This round's
own new probes (§13.4) use the corrected mapping and say so in a comment;
the pre-existing (mislabeled-but-functionally-unaffected, since nothing
computes with the *label*, only the *offset*) constants elsewhere are left
untouched.

### 13.4 Probes shipped instead of a guessed fix

Per the task's own explicit fallback ("if unresolved from reading alone,
ship ONLY the Task A probes, no guessed fix, and say so") — that is exactly
the situation §13.3 leaves this round in for Task B, even though Task A
itself (§13.1) is fully resolved. Two probes added, both
`LOD_ENABLE_NI0E_TRACE`-gated, both in `RecompiledFuncs/funcs_8.c`:

- **`pump-tick`** (`lod_ni0e_pump_tick_probe`, hooked in `func_80012AB0`
  right before the sub-state `jalr`, the last point `state_index` is live
  before it's overwritten for the jump-table computation): obj, the
  post-increment state count and the about-to-run state index (the literal
  `obj+0x8`/`obj+0x9` bytes at depth 0, matching the ground truth's own
  citation), plus the pump's own ring descriptor's readidx/pending (corrected
  mapping, §13.3)/progress(+0x82C)/chunks_remaining(+0x83C). Rate-limited
  first 80 + every 200, per the task brief.
- **`pump-chunk`** (`lod_ni0e_pump_chunk_entry_probe`/
  `lod_ni0e_pump_chunk_exit_probe`, hooked at `func_80012D24`'s entry and its
  single exit point `L_80012E98`): descriptor address (captured into a plain
  C local at entry, since the MIPS register that holds it, `$s1`, is a
  callee-saved register the function's own epilogue restores to the
  *caller's* value right before `L_80012E98`'s later instructions — reading
  it from the register at exit would be wrong), progress(+0x82C), and
  chunks_remaining(+0x83C), before and after. Same rate limit.

A future capture filtered to `grep 'pump-tick\|pump-chunk'` is decisive: if
`pump-chunk site=entry` logs once for the fatal transfer and **no matching
`site=exit` or any further `pump-tick`/`pump-chunk` line ever follows** for
the rest of the session, that confirms §13.1's reading directly (call is
live, synchronously stuck inside one `DMA_ROMCopy`/`DMA_readWrite`) and the
next step is a targeted `osRecvMesg_recomp` argument-register dump at that
exact call site (not a repeat of round 21's generic queue-completion fix) to
see which `mq` address it is actually blocked on, resolving §13.3's open
question. If instead `pump-tick` simply stops appearing at all (no entry,
no exit, nothing) across the fatal transition, that points to §13.2's
dispatch-stop scenario instead, and the next step is the `D_800AF560`/
`func_80011D10` caller hunt every prior round already flagged as needing a
`.data` dump this checkout cannot produce statically.

### 13.5 Files changed (Round 25)

- `RecompiledFuncs/funcs_8.c` — new `LOD_ENABLE_NI0E_TRACE` probes
  `lod_ni0e_pump_tick_probe`/`lod_ni0e_pump_chunk_entry_probe`/
  `lod_ni0e_pump_chunk_exit_probe`; hooked into `func_80012AB0` (one call
  site) and `func_80012D24` (entry + single exit). No behavior change when
  the flag is off. (This file is gitignored per `RecompiledFuncs/` in
  `.gitignore`, like most of `RecompiledFuncs/` other than the handful of
  files already force-added in earlier rounds — the change is real and
  compiled/linked into both binaries below, just not tracked by git, same as
  this file's pre-existing round 18/19/24 probes.)
- `docs/issue27-31-fix-design.md` — this section.
- `docs/issue27-31-ni0e-findings.md` — Round 25 pointer/summary section.
- No `lib/N64ModernRuntime` changes this round (read `pi.cpp`/`mesgqueue.cpp`/
  `threads.cpp`/`scheduling.cpp`/`threadqueue.cpp` in full again, per
  instructions, but did not extend `LOD_FIX_DMA_COMPLETION` — see §13.3 for
  why a host-layer patch was not the right minimal move this round).
- No `CMakeLists.txt` changes needed — reuses the existing
  `LOD_ENABLE_NI0E_TRACE` flag, already `ON` in `build-ni0e` and `OFF` in
  `build`.

### 13.6 Build/deploy (Round 25)

- `build-ni0e/CMakeFiles/Makefile2` and `build/CMakeFiles/Makefile2` both
  backed up to the scratchpad before reconfiguring, per the documented
  recovery procedure. Bare `cmake build-ni0e` (no `-D`, cache-only) produced
  a **byte-identical** `Makefile2` (empty diff) — no spirv-cross hazard this
  round. All relevant pre-existing cache flags
  (`LOD_ENABLE_NI0E_TRACE`, `LOD_ENABLE_PIDMA_TRACE`,
  `LOD_FIX_DMA_COMPLETION`, `LOD_FIX_PAIR126_INPUT_RELEASE`,
  `LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN`, `LOD_FIX_TEXT_MEASURE_GUARD`)
  confirmed unchanged (`ON`/`ON`/`ON`/`ON`/`ON`/`ON` respectively) in
  `build-ni0e`.
- `cmake --build build-ni0e --target LodRecomp -j8`: succeeded. `funcs_8.c`
  itself compiled with **zero warnings/errors**, confirmed both in the full
  build log and again via a targeted rebuild after `touch`ing only that
  file. The only warnings anywhere in the full build are the pre-existing,
  unrelated `sse2neon.h`/shader `-Wunused-variable` warnings.
- `build/CMakeFiles/Makefile2` reconfigure (bare `cmake build`, no `-D`)
  produced a 40-line diff against its own backup; a sorted diff against the
  backup was **empty**, confirming pure line-order reshuffling (the same
  benign pattern rounds 19/22/24 already documented for this tree); all
  three flags (`LOD_ENABLE_NI0E_TRACE`, `LOD_ENABLE_PIDMA_TRACE`,
  `LOD_FIX_DMA_COMPLETION`) confirmed `BOOL=OFF` in `build/CMakeCache.txt`.
- `cmake --build build --target LodRecomp -j8` (default tree, flags OFF):
  succeeded, clean except the same pre-existing warnings — confirms the new
  probe code is fully inert when the flag is off.
- `cp build-ni0e/LodRecomp build/LodRecomp` applied; MD5 checksums of both
  binaries verified identical afterward (`1279988d901bdeabc7a2169a76a6076e`),
  so `./build/LodRecomp` now runs the Round-25-instrumented build.
- The game was not launched in this pass (per instructions). No commit was
  made (per instructions).
