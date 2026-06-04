# Castlevania: Legacy of Darkness — Function Map

Names derived from CV64 decomp (k64ret/cv64) cross-reference and runtime analysis.
Names updated in `castlevania2.syms.toml` — re-run N64Recomp to regenerate recompiled code with these names.

## Object System
| Address | Name | Description |
|---------|------|-------------|
| 0x80000578 | GameStateMgr_execute | Root object manager — dispatches scene via scene_arg |
| 0x800007B0 | scene_init | Scene init — writes scene_arg, calls scene loader |
| 0x80001CE8 | object_curLevel_goToNextFunc | Advances object's state schedule |
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
| 0x80098B50 | contpak_readPIF | Low-level PIF SI DMA read/write |
| 0x80098BD4 | contpak_parsePIF | Parses PIF response, extracts button data |
| 0x80098C60 | contpak_formatPIF | Formats PIF command buffer for button read |

## Save Screen / map_ovl_34
| Address | Name | Description |
|---------|------|-------------|
| 0x802EBAF0 | save_screen_outer_dispatch | map_ovl_34 wrapper that dispatches the outer save-screen table at `0x802F7170` |
| 0x802ECA84 | save_screen_schedule_dispatch | Outer-table state 2 wrapper that may dispatch the inner table at `0x802F71A8` |
| 0x802ECF4C | save_screen_outer_state3_update | Outer-table state 3 handler; current fixed parent object reaches this state |
| 0x802F7170 | save_screen_outer_state_table | map_ovl_34 data jump table used by `save_screen_outer_dispatch` |
| 0x802F71A8 | save_screen_inner_state_table | map_ovl_34 data jump table used by `save_screen_schedule_dispatch` |
| 0x802ED804 | save_screen_state3_update | Inner-table state 3 update handler |

Decoded `save_screen_outer_state_table` entries from map_ovl_34 data:

| State | Handler |
|-------|---------|
| 0 | 0x802EBCBC |
| 1 | 0x802EC550 |
| 2 | `save_screen_schedule_dispatch` |
| 3 | `save_screen_outer_state3_update` |
| 4 | 0x802EDBD8 |
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
| 4 | 0x802ED630 |
| 5 | 0x802ED6C4 |
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
