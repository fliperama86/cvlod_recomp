# LoD Function Map

Known functions discovered through debugging. Reference: [k64ret/cv64](https://github.com/k64ret/cv64) decomp (same engine, different game).

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
| — | msg_type=3 | L_80017D50: pre-NMI handler → func_80091108(0xA) |
| L_80017CB8 | Retrace dispatch | If guard==0 → func_80017DB8 (init). If guard!=0 → func_80017EC0 (frame SM) |

## Frame Processing

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x80017DB8 | `func_80017DB8` | Frame producer (guard=0 path) | Reads controller (func_800196DC), decrements timer at struct+0x88, calls object system (func_80002950), submits display list (func_8001A25C) |
| 0x80017EC0 | `func_80017EC0` | Frame state machine (guard!=0 path) | Checks state at 0x800B008C. States 0x00-0x10 produce display lists (17 init frames). State >= 0x11 returns immediately (no-op) |
| 0x80017E9C | `func_80017E9C` | | Reads field at struct+0x26, indexes into function table at struct+0x14, stores to 0x800CE760 |

## Object System

| Address | Name | cv64 Equivalent | Description |
|---------|------|-----------------|-------------|
| 0x80002950 | `func_80002950` | `object_execute`? | Takes object pointer, checks flags (bit 0x2000), calls func_80011060. **Currently hangs** |
| 0x80011060 | `func_80011060` | Object update handler? | Called from func_80002950. Unknown — needs investigation |
| 0x80000460 | `func_80000460` | Overlay/object loader | Takes index, looks up function table, calls loader, calls func_80001090 to init object |

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
| 0x8001A42C | `func_8001A42C` | DMA loader | Called during init for ROM→RAM transfers (DMA #1 and #2) |
| 0x80090C00 | `func_80090C00` | | Called during init, between DMA #2 and overlay loading |

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
| 0x800B0088 | u16 | Guard variable — set to 1 by pre-NMI scheduler handler. Controls retrace dispatch path |
| 0x800B008C | u16 | Frame state machine state (0x00-0x10 = init frames, 0x11+ = done) |
| 0x800C1520 | ptr | Pointer to main game object (set by func_80000460 during overlay loading) |
| 0x800C5D38 | OSMesgQueue | Main loop message queue (8 slots) |
| 0x800C5E80 | struct | Scheduler struct (0x8A0+ bytes) |
| 0x800C5EBC | OSMesgQueue | GRAPH thread input queue |
| 0x800CE760 | u32 | Written by func_80017E9C — function pointer? |
| 0x800EFB70 | OSMesgQueue | SI event queue (controller completion) |
| 0x800F9B60 | OSMesgQueue | Second client queue (registered with scheduler, likely GFX-related) |
| 0x801C82C0 | struct | Game state struct base (accessed via `lui 0x801D; addiu -0x7D40`). Fields at +0x00 (frame counter), +0x88 (timer/counter), +0x534 area (controller status) |
| 0x8031AC78 | object | Main game object loaded during init (passed to func_80002950) |
