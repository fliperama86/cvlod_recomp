/**
 * lod_symbols.h — Named constants for Castlevania: Legacy of Darkness recomp
 *
 * Derived from CV64 decomp (cv64_decomp) and LoD-specific reverse engineering.
 * Auto-included in RecompiledFuncs by tools/apply_symbols.py.
 *
 * Convention: prefix indicates category
 *   OBJ_   = Object struct offsets
 *   SYS_   = system_work (sys) struct offsets
 *   SCENE_ = Gamestate/scene IDs
 *   TMPL_  = Object template IDs (ObjectID enum)
 *   NI_    = Nisitenma-Ichigo file/system constants
 *   SCHED_ = OSSched struct offsets
 *   GSS_   = Gamestate slot flags
 *   EXEC_  = Execution flags (sys.execution_flags bits)
 */

#ifndef LOD_SYMBOLS_H
#define LOD_SYMBOLS_H

/* ===================================================================
 * Object struct (0x74 bytes) — from CV64 include/game/object.h
 * =================================================================== */

/* ObjectHeader (0x00-0x1F) */
#define OBJ_ID                  0x00   /* s16: object type ID */
#define OBJ_FLAGS               0x02   /* u16: exec flags */
#define OBJ_TIMER               0x04   /* u16: misc timer */
#define OBJ_FIELD_06            0x06   /* s16 */
#define OBJ_FUNCINFO            0x08   /* ObjectFuncInfo[3] (6 bytes) */
#define OBJ_STATE               0x09   /* u8: state byte (funcinfo[0].function) */
#define OBJ_FUNCINFO_ID         0x0E   /* s16: current function level */
#define OBJ_DESTROY             0x10   /* ObjectDestroyFunc* */
#define OBJ_PARENT              0x14   /* ObjectHeader* */
#define OBJ_NEXT                0x18   /* ObjectHeader* */
#define OBJ_CHILD               0x1C   /* ObjectHeader* */

/* Object body (0x20-0x73) */
#define OBJ_ALLOC_ENTRIES       0x20   /* u16: bitmask of allocated alloc_data */
#define OBJ_GFX_ENTRIES         0x22   /* u16: bitmask of graphic containers */
#define OBJ_FIGURES             0x24   /* Figure*[4] (0x24-0x33) */
#define OBJ_FIGURES_0           0x24
#define OBJ_FIGURES_1           0x28
#define OBJ_FIGURES_2           0x2C
#define OBJ_FIGURES_3           0x30
#define OBJ_ALLOC_DATA          0x34   /* void*[16] (0x34-0x73) */
#define OBJ_ALLOC_DATA_N(n)     (0x34 + (n) * 4)
#define OBJ_ALLOC_DATA_15       0x70

/* Object flags (header.flags) */
#define OBJ_FLAG_DONT_DESTROY   0x0010
#define OBJ_FLAG_PAUSE          0x4000
#define OBJ_FLAG_TOP            0x8000

/* ===================================================================
 * Scene / Gamestate IDs — from CV64 include/game/gamestate.h
 * LoD-specific: scene 4 (save game) is the FIRST scene on boot
 * =================================================================== */
#define SCENE_KONAMI_LOGO       1
#define SCENE_GAMEPLAY          2
#define SCENE_GAME_OVER         3
#define SCENE_SAVE_GAME         4   /* a.k.a. STAGE_SELECT in CV64 */
#define SCENE_INTRO_CUTSCENE    5
#define SCENE_TITLE_SCREEN      6
#define SCENE_DATA_MENU         7
#define SCENE_OPTIONS_MENU      8
#define SCENE_TITLE_DEMO        9
#define SCENE_GAMENOTE_DELETE   10
#define SCENE_CREDITS           11

/* Gamestate config struct offsets (80 bytes per entry, base = scene_table) */
#define GSS_CFG_INIT_FUNC       (-0x48)   /* void* init function (s2-0x48) */
#define GSS_CFG_SCENE_HANDLER   (-0x44)   /* void* per-frame handler (s2-0x44) */
#define GSS_CFG_SLOTS           (-0x40)   /* s32[16] GSS slot entries (s2-0x40) */

/* GSS slot flags (upper byte of each slot entry) */
#define GSS_FLAG_OBJECT         0x10   /* create object from template ID */
#define GSS_FLAG_FUNCTION       0x20   /* call function pointer each frame */
#define GSS_FLAG_OVERLAY        0x07   /* load overlay + create object */
#define GSS_FLAG_ROM_PTR        0x90   /* ROM pointer (needs DMA resolution) */

/* ===================================================================
 * Object template IDs — from CV64 include/game/object_ID.h
 * =================================================================== */
#define TMPL_GAMESTATEMGR       0x001
#define TMPL_OBJECT_002         0x002
#define TMPL_NI_DISPATCHER      0x003   /* NI system dispatcher (func_800113EC) */
#define TMPL_DMAMGR             0x004   /* DMA manager */
#define TMPL_GAMEPLAYMGR        0x005
#define TMPL_MAP_OBJECT_PARENT  0x006
#define TMPL_PLAYER_CONTROLLER  0x007
#define TMPL_MAP_SETUP          0x008
#define TMPL_ENEMY_PARENT       0x009
#define TMPL_OVERLAY_SYSTEM     0x1AB   /* NI overlay system (created by overlay_system_create) */

/* ===================================================================
 * system_work struct offsets — from CV64 include/game/system_work.h
 * Base address: 0x801C82C0 (LoD), 0x80363AB8 (CV64)
 * Access pattern: lui $reg, 0x801D; addiu $reg, -0x7D40; lw $reg, OFFSET($reg)
 * =================================================================== */
#define SYS_BASE                0x801C82C0

/* Display / frame management */
#define SYS_GLOBAL_TIMER        0x00     /* u32: uncapped frame counter */
#define SYS_EXEC_FLAGS          0x04     /* u32: execution flags */
#define SYS_CUR_GFX_BUF        0x24     /* s16: current graphic buffer index (LoD-specific) */

/* NI file system */
#define SYS_NI_LOADED_FILES     0x1C0    /* u32[64]: loaded NI file bitmask (CV64: 0x241BC) */
#define SYS_NI_DECOMP_FILES     0x2C0    /* u32[64]: currently decompressing bitmask */
#define SYS_FILE_LOAD_ID        0x3BC    /* u8: file load array index */
#define SYS_FILE_PTR_ARRAY      0x570    /* void*[255]: NI loaded files pointers */
/* NI descriptor fields (accessed via ni_desc pointer) */
#define NI_DESC_TYPE            0x00     /* u32: NI descriptor type */
#define NI_DESC_FRAME_CTR       0x08     /* s16: frame/animation counter (3000 initial) */
#define NI_DESC_SUBFIG_BITMASK  0x4C     /* u32: sub-figure active bitmask */
#define NI_DESC_SUBFIG_CHAIN    0x64     /* linked descriptor chain for sub-figures */

/* Scene / map system */
#define SYS_SCENE_ID            0x80     /* s16: current scene ID (LoD-specific) */

/* Overlay / NI system */
#define SYS_NI_SYSTEM_PTR       0x295C   /* void*: NI system object pointer */
#define SYS_EXEC_FLAGS_2        0x2908   /* u32: secondary execution flags */
#define SYS_NI_LOAD_COUNT       0x2B24   /* u32: NI load counter (guards overlay_system_create) */

/* lui 0x801D + addiu form: these are the signed 16-bit immediates for common offsets */
#define SYS_IMM_NI_SYSTEM_PTR   (-0x521C)   /* MEM_W(lui_801D, this) = sys+0x295C */
#define SYS_IMM_NI_LOAD_COUNT   (-0x521C)   /* Actually 0x2B24 from sys_base - verify */

/* ===================================================================
 * OSSched struct offsets — from CV64 include/ultra64/PR/sched.h
 * Base: 0x800C5E80 (LoD)
 * =================================================================== */
#define SCHED_BASE              0x800C5E80

#define SCHED_RETRACE_COUNT     0x000   /* s16 */
#define SCHED_NUM_FIELDS        0x002   /* s16 */
#define SCHED_CMD_Q             0x004   /* OSMesgQueue */
#define SCHED_GRAPH_Q           0x03C   /* OSMesgQueue: GRAPH thread input */
#define SCHED_AUDIO_Q           0x074   /* OSMesgQueue */
#define SCHED_INTERRUPT_Q       0x0AC   /* OSMesgQueue: main event queue */
#define SCHED_SP_DONE_Q         0x0E4   /* OSMesgQueue */
#define SCHED_DP_DONE_Q         0x154   /* OSMesgQueue */
#define SCHED_CLIENT_LIST       0x888   /* OSScClient* */
#define SCHED_CUR_RSP_TASK      0x88C   /* OSScTask*: currently running GFX task */
#define SCHED_CUR_TASK          0x890   /* OSScTask* */
#define SCHED_DP_NOTIFY         0x894   /* u32 */
#define SCHED_SWAP_COUNT        0x898   /* u32 */
#define SCHED_SWAP_COUNT_2      0x89C   /* u32 */

/* Scheduler event message values */
#define SCHED_MSG_VI_RETRACE    0x29A
#define SCHED_MSG_SP_DONE       0x29B
#define SCHED_MSG_DP_DONE       0x29C
#define SCHED_MSG_PRENMI        0x29D

/* ===================================================================
 * Key global addresses
 * =================================================================== */
#define ADDR_ROOT_OBJECT_PTR    0x800C1520   /* Object**: pointer to root scene object */
#define ADDR_BOOT_THREAD        0x800C2680   /* OSThread: boot thread (id=1) */
#define ADDR_IDLE_THREAD        0x800C3830   /* OSThread: idle thread (id=5) */
#define ADDR_MAIN_MSG_Q         0x800C5D38   /* OSMesgQueue: main message queue */
#define ADDR_GUARD_800B0088     0x800B0088   /* u16: GFX loop guard (set by PRENMI handler) */

/* ===================================================================
 * Execution flag bits (sys+0x04 / SYS_EXEC_FLAGS)
 * =================================================================== */
#define EXEC_FLAG_BG_ACTIVE     0x10000000   /* Background state active */

/* ===================================================================
 * Atari work / pool system
 * =================================================================== */
#define POOL_TABLE_BASE         0x800F90E8   /* atari_work_table base (LoD) */
#define POOL_TABLE_END          0x800F42E8   /* atari_work_table end sentinel */
#define POOL_ENTRY_SIZE         0x9C         /* size of each pool entry */

#endif /* LOD_SYMBOLS_H */
