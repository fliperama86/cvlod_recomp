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
| 0x8001CCC0 | contpak_statusCheck | Checks CONT_CARD_ON, sets contpak_uninserted |
| 0x80098B50 | contpak_readPIF | Low-level PIF SI DMA read/write |
| 0x80098BD4 | contpak_parsePIF | Parses PIF response, extracts button data |
| 0x80098C60 | contpak_formatPIF | Formats PIF command buffer for button read |

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
