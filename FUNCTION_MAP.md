# LoD Function Map

Known functions discovered through debugging. Reference: [k64ret/cv64](https://github.com/k64ret/cv64) decomp (same engine, different game).

> **TODO**: Once we have enough confidence in the names, rename the actual functions in `castlevania2.syms.toml` and re-run N64Recomp to regenerate the C files with meaningful names (e.g. `func_80018890` → `scheduler_register_client`). This will make future debugging and the codebase much more readable.

## Boot / Entry

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x80000400 | `recomp_entrypoint` | `bootproc` | Zeros BSS, jumps to game_main |
| 0x800177E0 | `game_main` | `idle_thread_func`? | Calls osContInit, creates IDLE thread (id=1, pri=9), starts it |
| 0x8001783C | IDLE thread entry | `idle_thread_func` | Creates PI manager (pri=150), creates MAIN thread (id=5, pri=10), sets own pri=0, enters pause_self loop |
| 0x800178D0 | `func_800178D0` | Main thread / GRAPH thread | Boot init: creates scheduler, loads overlays, enters main loop at L_80017C78 |

## Scheduler System

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x800184E0 | `func_800184E0` | `scheduler_create` | Creates 4 scheduler threads (0x10-0x13), sets up VI/SP/DP/pre-NMI events via osViSetEvent + osSetEventMesg |
| 0x80018774 | `func_80018774` | Scheduler event loop (thread 0x13, pri 130) | Receives VI retrace (0x29A) and pre-NMI (0x29D) messages, forwards to registered clients via func_80018968 |
| 0x80018890 | `func_80018890` | `scheduler_register_client` | Inserts client queue into linked list at scheduler+0x888. Uses osSetIntMask for atomicity |
| 0x80018968 | `func_80018968` | `scheduler_forward_message` | Walks client linked list at scheduler+0x888, calls osJamMesg to each client's queue |
| 0x800189B8 | `func_800189B8` | Task scheduler (thread 0x12, pri 120) | Manages RSP audio/graphics task submission and completion |
| 0x80018D28 | `func_80018D28` | VI swap handler (thread 0x10, pri 110) | Waits for VI events, swaps framebuffers via osViSwapBuffer |
| 0x80018E28 | `func_80018E28` | `scheduler_get_pending_count` | Returns scheduler+0x89C (pending task count). Single instruction: `lw $v0, 0x89C($a0); jr $ra` |
| GFX thread | `scheduler_executeGraphics` | (thread 0x11, pri 100) | GFX display list scheduler, blocks on GRAPH queue 0x800C5EBC |

## Main Loop (inside func_800178D0)

| Address | Label | Description |
|---------|-------|-------------|
| L_80017C78 | Main loop top | osRecvMesg on queue 0x800C5D38, dispatches by msg_type |
| — | msg_type=1 | Retrace → L_80017CB8: checks guard (0x800B0088) |
| — | msg_type=2 | L_80017CFC: frame counter increment, osViBlack, osViSetYScale/XScale |
| — | msg_type=3 | L_80017D50: scheduler-ready handler. Sets guard=1 at 0x800B0088. **In recomp: simulated after 4 init retraces** |
| L_80017CB8 | Retrace dispatch | If guard==0 → func_80017DB8 (init, dispatches objects). If guard!=0 → func_80017EC0 (frame SM, no object dispatch). **Currently stays in init mode for object dispatch** |

## Frame Processing

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x80017DB8 | `func_80017DB8` | Frame producer (guard=0 path) | Reads controller (func_800196DC), decrements timer at struct+0x88, calls object system (func_80002950), submits display list (func_8001A25C) |
| 0x80017EC0 | `func_80017EC0` | Frame state machine (guard!=0 path) | Checks state at 0x800B008C. States 0x00-0x10 produce display lists (17 init frames). State >= 0x11 returns immediately (no-op) |
| 0x80017E9C | `func_80017E9C` | | Reads field at struct+0x26, indexes into function table at struct+0x14, stores to 0x800CE760 |

## Object System

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x80002950 | `func_80002950` | `object_execute` | Executes one object. Path A (bit 0x2000 set): calls mapOverlay → func from table → func_8001123C. Path B (no overlay): calls func from table directly. Recurses for child objects via func_80000578 |
| 0x80011060 | `func_80011060` | `mapOverlay` | Maps overlay code via TLB for objects with OBJ_FLAG_MAP_OVERLAY (bit 0x2000). Uses overlay descriptor table at 0x800AEA8C. TLB implemented in `src/main/tlb_segment.cpp` |
| 0x8001123C | `func_8001123C` | `unmapOverlay`? | Called after overlay object execution to unmap TLB |
| 0x80000578 | `func_80000578` | `root_object_handler` | Root object handler (ID=1). Reads scene_arg from obj+0x24, dispatches to scene handler. State 0: init (func_80010B84 creates children). State 1: frame (func_80010C80 dispatches children). State 2+: func_80010DD4 |
| 0x80010B84 | `func_80010B84` | `object_createChildren` | Processes child spec entries at obj+0x34..0x74. For each non-zero entry, extracts index (lower 11 bits), looks up overlay descriptor at 0x800AEA8C, creates child via func_80002410 |
| 0x80010C80 | `func_80010C80` | `object_dispatchChildren` | Per-frame child dispatch — iterates child spec slots, calls func_80002950 on each valid child object |
| 0x80010EFC | `func_80010EFC` | `object_loadOverlay` | Sets bit 0x2000 in flags2 (overlay pending), searches loaded overlay list, triggers DMA if needed. **Bypassed in recomp**: always returns success since all overlays pre-loaded |
| 0x80002410 | `func_80002410` | `object_createChild` | Allocates child object via func_8000224C, loads overlay via func_80010EFC if descriptor exists |
| 0x80000460 | `func_80000460` | Overlay/object loader | Takes index, looks up function table, calls loader, calls func_80001090 (memcpy) to init object from template |
| 0x80001090 | `func_80001090` | `memcpy` | Copies N bytes from src to dest. Used to copy object template data to allocated objects |
| 0x8000233C | `func_8000233C` | Object allocator? | Called from func_80000460, returns object pointer (e.g. 0x8031AC78) |
| 0x8002A790 | `func_8002A790` | `objectOverlayExec` | Reads overlay selector from 0x800F3F54, writes it to obj ID, then calls mapOverlay. Called from func_8017747C |
| 0x8017747C | `func_8017747C` | Overlay dispatcher | Reads state byte from obj+0x9, indexes function table at 0x801927C8, dispatches call. Used by objects with idx=136 (flags=0x0088) |

## Game State System

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x80002E3C | `func_80002E3C` | `gameStateMgr_update` | Game state manager. Reads state field from 0x801C82E8 and cmd_list from obj+0x34. Processes command list to create/destroy game states. Object #2 (flags=0x0002) |
| 0x8002A6DC | `func_8002A6DC` | `gamestate_change` | Sets overlay selector at 0x800F3F54. Never called directly — only via LOOKUP_FUNC/jalr. Called when game transitions between states (e.g., logo → title) |

### Object Flags
- Bit 0x2000 in ID (halfword at obj+0x00): `OBJ_FLAG_MAP_OVERLAY` — needs TLB overlay mapping before execution
- Lower 11 bits of ID (& 0x7FF): index into `Objects_functions` table at 0x800AD640
- Halfword at obj+0x02: secondary flags

### Function Table (Objects_functions)
- Base: 0x800AD640 (in data gap between ..main and ..common)
- Entries 0-10: direct function pointers (0x80xxxxxx)
- Entries 11-14: overlay ROM addresses (0x0F000000) — need TLB/overlay loading
- Entry 15: 0x0F000B60 — another overlay ROM address

### Overlay Descriptor Table
- Base: 0x800AEA8C (0x800B0000 - 0x1574)
- Contains overlay metadata (ROM address, size, target vram) used by mapOverlay

## Controller / Input

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x800195E0 | `func_800195E0` | PADMGR init | Creates SI event queues, calls osContStartReadData once, sets up persistent SI event queue at 0x800EFB70 |
| 0x800196DC | `func_800196DC` | Controller read (per-frame) | Calls func_80098B50 (SI write), osRecvMesg (wait SI), func_80098BD4 (parse PIF response), updates controller state |
| 0x80098B50 | `func_80098B50` | `__osContStartReadData` (internal) | Formats PIF command buffer, calls __osSiRawStartDma(WRITE) |
| 0x80098BD4 | `func_80098BD4` | `__osContGetReadData` (internal) | Parses PIF response buffer into OSContPad button/stick data |

## Display List / Rendering

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x8001A25C | `func_8001A25C` | Display list submit? | Called after object system update. Submits GFX task to scheduler |
| 0x8001A230 | `func_8001A230` | | Called twice during init — possibly framebuffer setup |
| 0x8001A42C | `func_8001A42C` | `NI_fileLoad` | NI file loader. Reads NI table ptr from 0x800F13F8, calls func_8001A374 for ROM DMA. Called by DMAMgr state 1 handler |
| 0x80090C00 | `func_80090C00` | | Called during init, between DMA #2 and overlay loading |
| 0x801771E0 | `func_801771E0` | `renderContext_buildDL` | Builds display list commands (matrices, G_DL branches). Called by overlay state handlers. Calls func_80013CF8 |
| 0x80013CF8 | `func_80013CF8` | `gfx_appendCommands` | Appends view/projection matrix + sub-DL commands to the current display list |

## Game State Handler / NI System

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x80177860 | `func_80177860` | `gameStateHandler_main` | Main game state handler. Gated by sys+0x2908 bit 28 (`sll 3, bgez`). Calls func_8017A600, func_80177948/80177B4C, func_801780C4, func_80013CF8 |
| 0x8017A600 | `func_8017A600` | `NI_processFrame` | **KEY GATE**: Checks sys+0x295C, sys+0x2B6C, sys+0x2B70 — all must be non-zero. If any NULL, returns immediately (no rendering). Calls func_8017A790 → func_80181EF0 |
| 0x8017A790 | `func_8017A790` | `NI_renderFrame` | Actual frame rendering. Reads sys+0x2E2C, calls func_80180710, func_801797B8. Only reached when all 3 NI pointers are valid |
| 0x8001A8C4 | `func_8001A8C4` | `NI_createObjects` | Creates NI subsystem objects via func_8000233C. Sets sys+0x294C (spec 0x185), sys+0x2954 (spec 0x6), sys+0x2B70 (spec 0x89). **Called before root child creation in recomp** |
| 0x801CB5CC | `overlay_system_func_801CB5CC` | `NI_init` | NI file system initializer. Allocates 0x314 bytes via func_80002808, initializes NI file manager. Sets sys+0x295C. In overlay_system section |
| 0x80011D10 | `func_80011D10` | `DMAMgr_entry` | DMAMgr object entry (ID=4). Same state machine pattern as overlays. State 0: init (func_80011D80), State 1: processing (func_80011E48) |
| 0x80011D80 | `func_80011D80` | `DMAMgr_init` | DMAMgr state 0: allocates 0x988 bytes, registers self at 0x800C1600 (overlay load manager ptr) |
| 0x80011E48 | `func_80011E48` | `DMAMgr_processLoad` | DMAMgr state 1: checks pending count at ctx+0x846, processes load requests, calls func_8001A42C for ROM DMA. Transitions to state 2 on success |
| 0x800119CC | `func_800119CC` | `overlay_loadSection` | Overlay section loader. Reads load_mgr from 0x800C1600, processes overlay descriptor entries, queues DMA to DMAMgr |
| 0x80010EA0 | `func_80010EA0` | `overlay_initiateLoad` | Initiates overlay load. Reads 0x800C1600 (load_mgr) and descriptor, calls func_800119CC |
| 0x80142388 | `func_80142388` | `NI_checkTiming` | Reads game timing from sys+0x2B2C. Returns 0 when timing data not initialized |
| 0x8014314C | `func_8014314C` | `NI_checkFileReady` | Checks NI file readiness. Reads obj+0x70 (file handle), falls through to func_80142388 if NULL |

## Utility / Misc

| Address | Name | Description |
|---------|------|-------------|
| 0x80018764 | `func_80018764` | Trivial stub: `return $a0 + 0x3C` (8 bytes) |
| 0x8001875C | `func_8001875C` | Trivial stub: `return $a0 + 0x4` (8 bytes) |
| 0x8001876C | `func_8001876C` | Returns scheduler+0x898 |
| 0x80091108 | `func_80091108` | NMI handler — called with arg 0xA on pre-NMI |
| 0x800197F8 | `func_800197F8` | Called from func_80017DB8 when struct+0x86 == struct+0x88 |

## Key Memory Addresses

| Address | Type | Description |
|---------|------|-------------|
| 0x800B0088 | u16 | Guard variable — controls retrace dispatch (0=init, 1=frame SM). Set by msg_type=3 handler. **In recomp: simulated** |
| 0x800B008C | u16 | Frame state machine state (0x00-0x10 = init frames, 0x11+ = done) |
| 0x800B00C4 | table | Overlay entry table — pairs of ROM start/end addresses per overlay (segment 0x06 addresses) |
| 0x800AD640 | table | `Objects_functions` — function pointer/segment address table indexed by obj ID & 0x7FF |
| 0x800AEA8C | table | Overlay descriptor pointer table — per-function-index, points to overlay descriptor structs |
| 0x800C1520 | ptr | Pointer to root game object (set by func_80000460 during overlay loading) |
| 0x800C15E4 | u32 | Current overlay slot index (used by mapOverlay state table) |
| 0x800C15E8+ | table | Overlay state table — vram targets, sizes, ROM pages per slot |
| 0x800C5D38 | OSMesgQueue | Main loop message queue (8 slots) |
| 0x800C5E80 | struct | Scheduler struct (0x8A0+ bytes). +0x888=client list head, +0x89C=pending count |
| 0x800C5EBC | OSMesgQueue | GRAPH thread input queue |
| 0x800EFB70 | OSMesgQueue | SI event queue (controller completion) |
| 0x800F3F54 | i16 | **Overlay selector** — determines which overlay to load for overlay objects. Set by gamestate_change (func_8002A6DC) |
| 0x801CABC8 | u32 | **Execution flags** (sys+0x2908). Bit 28 gates func_80177860. Forced to 0x380080C8 in recomp |
| 0x801CAC1C | ptr | **NI file manager** (sys+0x295C). Set to object 12 (0x8031AF30) by overlay dispatch. Gates func_8017A600 |
| 0x801CAE2C | ptr | sys+0x2B6C — set naturally to 0x8031ADD4 |
| 0x801CAE30 | ptr | sys+0x2B70 — set by func_8001A8C4. Gates func_8017A600 |
| 0x801CADEC | ptr | sys+0x2B2C — game timing ptr. Read by func_80142388. NULL during Konami logo |
| 0x801C8830 | ptr[] | sys+0x570 — `Nisitenma_Ichigo_loaded_files_ptr[]` array (indexed by file ID) |
| 0x801C83EC | u32[64] | sys+0x12C — NI loaded files bitmask (64 × 32-bit = 2048 bits) |
| 0x800C1600 | ptr | Overlay load manager pointer. Set by DMAMgr state 0 (func_80011D80) to DMAMgr obj |
| 0x800C1604 | u32 | Current NI file ID being loaded by DMAMgr |
| 0x800F13F8 | ptr | NI file table ROM pointer. Set during boot by func_80090C00 |
| 0x800CE768 | u8 | DMA busy flag. Set to 1 during DMA, cleared after |
| 0x800F9B60 | OSMesgQueue | Second client queue (registered with scheduler, likely GFX-related) |
| 0x801C82C0 | struct | Game state struct base (`lui 0x801D; addiu -0x7D40`). +0x00=frame counter, +0x28=game state field, +0x88=timer, +0x534=controller status |
| 0x801C82E8 | i16 | Game state field — read by func_80002E3C to determine if state manager is initialized |
| 0x8031AC78 | object | Root game object. +0x34=cmd_list_ptr (0x9000C87C after init). Children: game state mgr, overlay dispatcher, etc. |
| 0x8031ACEC | object | Game state manager object (ID=0x0002). +0x34=cmd_list_ptr (should contain game state commands but reads as 0) |
| 0x8031AF30 | object | Overlay object (ID=0x200C). Calls mapOverlay → dispatches to overlay_system function |

## Segment Address System

The game uses N64 TLB-based segment addresses. TLB implemented in `src/main/tlb_segment.cpp` with page-aligned mprotect + memcpy.

| Segment | Address Pattern | Maps To | Status |
|---------|----------------|---------|--------|
| 0x09 | 0x9000XXXX | **KSEG0** (not a segment!) | **RESOLVED** — 0x9000XXXX = KSEG0, physical = addr - 0x80000000. ROM data at physical 0x10000000+ |
| 0x0C-0x0E | 0x0C/0D/0E000000+ | TLB-mapped overlay data | osMapTLB implemented, pages copied from physical RDRAM |
| 0x0F | 0x0F000000+ | overlay_system or DMA target | TLB-mapped. Function calls translated via segment→vram lookup. Data copied via osMapTLB |

### KSEG0 addresses (not TLB)
- `0x9000C87C` — root object command list pointer (ROM offset 0xC87C via KSEG0→physical 0x1000C87C)
- `0x90000864`, `0x9000C914`, `0x900065DC`, `0x90000A30` — ROM data pointers

### Framebuffers
- cfb[0] = 0x801DA800 (physical 0x1DA800) — **DL renders here**
- cfb[1] = 0x80200000 (physical 0x200000)
- cfb[2] = 0x80225800 (physical 0x225800)
- cfb[3] = 0x8024B000 (physical 0x24B000)
- VI rotates cfb[1-3] but DL always targets cfb[0]. Fixed by forcing osViSwapBuffer to cfb[0]
