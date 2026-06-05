# Castlevania: Legacy of Darkness — Function Map

Names derived from CV64 decomp (k64ret/cv64) cross-reference and runtime analysis.
Names updated in `castlevania2.syms.toml` — re-run N64Recomp to regenerate recompiled code with these names.

## Object System
| Address | Name | Description |
|---------|------|-------------|
| 0x80000578 | GameStateMgr_execute | Root object manager — dispatches scene via scene_arg |
| 0x800007B0 | scene_init | Scene init — writes scene_arg, calls scene loader |
| 0x80001C20 | object_curLevel_goToNextFunc | Advances the current object schedule level and clears deeper levels |
| 0x80001C5C | object_prevLevel_goToNextFunc | Advances the previous object schedule level and clears deeper levels |
| 0x80001C9C | object_nextLevel_goToNextFunc | Advances the next object schedule level and clears deeper levels |
| 0x80001CE8 | object_curLevel_goToNextFuncAndClearTimer | Advances the current object schedule level, clears its timer byte, and clears deeper levels |
| 0x80001D28 | object_curLevel_goToPrevFunc | Rewinds the current object schedule level and clears deeper levels |
| 0x80001D64 | object_prevLevel_goToPrevFunc | Rewinds the previous object schedule level and clears deeper levels |
| 0x80001DA4 | object_nextLevel_goToPrevFunc | Rewinds the next object schedule level and clears deeper levels |
| 0x80001DF0 | object_curLevel_goToPrevFuncAndClearTimer | Rewinds the current object schedule level, clears its timer byte, and clears deeper levels |
| 0x80001E30 | object_curLevel_goToFunc | Sets the current object schedule level to a target function and clears deeper levels |
| 0x80001E64 | object_goToFuncInLevel | Sets an explicit object schedule level to a target function and clears deeper levels |
| 0x80001EA0 | object_prevLevel_goToFunc | Sets the previous object schedule level to a target function and clears deeper levels |
| 0x80001ED8 | object_nextLevel_goToFunc | Sets the next object schedule level to a target function and clears deeper levels |
| 0x8000224C | object_allocate | Allocates object struct (bzero 0x74 bytes) |
| 0x80002410 | object_create | Creates object from template ID |
| 0x80002808 | object_allocEntryInList | Allocates data in object's alloc_data[] array |
| 0x80002CE0 | object_free | Default destroy function |
| 0x80005A30 | sceneLookup | Looks up scene state entry from descriptor table |
| 0x80010B84 | object_executeChildObject | Iterates parent GSS_SLOT entries, creates child objects |
| 0x80010DD4 | object_execute | Dispatches object's state function |
| 0x801431CC | sceneStateCopy | Reads alloc_data[15] (+0x70), copies transform data |

## Object Struct (from CV64 decomp: `Object`, size=0x74)
| Offset | Type | Name | Description |
|--------|------|------|-------------|
| 0x00 | s16 | ID | Object type ID |
| 0x02 | u16 | flags | Object flags |
| 0x04 | u16 | timer | Misc timer |
| 0x08-0x0D | ObjectFuncInfo[3] | current_function | State schedule (3 levels × 2 bytes) |
| 0x0E | s16 | function_info_ID | Current schedule counter |
| 0x10 | func* | destroy | Destroy/dispatch function pointer |
| 0x14 | Object* | parent | Parent object pointer |
| 0x18 | Object* | next | Next sibling pointer |
| 0x1C | Object* | child | First child pointer |
| 0x20 | u16 | alloc_data_entries | Bitmask of allocated alloc_data slots |
| 0x22 | u16 | graphic_container_entries | Bitmask of graphic container slots |
| 0x24-0x33 | Figure*[4] | figures | Display model/mesh pointers |
| 0x34-0x73 | void*[16] | alloc_data | Generic data pointers (heap-allocated) |

## Scheduler
| Address | Name | Description |
|---------|------|-------------|
| 0x800178D0 | graphThread_entrypoint | Thread 5: main render loop |
| 0x80017DB8 | (init frame handler) | Called when D_800B0088==0 (dispatches objects) |
| 0x80017EC0 | (GFX frame handler) | Called when D_800B0088!=0 (builds display list) |
| 0x800184E0 | scheduler_create | Init scheduler struct, create queues, register events |
| 0x80018774 | scheduler_eventHandler | Thread 19: receives VI/SP/DP events, dispatches |
| 0x80018968 | scheduler_eventBroadcast | Iterates client list, sends msg to each |
| 0x800189B8 | scheduler_executeAudio | Thread 18: runs audio+GFX tasks on RSP |
| 0x80018B50 | scheduler_executeGraphics | Thread 17: processes GFX display lists |
| 0x80018E30 | scheduler_taskDispatch | Sends task to scheduler cmdQ via osJamMesg |
| 0x80019034 | createGraphicTasks | Builds GFX task struct, submits to scheduler |

## Audio / libaudio
| Address | Name | Description |
|---------|------|-------------|
| 0x800973A0 | alSynSetVol | libaudio voice volume event scheduler; CV64 fingerprint match |
| 0x80099278 | alSynAllocVoice | libaudio voice allocator; CV64 fingerprint match |
| 0x80099930 | alSynSetFXParam | libaudio FX parameter setter; CV64 fingerprint match |
| 0x800999B0 | alSynStartVoice | libaudio voice start helper; CV64 fingerprint match |
| 0x8009B1A0 | alSynSetPitch | libaudio voice pitch setter; CV64 fingerprint match |
| 0x8009B940 | alSynSetPan | libaudio voice pan setter; CV64 fingerprint match |
| 0x8009C790 | alSynNew | libaudio synthesizer init; CV64 fingerprint match |
| 0x8009CA90 | alAudioFrame | libaudio frame/alist builder; CV64 fingerprint match |
| 0x8009D2EC | alEnvmixerNew | envmixer node initializer; CV64 fingerprint match |
| 0x8009E480 | alEnvmixerPull | envmixer pull callback; emits SETVOL/ENVMIXER RSP commands |
| 0x8009E988 | alEnvmixerParam | envmixer param/event callback; manages source pointer and queued envelope events |
| 0x800A1540 | alSynSetFXMix | libaudio voice FX mix setter; CV64 fingerprint match |
| 0x800A1700 | alSynGetFXRef | libaudio FX reference getter; CV64 fingerprint match |
| 0x800A3AF0 | alSynAddPlayer | libaudio player registration; CV64 fingerprint match |
| 0x800A4DF0 | alSynStartVoiceParams | libaudio combined voice start/vol/pan/fx/pitch helper; CV64 fingerprint match |
| 0x800A7110 | alSynAllocFx | libaudio FX allocator; CV64 fingerprint match |
| 0x800A71B0 | alSynStopVoice | libaudio voice stop helper; CV64 fingerprint match |

## Gamestate IDs
| ID | Name | Notes |
|----|------|-------|
| 1 | `SCENE_KONAMI_LOGO` | Boot logo flow |
| 4 | `SCENE_SAVE_GAME` | Save/Controller Pak screen; LoD starts here before logo/intro flow |
| 5 | `SCENE_INTRO_CUTSCENE` | Current post-Expansion-Pak target; crashes after NI pair 104 in `G12-001A` |
| 6 | `SCENE_TITLE_SCREEN` | Expected title-screen state from older skip comments |
| 12 | `SCENE_EXPANSION_PAK` | Expansion Pak screen, visually confirmed during recovery |

## Controller Pak / Input
| Address | Name | Description |
|---------|------|-------------|
| 0x800195E0 | contpak_init | PADMGR init, calls osContStartReadData |
| 0x800196DC | contpak_mainLoop | PADMGR loop, reads controllers periodically |
| 0x8001CCC0 | contpak_get_inserted_status | Checks CONT_CARD_ON, sets contpak_uninserted |
| 0x8001CDAC | contpak_init_all | Initializes Controller Pak state for all controller slots |
| 0x8001CE78 | contpak_init_pak | Initializes Controller Pak state for one controller slot |
| 0x8001CF0C | contpak_alloc_file | Wrapper around osPfsAllocateFile |
| 0x8001CFA4 | contpak_delete_file | Wrapper around osPfsDeleteFile-style PFS API |
| 0x8001D024 | contpak_find_file | Wrapper around osPfsFindFile |
| 0x8001D0B0 | contpak_get_file_state | Wrapper around osPfsFileState |
| 0x8001D114 | contpak_get_num_files | Wrapper around osPfsNumFiles |
| 0x8001D178 | contpak_get_free_blks | Wrapper around osPfsFreeBlocks |
| 0x8001D1DC | contpak_is_plug | Wrapper around osPfsIsPlug |
| 0x8001D204 | contpak_read_file | Wrapper around osPfsReadWriteFile(PFS_READ) |
| 0x8001D288 | contpak_write_file | Wrapper around osPfsReadWriteFile(PFS_WRITE) |
| 0x8001D30C | contpak_check_rumble_pak | Checks whether inserted pak is a rumble pak |
| 0x8001D398 | contpak_check_inserted_err | Rechecks insertion and returns new-pack/no-pack style error |
| 0x8001D3E8 | contpak_backup_pfs_state | Copies live OSPfs slot to backup storage |
| 0x8001D458 | contpak_restore_pfs_state | Restores live OSPfs slot from backup storage |
| 0x8009F400 | pfs_find_file | libultra `osPfsFindFile`-equivalent routine; the old direct host shim was stale after Pak writes and is now default-off |
| 0x80098B50 | contpak_readPIF | Low-level PIF SI DMA read/write |
| 0x80098BD4 | contpak_parsePIF | Parses PIF response, extracts button data |
| 0x80098C60 | contpak_formatPIF | Formats PIF command buffer for button read |

## Save Screen / map_ovl_34
| Address | Name | Description |
|---------|------|-------------|
| 0x8001ACB0 | func_8001ACB0 | Main-code save/PFS state-table dispatcher; fans out through globals near `0x801D2908` |
| 0x8001AF54 | func_8001AF54 | Main-code save/PFS state-table exit candidate; calls `gamestate_change(3)`, then `object_curLevel_goToNextFuncAndClearTimer` |
| 0x8001AF90 | func_8001AF90 | Adjacent main-code save/PFS state-table handler; sets a save global, calls `0x80010558`, then advances schedule |
| 0x8001AFF0 | func_8001AFF0 | Adjacent main-code save/PFS state-table handler; disables several objects/pointers and rewinds/advances via `0x80001C5C` |
| 0x8001B0B4 | func_8001B0B4 | Adjacent main-code save/PFS state-table handler; re-enables objects, calls `0x80010558`, then advances schedule |
| 0x802EBAF0 | save_screen_outer_dispatch | map_ovl_34 wrapper that dispatches the outer save-screen table at `0x802F7170` |
| 0x802ECA84 | save_screen_schedule_dispatch | Outer-table state 2 wrapper that may dispatch the inner table at `0x802F71A8` |
| 0x802ECF4C | save_screen_outer_state3_update | Outer-table state 3 handler; current fixed parent object reaches this state |
| 0x802ED630 | save_screen_inner_state4_init | Inner-table state 4 initializer; writes motion/vector fields under object data `+0x1C/+0x20/+0x24`, sets `data+0x46=0x78`, then advances via `object_curLevel_goToNextFuncAndClearTimer` |
| 0x802ED6C4 | save_screen_inner_state5_update | Inner-table state 5 update; uses `data+0x48/+0x50` and movement/result helpers; if `data+0x50 == 0`, clears `data+0x46` and returns, otherwise advances the current schedule level |
| 0x802EDBD8 | save_screen_outer_state4_bridge | Outer-table state 4 bridge; temporarily dispatches the next schedule level through `0x802F71D0`, then may force current level to state `9` via `object_curLevel_goToFunc` when `data+0x46 <= 0`, input flag `0x40` is set, or global `0x802F6FE4 == 1` |
| 0x802F7170 | save_screen_outer_state_table | map_ovl_34 data jump table used by `save_screen_outer_dispatch` |
| 0x802F71A8 | save_screen_inner_state_table | map_ovl_34 data jump table used by `save_screen_schedule_dispatch` |
| 0x802ED804 | save_screen_state3_update | Inner-table state 3 update handler |

ROM scan note:

- The sequence at ROM `0xB43C0` is a main-code save/PFS state schedule table:
  `00090009 8001A63C 8001A6BC 8001A74C ... 8001ACB0 80010B40 8001AF54 800107F0
  8001AF90 80010B40 8001AFF0 8001B0B4 ...`.
- `func_8001AF54` is therefore a natural state-table target, not a standalone forced skip.
- `G4-001E` tracing did not observe this table or `func_8001AF54` in the current
  auto-input loop; the active tested path remained map_ovl_34 object scheduling.

Decoded `save_screen_outer_state_table` entries from map_ovl_34 data:

| State | Handler |
|-------|---------|
| 0 | 0x802EBCBC |
| 1 | 0x802EC550 |
| 2 | `save_screen_schedule_dispatch` |
| 3 | `save_screen_outer_state3_update` |
| 4 | `save_screen_outer_state4_bridge` |
| 5 | 0x802EE454 |
| 6 | 0x802ED1E8 |
| 7 | 0x802ED3AC |
| 8 | 0x802EDCA0 |
| 9 | 0x802EE908 |
| 10 | 0x802EEF3C |
| 11 | 0x802EBEC0 |
| 12 | 0x802EC0B8 |
| 13 | 0x802EC848 |

Decoded `save_screen_inner_state_table` entries from map_ovl_34 data:

| State | Handler |
|-------|---------|
| 0 | 0x802ECD18 |
| 1 | 0x802ED578 |
| 2 | 0x802ED6C4 |
| 3 | `save_screen_state3_update` |
| 4 | `save_screen_inner_state4_init` |
| 5 | `save_screen_inner_state5_update` |
| 6 | 0x802ED804 |
| 7 | 0x802ED630 |
| 8 | 0x802ED6C4 |
| 9 | 0x802ED804 |
| 10 | 0x802EDD68 |
| 11 | 0x802EE358 |
| 12 | 0x802EE420 |
| 13 | 0x802EE1A0 |
| 14 | 0x802EE358 |
| 15 | 0x802EE420 |

Known state-3 inputs:

- Save-screen dispatch functions use `h(obj+0x0E) + 1` to select the active
  two-byte schedule entry from `obj+0x08..0x0D`; the state byte is
  `u8(obj + (h(obj+0x0E)+1)*2 + 0x09)`.
- `save_screen_outer_state3_update` expects `Object + 0x24` and `Object + 0x34`
  to be valid pointers.
- `Object + 0x34` is the inner state-3 base allocation used as `s0`.
- `save_screen_state3_update` unconditionally advances `s0 += 8` after reading
  base `s0+0x48`, so later `s0+0x40` refers to base allocation `+0x48`.
- Base allocation `+0x48 == 2` selects the nested `ctx->0x14->0x14->0x14`
  source and helper `0x802F5584`; otherwise it uses the direct `ctx->0x14`
  source and helper `0x802F0014`.
- Global `0x801CAB18` bits `0x10` and `0x40` adjust a state-3 parameter.
- Global pointer `0x801CAC20` is the context root used by the direct/nested source branch.

## NI (Nisitenma-Ichigo) System
| Address | Name | Description |
|---------|------|-------------|
| 0x801CB5CC | NI_systemInit | Creates NI file management object at sys+0x295C |
| 0x0F000000 (NI pair 104) | ni_ovl_104_schedule_dispatch | Pair-104 schedule dispatcher: dispatches table `0x0F0026F8[obj schedule state]`; current post-Expansion-Pak `gs=5` crash path enters here |
| 0x0F000130 (NI pair 104) | ni_ovl_104_func_0F000130 | Pair-104 state handler in current `gs=5` crash path; role still under investigation |
| 0x0F000484 (NI pair 120) | ni_ovl_120_result_schedule_dispatch | Pair-120 result dispatcher: calls callback table `0x0F0016BC[obj+0x48]`, stores `v0` at `obj+0x38`, resets current schedule to `0` when result is nonzero, otherwise sets `obj+0x44=1` and advances |

Pair-120 result callback table at `0x0F0016BC`:

| Index | Target | Notes |
|-------|--------|-------|
| 0 | 0x0F00086C | Internal label (`pair120_label_clear`): stores `a0` on stack, writes `-1` to `0x801CAE1C`, returns `0` |
| 1 | 0x0F000884 | Function start |
| 2 | 0x0F0008D4 | Internal label: stores/returns `0x10B` |
| 3 | 0x0F0008E4 | Internal label: stores/returns `0x10A` |
| 4 | 0x0F0008F4 | Function start |
| 5 | 0x0F00091C | Function start |
| 6 | 0x0F000958 | Internal label: copies `obj+0x3C` to `obj+0x38` and returns it |
| 7 | 0x0F000964 | Internal label: stores/returns `0x111` |
| 8 | 0x0F000974 | Internal label: stores/returns `0x112` |
| 9 | 0x0F000984 | Function start |
| 10 | 0x0F000070 | Function start |
| 11 | 0x0F0000BC | Function start |

Notes:

- The `0x0F...` VRAM range is reused by many NI pairs. Do not globally register pair-120 internal labels as loaded functions.
- The current compatibility fix scopes the internal-label behavior to `ni_ovl_120_result_schedule_dispatch` only.
- N64Recomp's range fallback resolves internal-label jumps to the containing function start; that is useful for many computed jumps but wrong for this callback table.

## Key Globals
| Address | Name | Description |
|---------|------|-------------|
| 0x800B0088 | D_800B0088 | "Video ready" flag (set by PRE-NMI handler) |
| 0x800ACFDC | scene_table | Scene descriptor table (80 bytes per entry, 20 words: 2 func ptrs + 18 data words) |
| 0x800C5E80 | sched_struct | Scheduler struct base |
| 0x800CE760 | gDisplayListHead | Current display list write pointer |
| 0x800EFB70 | si_event_queue | SI event message queue |
| 0x800EFB90 | pad_buffer | OSContPad[4] controller data |
| 0x800F2260 | contpak_uninserted | byte[4]: per-controller "no pak" flag |
| 0x8013ED70 | pif_buffer | 64-byte PIF command/response buffer |
| 0x8013EDB0 | pif_formatter_flag | "Already formatted" flag for PIF commands |
| 0x8013EDB1 | max_controllers | Number of detected controllers |
| 0x801C82C0 | sys | System struct base |
| 0x801CABC8 | sys+0x2908 | Execution flags (0x380080C8 = all enabled) |
| 0x801CAC1C | sys+0x295C | NI system object pointer |
| 0x8031AC78 | root_object | Root object (scene_arg at +0x24) |
| 0x8031B42C | save_object | Save screen overlay object (runtime addr, varies) |
