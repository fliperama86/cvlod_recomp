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

// Decompressed NI file address table (used by rt64_render_context.cpp for segment 6 resolution)
uint32_t ni_decompressed_addrs[1024] = {};
int ni_decompressed_count = 0;

void lod_on_init(uint8_t* rdram, recomp_context* ctx) {
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
    // Section 4: map_ovl_00 (ROM 0x76CD00 → RAM 0x802E3B70)
    load_overlays(0x76CD00, 0x802E3B70, 0x779510 - 0x76CD00);

    // === KSEK1 hardware register region ===
    // N64 KSEK1 addresses (0xA0000000-0xBFFFFFFF) map to physical 0x00000000-0x1FFFFFFF.
    // In the recomp, MEM_W(0xA4600000) → rdram + 0x24600000 (beyond 512MB committed).
    // mprotect these regions so hardware register accesses don't SIGBUS.
    {
        // PI registers: 0x04600000-0x046FFFFF → rdram offset 0x24600000
        // SP registers: 0x04000000-0x040FFFFF → rdram offset 0x24000000
        // VI registers: 0x04400000-0x044FFFFF → rdram offset 0x24400000
        // AI registers: 0x04500000-0x045FFFFF → rdram offset 0x24500000
        // SI registers: 0x04800000-0x048FFFFF → rdram offset 0x24800000
        // Map a wide range covering all hardware registers.
        // Cover full KSEK1 physical range: 0x00000000-0x3FFFFFFF → rdram+0x20000000 to +0x40000000
        int ret = mprotect(rdram + 0x20000000, 0x20000000, PROT_READ | PROT_WRITE); // 512MB
        fprintf(stderr, "[mprotect] KSEK1 physical range: rdram+0x20000000 (512 MB) %s\n",
                ret == 0 ? "OK" : "FAILED");
        // Zero-initialize the HW register region to prevent stale data
        if (ret == 0) {
            memset(rdram + 0x20000000, 0, 0x20000000);
            fprintf(stderr, "[mprotect] KSEK1 region zeroed (512 MB)\n");
        }
    }

    // === KSEK0 RDRAM mirror ===
    // Map 8MB at rdram+0x80000000 so KSEK0 addresses (0x80XXXXXX) resolve correctly.
    {
        int ret = mprotect(rdram + 0x80000000, 8 * 1024 * 1024, PROT_READ | PROT_WRITE);
        fprintf(stderr, "[mprotect] KSEK0 RDRAM mirror: rdram+0x80000000 (8 MB) %s\n",
                ret == 0 ? "OK" : "FAILED");
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

    // === Copy overlay_system data (byte-swapped) ===
    {
        constexpr uint32_t overlay_system_rom = 0x74BC10;
        constexpr uint32_t overlay_system_size = 0x755030 - 0x74BC10; // 38432 bytes
        constexpr uint32_t rdram_dst = 0x801D1880 - 0x80000000;       // 0x1D1880
        for (uint32_t i = 0; i < overlay_system_size; i += 4) {
            if (overlay_system_rom + i + 3 < rom.size()) {
                rdram[rdram_dst + i + 0] = rom[overlay_system_rom + i + 3];
                rdram[rdram_dst + i + 1] = rom[overlay_system_rom + i + 2];
                rdram[rdram_dst + i + 2] = rom[overlay_system_rom + i + 1];
                rdram[rdram_dst + i + 3] = rom[overlay_system_rom + i + 0];
            }
        }
        fprintf(stderr, "[init] Copied overlay_system data (byte-swapped): ROM 0x%X → rdram+0x%X (%u bytes)\n",
                overlay_system_rom, rdram_dst, overlay_system_size);
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

        // Dump NI files as raw binaries for asset extraction
        {
            char dir[] = "/tmp/lod_assets/ni_files";
            mkdir(dir, 0755);
            int dumped = 0;
            for (int f = 0; f < ni_decompressed_count && f < 1024; f++) {
                uint32_t ext = ni_decompressed_addrs[f];
                if (ext == 0) continue;
                uint32_t phys = ext & 0x7FFFFFFF;
                // Estimate size from next file's address
                uint32_t next_phys = 0;
                for (int g = f + 1; g < ni_decompressed_count && g < 1024; g++) {
                    if (ni_decompressed_addrs[g] != 0) {
                        next_phys = ni_decompressed_addrs[g] & 0x7FFFFFFF;
                        break;
                    }
                }
                uint32_t size = (next_phys > phys) ? (next_phys - phys) : 65536;
                if (size > 512 * 1024) size = 512 * 1024;
                char path[128];
                snprintf(path, sizeof(path), "%s/ni_%03d.bin", dir, f);
                FILE* nf = fopen(path, "wb");
                if (nf) {
                    fwrite(rdram + phys, 1, size, nf);
                    fclose(nf);
                    dumped++;
                }
            }
            fprintf(stderr, "[init] Dumped %d NI files to %s/\n", dumped, dir);
        }
        // Debug: check file[97] data
        if (ni_decompressed_addrs[97] != 0) {
            uint32_t phys = ni_decompressed_addrs[97] & 0x7FFFFFFF;
            fprintf(stderr, "[init] file[97] at 0x%08X, phys rdram+0x%08X, first 16 bytes:",
                    ni_decompressed_addrs[97], phys);
            for (int b = 0; b < 16; b++) fprintf(stderr, " %02X", rdram[phys + b]);
            fprintf(stderr, "\n");
        } else {
            fprintf(stderr, "[init] file[97] NOT loaded (addr=0)\n");
        }
    }

    // === Controller Pak detection ===
    // osContInit writes OSContStatus to rdram+0x80000000 (KSEK0 mirror for data_=0).
    // But the game reads CONT_CARD_ON from 0x800EFB92 (pad buffer + status offset).
    // Copy the status byte so the game sees the Controller Pak.
    {
        // OSContStatus layout: { u16 type, u8 status, u8 err_no } = 4 bytes per controller
        // osContInit wrote to KSEK0 mirror: rdram[0x80000002] = status
        // Force CONT_CARD_ON (0x01) at the game's pad buffer status byte.
        // osContInit's output goes to the wrong address (KSEK0 mirror + byte-swap).
        rdram[0x000EFB92] = 0x01; // CONT_CARD_ON
        // Also set max_controllers for PIF formatter
        rdram[0x0013EDB1] = 1;
        fprintf(stderr, "[init] Controller Pak: CONT_CARD_ON → 0x800EFB92, max_controllers=1\n");
    }

    // === Dummy NI system object ===
    {
        constexpr uint32_t ni_sys_ptr_addr = 0x801CAC1C; // sys+0x295C
        constexpr uint32_t dummy_ni_obj = 0x80310000;
        constexpr uint32_t dummy_rdram_off = dummy_ni_obj - 0x80000000;
        memset(rdram + dummy_rdram_off, 0, 256);
        *(int32_t*)(rdram + (ni_sys_ptr_addr - 0x80000000)) = (int32_t)dummy_ni_obj;
        fprintf(stderr, "[init] Set sys+0x295C (NI system ptr) = 0x%08X (dummy)\n", dummy_ni_obj);
    }

    // === ROM byte-swap mapping ===
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
}
