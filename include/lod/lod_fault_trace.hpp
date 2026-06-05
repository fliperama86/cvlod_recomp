#pragma once

#include <cstdint>

struct LodKseg0FaultTraceSnapshot {
    uint32_t magic;
    uint32_t count;
    uint32_t func_vram;
    uint32_t gs;
    uint32_t map_load_count;
    uint32_t map_rom;
    uint32_t map_size;
    uint32_t ra;
    uint32_t sp;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t v0;
    uint32_t v1;
    uint32_t s0;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t t7;
    uint32_t t8;
    uint32_t t9;
    uint32_t chunk_addr;
    uint32_t chunk_phys;
    uint32_t slot;
    uint32_t pending;
    uint32_t entry_rom;
    uint32_t entry_ram;
    uint32_t entry_size;
    uint32_t entry_file_id;
    uint32_t entry_user_ptr;
};

extern "C" LodKseg0FaultTraceSnapshot lod_kseg0_fault_trace_snapshot;
