# Controller Pak Save Implementation Approaches

## Context

Castlevania: Legacy of Darkness uses N64 Controller Pak (Memory Pak) for saves.
The game has its own compiled copies of the N64 SDK osPfs functions that read/write
the pak's inode table and directory directly via PIF SI DMA. The N64ModernRuntime
does NOT have built-in Controller Pak support — upstream returns PFS_ERR_NOPACK
for all osPfs functions.

## Option A: BM64 Approach (Function-Level Redirect)

**Used by:** BM64Recomp (Bomberman 64) — the only other N64 recomp project with Controller Pak saves.

**Strategy:** Never let the game's compiled osPfs SDK code run. Intercept at the function
call level and redirect to our implementations.

### Steps

1. **Stub all low-level PIF/SI pak functions** as no-ops:
   - `__osContRamRead_recomp` — PIF-level 32-byte pak read
   - `__osContRamWrite_recomp` — PIF-level 32-byte pak write
   - `__osPfsSelectBank_recomp` — bank selection (no-op for 32KB)
   - `__osPfsRWInode_recomp` — raw inode read/write
   - `__osContAddressCrc_recomp` — pak address CRC computation

2. **Override all game osPfs functions** to redirect to our implementations:
   - Either via `[[patches.instruction]]` in the TOML config (BM64's approach)
   - Or by defining strong-symbol overrides in C++ (our current approach, which works
     since the recompiled functions are `__attribute__((weak))`)

3. **Our osPfs implementations** (in pak.cpp) use ANY consistent format for the save buffer.
   Since the game never reads the raw pak data (all low-level reads are stubbed), the format
   doesn't need to match the N64 PFS layout. A simple custom format works fine.

4. **PIF handler** still handles button reads (cmd=0x01) and controller status (cmd=0x00),
   but pak read/write commands (cmd=0x02/0x03) become irrelevant since the game's osPfs
   code never reaches the PIF level.

### Pros
- Simpler — no need to implement N64 PFS inode format
- No format conflicts — single source of truth for pak data
- Proven approach (BM64Recomp uses it)
- Faster to implement

### Cons
- Must identify and override EVERY game osPfs function (including internal helpers)
- Less faithful to original N64 behavior
- If we miss a function, it silently fails

### Key game functions to override

| Game Address | SDK Function | Our Implementation |
|---|---|---|
| `func_800A4040` | osPfsInitPak | osPfsInitPak_recomp |
| `func_800A3E60` | osPfsChecker | osPfsChecker_recomp |
| `func_8009F400` | osPfsFindFile | osPfsFindFile_recomp |
| `func_800968B0` | osPfsFreeBlocks | osPfsFreeBlocks_recomp |
| `func_800A3B40` | osPfsFreeBlocks (alt) | osPfsFreeBlocks_recomp |
| `func_800A09A0` | osPfsReadWriteFile | osPfsReadWriteFile_recomp |
| `func_800A4D00` | __osPfsSelectBank | no-op |
| `func_8009AE60` | osPfsNumFiles | osPfsNumFiles_recomp |
| `func_80099D84` | osPfsAllocateFile | osPfsAllocateFile_recomp |
| `func_8009A100` | __osPfsRequestData (pak type detect) | return success |
| `func_800A4248` | __osPfsGetInitData (pak setup) | return success |
| `func_800A5130` | __osPfsGetLabel (read ID area) | return success |

Low-level stubs (all no-ops):
| Function | Purpose |
|---|---|
| `__osContRamRead_recomp` | PIF pak read |
| `__osContRamWrite_recomp` | PIF pak write |
| `__osPfsSelectBank_recomp` | Bank select |
| `__osPfsRWInode_recomp` | Raw inode I/O |
| `__osContAddressCrc_recomp` | Address CRC |
| `__osPfsCheckRamArea_recomp` | RAM test |


## Option B: Faithful N64 PFS Approach

**Strategy:** Make both PIF-level reads and high-level osPfs functions operate on the same
real N64 PFS format in the save buffer.

### Steps

1. **Rewrite pak.cpp** to use proper N64 PFS inode/directory structures:
   - ID area at page 0 (offset 0x000-0x0FF)
   - Inode table at pages 1-2 (offset 0x100-0x2FF)
   - Directory at pages 3-4 (offset 0x300-0x4FF)
   - Data at pages 5-127 (offset 0x500-0x7FFF)

2. **osPfs functions** read/write inodes and directory entries in the save buffer directly.
   osPfsAllocateFile allocates pages by updating the inode chain. osPfsFindFile scans
   the 16-entry directory.

3. **PIF handler** continues to serve pak read/write (cmd=0x02/0x03) from the same save buffer.
   Since our osPfs functions modify the same data, PIF reads see consistent state.

4. **No game function overrides needed** (in theory) — the game's own compiled osPfs code
   reads/writes through PIF, which hits our PIF handler, which reads/writes the save buffer
   that our osPfs functions also manage.

### Pros
- Most faithful to original N64 behavior
- No need to find/override every game function
- Works even if the game has unusual osPfs call patterns

### Cons
- Must implement full N64 PFS inode chain management (allocate, free, traverse)
- Must compute correct checksums for ID area and inode table
- Must handle byte ordering correctly through PIF → save buffer → osPfs function chain
- More complex, more room for bugs
- Still need to handle the PIF byte ordering carefully


## Decision

**Selected: Option A (BM64 approach)** — faster, proven, and sufficient for our needs.
