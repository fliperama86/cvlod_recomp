// LoD-specific initialization callback.
// Called by N64ModernRuntime via GameEntry::on_init_callback after ROM is loaded.
// Handles: NI file decompression, overlay data copying, KSEK0 mirror, game-specific fixes.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sys/mman.h>
#include <sys/stat.h>
#include <zlib.h>

#include "recomp.h"
#include "librecomp/game.hpp"
#include "librecomp/overlays.hpp"

extern "C" void load_overlay_by_id(uint32_t id, uint32_t ram_addr);

// Decompressed NI file address table (used by rt64_render_context.cpp for segment 6 resolution)
uint32_t ni_decompressed_addrs[1024] = {};
int ni_decompressed_count = 0;

// Set rdram pointer for debug scanning in get_function()
extern uint8_t* rdram_ptr_for_debug;

// Cached overlay_system byte-swapped data for restoration after DMA overwrites
static std::vector<uint8_t> overlay_system_cache;
static constexpr uint32_t OVL_SYS_RDRAM_DST = 0x801CAEA0 - 0x80000000;
static constexpr uint32_t OVL_SYS_FULL_SIZE = 0x7AE0 + 0xD860 + 0x9430; // 0x21770

extern "C" void lod_restore_overlay_system_data(uint8_t* rdram) {
    if (overlay_system_cache.empty()) return;
    memcpy(rdram + OVL_SYS_RDRAM_DST, overlay_system_cache.data(), overlay_system_cache.size());
}

// Copy a regular overlay's full ROM data (code+data) to RDRAM with byte swapping.
// Called from ignored_func_stubs.cpp when map overlays need loading.
extern "C" void lod_copy_overlay_data(uint8_t* rdram, uint32_t rom_offset,
                                       uint32_t rdram_dst, uint32_t size) {
    std::span<const uint8_t> rom = recomp::get_rom();
    if (rom_offset + size > rom.size()) {
        fprintf(stderr, "[lod_copy_overlay_data] ERROR: ROM offset 0x%X + size 0x%X > ROM size 0x%zX\n",
                rom_offset, size, rom.size());
        return;
    }
    // Byte-swap copy: reverse each 4-byte word (BE ROM → LE RDRAM)
    for (uint32_t i = 0; i < size; i += 4) {
        rdram[rdram_dst + i + 0] = rom[rom_offset + i + 3];
        rdram[rdram_dst + i + 1] = rom[rom_offset + i + 2];
        rdram[rdram_dst + i + 2] = rom[rom_offset + i + 1];
        rdram[rdram_dst + i + 3] = rom[rom_offset + i + 0];
    }
}

void lod_on_init(uint8_t* rdram, recomp_context* ctx) {
    rdram_ptr_for_debug = rdram;

    // Load the ORIGINAL compressed ROM for NI decompression. The decompressed ROM
    // has different NI table format (shared offsets, no size info). The compressed
    // ROM has proper zlib-compressed entries with deterministic sizes.
    std::vector<uint8_t> rom_storage;
    std::span<const uint8_t> rom;
    {
        std::filesystem::path orig_path = "resources/castlevania_legacy_of_darkness.z64";
        if (std::filesystem::exists(orig_path)) {
            std::ifstream f(orig_path, std::ios::binary);
            rom_storage.assign(std::istreambuf_iterator<char>(f), {});
            rom = rom_storage;
            fprintf(stderr, "[lod_init] rdram=%p, original ROM: %zu bytes\n", (void*)rdram, rom.size());
        } else {
            rom = recomp::get_rom();
            fprintf(stderr, "[lod_init] Using runtime ROM: %zu bytes\n", rom.size());
        }
    }
    if (rom.empty()) {
        fprintf(stderr, "[lod_init] ERROR: no ROM data\n");
        return;
    }

    // === Overlay loading ===
    // Upstream init() loads sections by computing RAM linearly from ROM offsets.
    // For LoD, section 2 has a relocation gap (linear=0x800C1520, effective=0x80141870).
    // The upstream's linear computation puts section 2 functions at the wrong address.
    //
    // Fix: first UNLOAD section 2 at the wrong address, then reload all sections
    // at their correct effective addresses.
    unload_overlays(0x800C1520, 0x4AEE0);  // Remove section 2 at wrong linear addr

    // Load sections at their correct effective addresses (from recomp_overlays.inl):
    // Section 2: common (ROM 0xC2120 → RAM 0x80141870)
    load_overlays(0xC2120, 0x80141870, 0x4AEE0);
    // Section 3: overlay_system (ROM 0x745230 → RAM 0x801CAEA0)
    load_overlays(0x745230, 0x801CAEA0, 0x69E0);
    // Map overlays at RAM 0x802E3B70 (shared VRAM, mutually exclusive).
    // Register map_ovl_00 functions as default (used by gs=1 Konami logo and others).
    // Copy map_ovl_34 DATA for gs=4 save screen (jump tables, constants).
    // Function registration stays as map_ovl_00 — safe for gs=1 after skip.
    // The func_80012ED0 override handles proper overlay swaps when the game's
    // own overlay loader fires during gamestate transitions.
    load_overlays(0x76CD00, 0x802E3B70, 0x779510 - 0x76CD00);
    // Copy map_ovl_34 data to RDRAM (for save screen state machine tables)
    lod_copy_overlay_data(rdram, 0x7EF5E0, 0x802E3B70 - 0x80000000, 0x16690);
    fprintf(stderr, "[init] Registered map_ovl_00 functions, copied map_ovl_34 data to 0x802E3B70\n");

    // === Copy main code DATA section (byte-swapped) ===
    // The main code DATA section contains critical tables (section descriptors,
    // object dispatch tables, gamestate tables) that the game reads during boot.
    // On real N64, the IPL copies the first ~1MB of ROM to RDRAM, which includes
    // this data. In the recomp, the IPL doesn't run, so we must copy it here.
    // Without this, the game reads zeroes for section descriptors and DMAs
    // overlay data from wrong ROM offsets, corrupting the overlay_system.
    {
        constexpr uint32_t main_data_rom = 0xAB9F0;
        constexpr uint32_t main_data_size = 0xC2120 - 0xAB9F0; // 91952 bytes
        constexpr uint32_t rdram_dst = 0x800AADF0 - 0x80000000; // 0xAADF0
        for (uint32_t i = 0; i < main_data_size; i += 4) {
            if (main_data_rom + i + 3 < rom.size()) {
                rdram[rdram_dst + i + 0] = rom[main_data_rom + i + 3];
                rdram[rdram_dst + i + 1] = rom[main_data_rom + i + 2];
                rdram[rdram_dst + i + 2] = rom[main_data_rom + i + 1];
                rdram[rdram_dst + i + 3] = rom[main_data_rom + i + 0];
            }
        }
        fprintf(stderr, "[init] Copied main data section (byte-swapped): ROM 0x%X → rdram+0x%X (%u bytes)\n",
                main_data_rom, rdram_dst, main_data_size);
    }

    // Section 2 (common) DATA section is loaded by the game's own DMA
    // (rom=0x100C2120, size=0x5B8E0 → covers TEXT+DATA).
    // The game's DMA uses do_rom_read with MEM_B (byte-swapped), producing
    // correctly aligned DATA at the game's expected addresses.
    // No pre-copy needed — the game handles it.

    // === Full N64 address space mapping ===
    // The N64 has a 32-bit address space split into segments:
    //   KSEG0 (0x80-0x9F): cached,   physical = addr - 0x80000000 → rdram+0x80000000
    //   KSEG1 (0xA0-0xBF): uncached, physical = addr - 0xA0000000 → rdram+0x20000000
    //   KSEG2 (0xC0-0xFF): TLB-mapped                              → rdram+0x40000000
    //
    // Map the entire range from rdram+0x20000000 to rdram+0xA0000000 (2GB) as R/W.
    // This covers all N64 address segments and avoids SIGBUS on any game access
    // including RDRAM probes, hardware register reads, and TLB-mapped accesses.
    {
        constexpr size_t map_size = 0x20000000ULL; // 512MB for KSEG1 physical range
        int ret = mprotect(rdram + 0x20000000, map_size, PROT_READ | PROT_WRITE);
        fprintf(stderr, "[mprotect] KSEG1 physical range: rdram+0x20000000 (512 MB) %s\n",
                ret == 0 ? "OK" : "FAILED");
        if (ret == 0) {
            memset(rdram + 0x20000000, 0, map_size);
            fprintf(stderr, "[mprotect] KSEG1 region zeroed\n");
        }
        // Set osMemSize at the standard N64 location (0x80000318).
        // 8MB = expansion pak present. Prevents RDRAM size probe crashes.
        *(uint32_t*)(rdram + 0x318) = 0x00800000;
        fprintf(stderr, "[init] Set osMemSize (0x80000318) = 0x00800000 (8MB)\n");
    }

    // === Null-pointer guard regions ===
    // Game code dereferences null/garbage pointers with struct offsets.
    // On real N64, these read 0 from RDRAM. In the recomp, the MEM_W macro
    // maps them to various rdram offsets that may be PROT_NONE → SIGBUS.
    //
    // Key patterns:
    //   MEM_W(+offset, 0)    → rdram + 0x80000000 + offset (KSEG0 mirror)
    //   MEM_W(-offset, 0)    → rdram + 0x7FFFFFFF - offset (below KSEG0)
    //   MEM_W(+offset, 0x40000000) → rdram + 0x40000000 + offset (KSEG2)
    //
    // Map the gap between KSEG1 end and KSEG0 start (rdram+0x40000000 to +0x80000000)
    {
        int ret = mprotect(rdram + 0x40000000, 0x40000000, PROT_READ | PROT_WRITE);
        fprintf(stderr, "[mprotect] KSEG2/null guard: rdram+0x40000000 (1 GB) %s\n",
                ret == 0 ? "OK" : "FAILED");
        if (ret == 0) {
            // Zero only the edges where null-pointer accesses land (not all 1GB)
            memset(rdram + 0x40000000, 0, 0x100000);  // first 1MB
            memset(rdram + 0x7FF00000, 0, 0x100000);  // last 1MB before KSEG0
        }
    }

    // === ROM KSEK0 region ===
    // Map ROM at rdram+0x90000000 for KSEK0 ROM access (0x9XXXXXXX).
    {
        size_t rom_size = std::min(rom.size(), (size_t)(64 * 1024 * 1024));
        int ret = mprotect(rdram + 0x90000000, rom_size, PROT_READ | PROT_WRITE);
        fprintf(stderr, "[mprotect] ROM KSEK0 region: rdram+0x90000000, size=0x%zX %s\n",
                rom_size, ret == 0 ? "OK" : "FAILED");
        if (ret == 0) {
            memcpy(rdram + 0x90000000, rdram + 0x10000000, rom_size);
            fprintf(stderr, "[init] Mirrored ROM to KSEK0 region at rdram+0x90000000\n");
        }
    }

    // === Copy full overlay_system section (TEXT+DATA+RODATA, byte-swapped) ===
    // The game's section table defines the overlay_system as:
    //   TEXT:   ROM 0x745230, size 0x7AE0 (note: larger than recompiler's 0x69E0)
    //   DATA:   ROM 0x74CD10, size 0xD860
    //   RODATA: ROM 0x75A570, size 0x9430
    // Total in RAM: 0x7AE0+0xD860+0x9430 = 0x21770 bytes at 0x801CAEA0-0x801ED010
    //
    // The recompiler only identified 0x69E0 bytes of TEXT. The extra 0x1100 bytes
    // (offset 0x69E0-0x7AE0) contain jump tables and data-in-text that the game
    // accesses via computed addresses (e.g., function pointer table at 0x801D1880).
    //
    // We must copy the FULL section because:
    // 1. The game's overlay loader may DMA different sub-sections at runtime
    // 2. The jump tables in TEXT tail are critical for overlay dispatch
    // 3. DATA+RODATA contain initialized globals used by overlay_system code
    {
        constexpr uint32_t overlay_system_rom = 0x745230;
        constexpr uint32_t overlay_system_full_size = 0x7AE0 + 0xD860 + 0x9430; // 0x21770
        constexpr uint32_t rdram_dst = 0x801CAEA0 - 0x80000000; // 0x1CAEA0
        for (uint32_t i = 0; i < overlay_system_full_size; i += 4) {
            if (overlay_system_rom + i + 3 < rom.size()) {
                rdram[rdram_dst + i + 0] = rom[overlay_system_rom + i + 3];
                rdram[rdram_dst + i + 1] = rom[overlay_system_rom + i + 2];
                rdram[rdram_dst + i + 2] = rom[overlay_system_rom + i + 1];
                rdram[rdram_dst + i + 3] = rom[overlay_system_rom + i + 0];
            }
        }
        fprintf(stderr, "[init] Copied overlay_system full section (byte-swapped): ROM 0x%X → rdram+0x%X (0x%X bytes)\n",
                overlay_system_rom, rdram_dst, overlay_system_full_size);
        // Cache for restoration after DMA overwrites
        overlay_system_cache.assign(rdram + rdram_dst, rdram + rdram_dst + overlay_system_full_size);
        fprintf(stderr, "[init] Cached overlay_system data (%zu bytes) for DMA restoration\n",
                overlay_system_cache.size());
    }

    // === Copy section 4 data (byte-swapped) ===
    {
        constexpr uint32_t sec4_rom  = 0x76CD00;
        constexpr uint32_t sec4_size = 0x779510 - 0x76CD00;
        constexpr uint32_t rdram_dst = 0x802E3B70 - 0x80000000;
        for (uint32_t i = 0; i < sec4_size; i += 4) {
            if (sec4_rom + i + 3 < rom.size()) {
                rdram[rdram_dst + i + 0] = rom[sec4_rom + i + 3];
                rdram[rdram_dst + i + 1] = rom[sec4_rom + i + 2];
                rdram[rdram_dst + i + 2] = rom[sec4_rom + i + 1];
                rdram[rdram_dst + i + 3] = rom[sec4_rom + i + 0];
            }
        }
        fprintf(stderr, "[init] Copied section 4 data (byte-swapped): ROM 0x%X → rdram+0x%X (%u bytes)\n",
                sec4_rom, rdram_dst, sec4_size);
    }

    // === NI (Nisitenma-Ichigo) file decompression ===
    {
        // mprotect NI data region
        constexpr uint32_t ni_rdram_base = 0x14000000;
        constexpr uint32_t ni_rdram_size = 64 * 1024 * 1024;
        int ret = mprotect(rdram + ni_rdram_base, ni_rdram_size, PROT_READ | PROT_WRITE);
        fprintf(stderr, "[init] mprotect NI data: rdram+0x%08X, %d MB%s\n",
                ni_rdram_base, ni_rdram_size / (1024*1024), ret == 0 ? "" : " FAILED");

        // Find NI table in ROM (byte-swapped "Nisitenma-Ichigo" magic)
        constexpr uint32_t ni_table_rom = 0xB1B88;
        constexpr uint32_t ni_entries_rom = ni_table_rom + 0x10; // after 16-byte magic
        constexpr uint32_t file_ptr_array = 0x801C8830;
        constexpr uint32_t bitmask_array  = 0x801C83EC;

        uint32_t ni_rdram_cursor = ni_rdram_base;
        uint32_t ni_rdram_limit = ni_rdram_base + ni_rdram_size;
        int file_count = 0, decomp_count = 0;
        uint32_t pos = ni_entries_rom;

        while (pos + 3 < rom.size()) {
            // NI entry: full 32-bit big-endian offset (NOT flag+24bit)
            uint32_t rom_offset = ((uint32_t)rom[pos] << 24) |
                                   ((uint32_t)rom[pos+1] << 16) |
                                   ((uint32_t)rom[pos+2] << 8) |
                                   ((uint32_t)rom[pos+3]);

            if (rom_offset == 0) break; // end of table

            int entry_idx = file_count;

            // Skip 4-byte header at the file data, then zlib decompress
            if (rom_offset + 4 < rom.size()) {
                const uint8_t* src = rom.data() + rom_offset + 4; // skip header
                size_t src_size = rom.size() - rom_offset - 4;

                std::vector<uint8_t> temp(512 * 1024);
                size_t decomp_size = 0;
                bool ok = false;

                z_stream strm = {};
                strm.next_in = (Bytef*)src;
                strm.avail_in = (uInt)std::min(src_size, (size_t)(512 * 1024));
                strm.next_out = (Bytef*)temp.data();
                strm.avail_out = (uInt)temp.size();
                if (inflateInit(&strm) == Z_OK) {
                    int zret = inflate(&strm, Z_FINISH);
                    decomp_size = strm.total_out;
                    inflateEnd(&strm);
                    ok = (zret == Z_STREAM_END || zret == Z_OK) && decomp_size > 0;
                }

                if (ok) {
                        uint32_t aligned_size = (decomp_size + 7) & ~7u;
                        if (ni_rdram_cursor + aligned_size <= ni_rdram_limit) {
                            // Byte-swap and copy to RDRAM.
                            // The temp buffer contains data starting from a ROM offset
                            // that may NOT be 4-byte aligned. The byte-swap operates
                            // on the TEMP buffer positions (always aligned from 0).
                            for (uint32_t j = 0; j < aligned_size; j += 4) {
                                uint8_t b0 = (j + 0 < decomp_size) ? temp[j + 0] : 0;
                                uint8_t b1 = (j + 1 < decomp_size) ? temp[j + 1] : 0;
                                uint8_t b2 = (j + 2 < decomp_size) ? temp[j + 2] : 0;
                                uint8_t b3 = (j + 3 < decomp_size) ? temp[j + 3] : 0;
                                // Big-endian to little-endian: reverse byte order
                                rdram[ni_rdram_cursor + j + 0] = b3;
                                rdram[ni_rdram_cursor + j + 1] = b2;
                                rdram[ni_rdram_cursor + j + 2] = b1;
                                rdram[ni_rdram_cursor + j + 3] = b0;
                            }
                            uint32_t ext_addr = 0x80000000 | ni_rdram_cursor;
                            ni_decompressed_addrs[entry_idx] = ext_addr;
                            ni_rdram_cursor += aligned_size;
                            decomp_count++;
                        }
                }
            }
            file_count++;
            pos += 4;
        }

        ni_decompressed_count = file_count;
        fprintf(stderr, "[init] NI: %d entries, %d decompressed (%.1f MB at 0x%06X-0x%06X)\n",
                file_count, decomp_count,
                (ni_rdram_cursor - ni_rdram_base) / (1024.0 * 1024.0),
                ni_rdram_base, ni_rdram_cursor);
        fprintf(stderr, "[init]   File ptr array at 0x%08X, bitmask at 0x%08X\n",
                file_ptr_array, bitmask_array);

        // DO NOT pre-populate file_ptr_array at init — it breaks game boot.
        // The game's DMAMgr loads NI files on demand during execution.
        fprintf(stderr, "[init] NI file_ptr_array NOT pre-populated (game loads on demand)\n");
    }



    // === KSEK0 RDRAM mirror ===
    // Map 8MB at rdram+0x80000000 so KSEK0 addresses (0x80XXXXXX) resolve.
    {
        int ret = mprotect(rdram + 0x80000000, 8 * 1024 * 1024, PROT_READ | PROT_WRITE);
        fprintf(stderr, "[mprotect] KSEK0 RDRAM mirror: rdram+0x80000000 (8 MB) %s\n",
                ret == 0 ? "OK" : "FAILED");
        // Set osMemSize so game knows RDRAM size without probing.
        *(uint32_t*)(rdram + 0x318) = 0x00800000; // 8MB
        fprintf(stderr, "[init] Set osMemSize (0x80000318) = 0x00800000 (8MB)\n");
    }

    // === ROM byte-swap mapping ===
    // Map the original 16MB ROM (byte-swapped) to rdram+0x10000000.
    {
        constexpr uint32_t rom_rdram_base = 0x10000000;
        size_t map_size = std::min(rom.size(), (size_t)(16 * 1024 * 1024));
        for (size_t i = 0; i < map_size; i += 4) {
            rdram[rom_rdram_base + i + 0] = rom[i + 3];
            rdram[rom_rdram_base + i + 1] = rom[i + 2];
            rdram[rom_rdram_base + i + 2] = rom[i + 1];
            rdram[rom_rdram_base + i + 3] = rom[i + 0];
        }
        fprintf(stderr, "[init] Mapped %zu bytes of ROM (byte-swapped) at rdram+0x%08X\n",
                map_size, rom_rdram_base);
    }

    // === Extended ROM mapping (NI overlay data) ===
    // The extended ROM contains NI overlay text+data at offsets 0x01000000+.
    // Map this region (byte-swapped) so overlay code can read its .data section
    // via MEM_W when the overlay loader copies it to the segment region.
    {
        // Load the extended ROM's NI overlay data (text+data at offsets 0x1000000+).
        // This is separate from the runtime ROM (which may be the 52MB decompressed version).
        constexpr uint32_t rom_rdram_base = 0x10000000;
        std::filesystem::path ext_rom_path = "resources/castlevania2_ni_extended.z64";
        if (std::filesystem::exists(ext_rom_path)) {
            std::ifstream ef(ext_rom_path, std::ios::binary);
            ef.seekg(0, std::ios::end);
            size_t ext_rom_size = ef.tellg();
            if (ext_rom_size > 0x01000000) {
                size_t ext_start = 0x01000000;
                size_t ext_size = ext_rom_size - ext_start;
                ext_size &= ~3ULL;
                std::vector<uint8_t> ext_data(ext_size);
                ef.seekg(ext_start);
                ef.read(reinterpret_cast<char*>(ext_data.data()), ext_size);

                // mprotect the target region
                uintptr_t page_mask = sysconf(_SC_PAGESIZE) - 1;
                uint8_t* ext_base = rdram + rom_rdram_base + ext_start;
                uint8_t* aligned = (uint8_t*)((uintptr_t)ext_base & ~page_mask);
                size_t aligned_size = ((ext_base + ext_size) - aligned + page_mask) & ~page_mask;
                mprotect(aligned, aligned_size, PROT_READ | PROT_WRITE);

                // Byte-swap and copy
                for (size_t i = 0; i < ext_size; i += 4) {
                    rdram[rom_rdram_base + ext_start + i + 0] = ext_data[i + 3];
                    rdram[rom_rdram_base + ext_start + i + 1] = ext_data[i + 2];
                    rdram[rom_rdram_base + ext_start + i + 2] = ext_data[i + 1];
                    rdram[rom_rdram_base + ext_start + i + 3] = ext_data[i + 0];
                }
                fprintf(stderr, "[init] Mapped extended ROM NI data: %zu bytes (byte-swapped) at rdram+0x%08X\n",
                        ext_size, (uint32_t)(rom_rdram_base + ext_start));
            }
        }
    }

    // Pre-seed Controller Pak status so the game's init function doesn't see
    // a "change" on first check (which causes "Controller Pak has been changed" loop).
    //
    // Two locations must be consistent:
    // 1. OSContStatus at 0x800EFB90 (4 bytes per controller: u16 type, u8 status, u8 err)
    //    - The init function reads status byte (offset +2) before contpak_get_inserted_status
    //      has a chance to populate it via PIF. Pre-seeding prevents a 0→1 transition.
    // 2. Pak insertion array at 0x800F2260 (1 byte per controller: 0=inserted, 1=not)
    {
        // OSContStatus: controller 0 = connected with pak, controllers 1-3 = no response
        uint32_t cs = 0x000EFB90; // OSContStatus physical base
        // Controller 0: type=0x0005, status=0x01 (CONT_CARD_ON), err=0x00
        rdram[(cs+0) ^ 3] = 0x00; // type high
        rdram[(cs+1) ^ 3] = 0x05; // type low
        rdram[(cs+2) ^ 3] = 0x01; // status: CONT_CARD_ON
        rdram[(cs+3) ^ 3] = 0x00; // err: success
        // Controllers 1-3: not connected
        for (int i = 1; i < 4; i++) {
            rdram[(cs+i*4+0) ^ 3] = 0x00;
            rdram[(cs+i*4+1) ^ 3] = 0x00;
            rdram[(cs+i*4+2) ^ 3] = 0x00;
            rdram[(cs+i*4+3) ^ 3] = 0x08; // CONT_NO_RESPONSE_ERROR
        }

        // Pak insertion: controller 0 inserted, others not
        uint32_t ps = 0x000F2260;
        rdram[(ps+0) ^ 3] = 0; // controller 0: inserted
        rdram[(ps+1) ^ 3] = 1; // controller 1: not inserted
        rdram[(ps+2) ^ 3] = 1;
        rdram[(ps+3) ^ 3] = 1;

        fprintf(stderr, "[init] Pre-seeded OSContStatus and pak status\n");
    }
}
