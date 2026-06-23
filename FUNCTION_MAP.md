# Castlevania: Legacy of Darkness — Function Map

Names derived from CV64 decomp (k64ret/cv64) cross-reference and runtime analysis.
Names updated in `castlevania2.syms.toml` — re-run N64Recomp to regenerate recompiled code with these names.

## Object System
| Address | Name | Description |
|---------|------|-------------|
| 0x80000578 | GameStateMgr_execute | Root object manager — dispatches scene via scene_arg |
| 0x800007B0 | scene_init | Scene init — writes scene_arg, calls scene loader |
| 0x80004CB0 | sceneGraphPools_init | Initializes scene-node pool (`0xB8` bytes per entry) and scene-data pool |
| 0x80005484 | sceneDataPool_contains | Checks whether a pointer is inside the scene-data pool span |
| 0x800054B0 | sceneDataPool_alloc | Allocates a 64-byte scene-data pool entry |
| 0x80005530 | sceneDataPool_free | Frees a 64-byte scene-data pool entry |
| 0x80005574 | sceneNode_alloc | Allocates a scene graph node and optional associated scene-data entries |
| 0x8000572C | sceneNode_initFromDescriptor | Initializes scene graph node fields from descriptor tables |
| 0x800058A4 | sceneNode_linkChild | Links a scene graph node as a child |
| 0x800058F8 | sceneNode_linkSibling | Links a scene graph node through the sibling-style path |
| 0x80005A30 | sceneLookup | Allocates/looks up a scene graph node and links it into the scene |
| 0x80005ABC | sceneNode_createLinked | Allocates and links a scene graph node variant |
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


## Model / Dynamic Vertex Deformation
| Address | Name | Description |
|---------|------|-------------|
| 0x800532E4 | vec3f_transform_by_mat4f | Transforms a Vec3f by a 4x4 matrix; CV64 match `func_80011880`/vector-by-matrix helper |
| 0x80053B00 | model_sample_vertex_pos_to_vec3f | Samples one 16-byte vertex position (`s16 x/y/z`) by index into a Vec3f |
| 0x80053B48 | model_apply_vertex_delta_cmd | Applies one vertex-delta command: `dst_vertex = src_vertex + delta_vec * command_scale` |
| 0x80057B6C | model_compute_vertex_delta | Computes the model-space/world-space delta vector used by dynamic vertex deformation |
| 0x80057CAC | model_apply_scaled_vertex_delta_cmds | Scales a computed delta and applies a vertex-delta command list |
| 0x80057ED0 | model_refresh_anchor_from_vertex | Samples a model vertex and transforms it into `model+0x14` anchor/position fields |

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
| 0x0F001444 (NI pair 7) | ni_ovl_007_func_0F001444 | Pair-7 dispatcher that can call the `0x0F01A0F8` state table; table entry `0` targets internal label/function boundary `0x0F00162C` |
| 0x0F00162C (NI pair 7) | ni_ovl_007_func_0F00162C | Split from the tail of `0x0F001444`; required because pair-7 dispatch table targets this address directly, and range fallback to `0x0F001444` causes recursive re-entry |
| 0x0F010084 (NI pair 7) | ni_ovl_007_func_0F010084 | Pair-7 state handler reached from the `0x0F01A5C0` table; originally included the internal target `0x0F010218` in its tail |
| 0x0F010218 (NI pair 7) | ni_ovl_007_func_0F010218 | Split from the tail of `0x0F010084`; required because pair-7 table entry `3` targets this address directly, and range fallback to `0x0F010084` caused recursive re-entry |
| 0x0F000F9C (NI pair 23) | ni_ovl_023_bike_skeleton_root_update | Pair-23 root update/wrapper: dispatches table `0x0F006F80[state]`; state `3` reaches `ni_ovl_023_bike_skeleton_spawn_dispatch` |
| 0x0F003358 (NI pair 23) | ni_ovl_023_bike_skeleton_state_spawn_gate | Pair-23 state-table handler reached from `0x0F006F80`; manages child/effect spawn gate state |
| 0x0F00469C (NI pair 23) | ni_ovl_023_bike_skeleton_spawn_dispatch | Pair-23 bike-skeleton spawn sub-dispatcher: dispatches two-entry table `0x0F006FC8[state]`; entry `1` is internal label `0x0F004A24`, and entry `2` falls into adjacent `"No memory..."` string data |
| 0x0F004744 (NI pair 23) | ni_ovl_023_bike_skeleton_spawn_init | Pair-23 bike-skeleton spawn/init state; contains internal update label `0x0F004A24` used by the `0x0F006FC8` table |
| 0x0F000000 (NI pair 104) | ni_ovl_104_schedule_dispatch | Pair-104 schedule dispatcher: dispatches table `0x0F0026F8[obj schedule state]`; current post-Expansion-Pak `gs=5` crash path enters here |
| 0x0F000130 (NI pair 104) | ni_ovl_104_func_0F000130 | Pair-104 state handler in current `gs=5` crash path; role still under investigation |
| 0x0F000484 (NI pair 120) | ni_ovl_120_result_schedule_dispatch | Pair-120 result dispatcher: calls callback table `0x0F0016BC[obj+0x48]`, stores `v0` at `obj+0x38`, resets current schedule to `0` when result is nonzero, otherwise sets `obj+0x44=1` and advances |

Pair-23 root state table at `0x0F006F80`:

| Index | Target | Notes |
|-------|--------|-------|
| 0 | 0x0F0032E8 | State handler |
| 1 | 0x0F00308C | State handler |
| 2 | 0x0F002C54 | State handler |
| 3 | 0x0F00469C | `ni_ovl_023_bike_skeleton_spawn_dispatch` |
| 4 | 0x0F002D68 | State handler |
| 5 | 0x0F002E04 | State handler |
| 6 | 0x0F002EDC | State handler |
| 7 | 0x0F002FB4 | State handler |

Pair-23 bike-skeleton spawn callback table at `0x0F006FC8`:

| Index | Target | Notes |
|-------|--------|-------|
| 0 | 0x0F004744 | Function start (`ni_ovl_023_bike_skeleton_spawn_init`) |
| 1 | 0x0F004A24 | Internal label inside `ni_ovl_023_bike_skeleton_spawn_init`; update path for the spawn timer/child display bytes |
| 2 | 0x4E6F206D | Not a function; first word of adjacent string data (`"No memory..."`) |

Notes:

- Pair 23 has the same internal-label hazard as pair 120: N64Recomp's range fallback resolves `0x0F004A24` to `0x0F004744`, which reruns initialization instead of the intended update label.
- The compatibility fix is scoped to `ni_ovl_023_bike_skeleton_spawn_dispatch`; it does not globally register `0x0F004A24` in the reused `0x0F...` NI address range.
- Handwritten NI shims must preserve MIPS callee-saved registers and canonicalize 32-bit KSEG pointers to sign-extended `gpr` values before returning. Otherwise zero-extended `0x80xxxxxx` pointers can become host offsets around `rdram+0x100xxxxxx`.

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

General NI call-table boundary audit:

- `tools/audit_indirect_targets.py --scan-call-tables` detects raw-data callback/state tables by locally slicing MIPS `jalr` table-load patterns. This catches tables beyond a section's executable byte range without the false positives from scanning the full raw blob.
- `--fix-syms` splits missing call-table targets that land inside declared functions, using generated `ni_ovl_XXX_func_XXXXXXXX` names. The generalized pass added 166 build-safe NI function-boundary splits after the `ni_ovl_015_knight_clear_motion_substate` fix.
- Regen now runs the strengthened audit with `--scan-call-tables --fail-on-missing`; future missing internal call targets should fail regeneration before becoming runtime range-fallback bugs.
- Exclusions are intentionally narrow: `ni118` labels `0x0F0014D8`/`0x0F00150C` make current N64Recomp abort while sizing a local jump table, and `ni120` result labels remain handled by the scoped pair-120 runtime shim.
- `tools/audit_map_hidden_boundaries.py --fail-on-missing` applies the same class of guard to map overlays. It conservatively reports stack-prologue starts that sit inside an already declared function range and immediately follow a `jr ra` epilogue, then regen fails before those hidden map callbacks can range-fallback to the wrong generated function.

NI pair 15 / Art Tower knight (`actor_id=0x20DD`) notes:

| Address | Symbol | Notes |
|---------|--------|-------|
| 0x0F000000 | ni_ovl_015_knight_dispatch_primary | Primary ni015 dispatcher; uses table rooted near `0x0F008928` |
| 0x0F000070 | ni_ovl_015_knight_dispatch_secondary | Secondary ni015 dispatcher |
| 0x0F0000E0 | ni_ovl_015_knight_update_main | Main Art Tower knight update. Chooses enemy-list state `0x03` when far and `0x23` when near, calls the local AI state machine, updates collision/probe data, and checks the `visual+0xB0` high-bit death gate before entering death/list state `0x10` |
| 0x0F002830 | ni_ovl_015_func_0F002830 | Knight state-table handler before the internal `0x0F002A4C` cleanup target |
| 0x0F002A4C | ni_ovl_015_knight_clear_motion_substate | Internal table target used by `0x0F008944[8]` and `0x0F008964[0]`; must be split from `0x0F002830` so indirect dispatch does not range-fallback into the wrong state handler. Splitting this fixed the Art Tower knight no-chase/repeated-animation loop. |
| 0x0F002E74 | ni_ovl_015_knight_swing_anim_a_update | Swing/animation state handler; calls the attack-effect and motion-pulse helpers, then the animation-finish enemy-list helper when the animation event byte matches `0x80` |
| 0x0F0030B4 | ni_ovl_015_knight_swing_anim_b_update | Sibling swing/animation state handler |
| 0x0F0032F4 | ni_ovl_015_knight_swing_anim_c_update | Sibling swing/animation state handler |
| 0x0F003530 | ni_ovl_015_knight_death_update | Death-state update; traced as death-writer path, but it does not run unless the main update/death gate transitions first |
| 0x0F0038A0 | ni_ovl_015_knight_collision_probe_update | Updates the knight collision/probe parameters under `visual+0x164` |
| 0x0F00394C | ni_ovl_015_knight_ai_state_machine_update | Dispatches the local AI/animation state from `visual+0x11C`; adjacent words `visual+0x114/+0x118` are flags and `visual+0x120/+0x124` are previous/substate fields |
| 0x0F003F5C | ni_ovl_015_knight_attack_effect_update | Handles attack effect/sound/visual flags during swing states |
| 0x0F0040C8 | ni_ovl_015_knight_motion_pulse_update | Updates temporary swing/motion pulse while `visual+0x154 & 0x400` |
| 0x0F0044A4 | ni_ovl_015_knight_finish_anim_enemy_state_update | Animation-finish helper; normally calls enemy-list state `0x03`, or `0x02` when `actor->3C->60` bit 25 is set |
| 0x0F004628 | ni_ovl_015_knight_close_action_check | Checks close/player-action condition used by a death/interaction path |
| 0x0F004698 | ni_ovl_015_knight_xz_distance_to_player | Computes XZ distance between the knight and player |

General geometry helper notes:

| Address | Symbol | Notes |
|---------|--------|-------|
| 0x80147D5C | vec3_distance | Computes 3D distance between two xyz vectors passed in `a0`/`a1`; used by Tower map object visibility and other scene/model helpers |

Common interactable helper notes:

| Address | Symbol | Notes |
|---------|--------|-------|
| 0x80186DF0 | Interactable_Entrypoint | State dispatcher for `CUTSCENE_INTERACTABLES` objects |
| 0x80186E60 | Interactable_Init | Initializes pickable item/text spot state, model, event flag, and trigger fields |
| 0x80187564 | Interactable_Main | Idle/update path; advances to interaction handling after object `+0x3C` is set |
| 0x80187C78 | Interactable_InitCheck | Starts item name textbox, White Jewel save prompt, or text spot message after interaction |
| 0x80187E04 | Interactable_SelectTextboxOption | Handles White Jewel yes/no and text spot option actions |
| 0x80188264 | Interactable_StopCheck | Waits for save/textbox completion and clears freeze/interacting state |
| 0x8018842C | Interactable_Destroy | Final item pickup/event-flag destroy path |
| 0x801885A4 | Interactable_stopInteraction | Clears textbox pointer, fade timer, textbox-active flag, and object `+0x3C` |
| 0x80188730 | Player_getActorCurrentlyInteractingWith | Scans object slots for current interactable candidates, including save points and drops using object ID `0x27`; returns the selected actor or deferred textbox/special candidate |
| 0x80188A80 | playerCanInteractWithSpecialTextbox | Special-textbox interaction gate, matching the CV64 shared-engine helper |
| 0x80188B84 | playerCanInteractWithInteractuable | Position, trigger, and facing gate for normal interactables; Tower save/drop traces hook this function to distinguish missing candidates from rejected candidates |
| 0x80188E84 | interactables_getInteractingType | Reads the current interactable type for the selected common interactable |
| 0x80188F78 | interactables_setInteractingFlag | Sets the object interaction flag at object `+0x3C`; trace hook logs when a selected save/drop candidate is actually flagged |
| 0x80188F84 | interactable_createWithSettings | Creates a common interactable from settings data |
| 0x80188FC4 | interactable_setPosition | Copies interactable position data into the object |

Tower of Sorcery map overlay (`map_ovl_29`, ROM `0x007D9790`) notes:

| Address | Symbol | Notes |
|---------|--------|-------|
| 0x802E4E9C | map_ovl_29_func_802E4E9C | Generated after removing the old Tower stub; calls `map_ovl_29_update_distance_flag` during per-object update |
| 0x802E534C | map_ovl_29_func_802E534C | Generated after removing the old Tower stub; animates/positions Tower scene objects and calls `map_ovl_29_update_distance_flag` |
| 0x802E5418 / 0x802E568C / 0x802E5900 | internal labels inside `map_ovl_29_func_802E534C` | These are internal jump-table labels that depend on the containing function's stack/register state; do not split them as standalone functions |
| 0x802E6750..0x802E8738 | map_ovl_29_func_* hidden callback splits | Twenty-one conservative map-overlay boundary splits found by `tools/audit_map_hidden_boundaries.py`; many are referenced by the Tower data table at `0x802E8F20..0x802E8F98`. These prevent indirect map callbacks from range-falling back into the earlier containing functions. |
| 0x802E8A98 | map_ovl_29_update_distance_flag | Compares object position (`obj+0x50`) against the current player/object pointer from `0x801CAC20`, then stores a dynamic draw/object handle at `obj+0x74` when inside the distance gate; widening the original `240.0f` cutoff did not fix the Tower black screen and the experiment was removed |

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

## Graphics node / display-list emission

| Address | Name | Notes |
|---------|------|-------|
| 0x800065DC | gfx_update_and_draw_root | Per-frame graphics root update, then draws the current root node list from `0x800C1650` |
| 0x80006990 | gfx_emit_node_segment_commands | Emits texture/segment display-list commands for a graphics node |
| 0x80007018 | gfx_emit_node_child_lists | Emits child/sub display-list commands for a graphics node |
| 0x800070F0 | gfx_emit_node_recursive | Recursively walks graphics nodes and emits RDP state packets (`E700`, `FA00`, `FB00`, `F900`, `F800`) |
| 0x80008AC8 | gfx_draw_root_node_list | Chooses the sorted/root graphics node draw path before falling back to `gfx_emit_node_recursive` |

### Segment-6 CPU alias notes

- CPU MEM_* accesses to game segment pointers `0x06xxxxxx` land in the recomp's KSEG0 mirror at `rdram+0x86000000+offset`.
- The first Tower fix mirrored active NI overlay data into the low segment-6 CPU alias, but later death/reload/attack crashes reached `0x0603D70C` and guest `0x4603DFBC`, outside the active NI overlay span.
- `LOD_FIX_SEG6_CPU_ALIAS_GUARD` maps zero-filled 16MB CPU aliases for direct `0x06xxxxxx` and kuseg/TLB `0x46xxxxxx` segment-6 forms so stale or not-yet-populated graphics-node links do not hard-crash. Normal `osMapTLB` segment copies and the NI overlay mirror still overwrite that guard with real data when mappings are known.
