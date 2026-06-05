#include <thread>
#include <chrono>
#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <cstring>
#include "librecomp/rsp.hpp"

#ifndef LOD_ENABLE_RSP_AUDIO_TRACE
#define LOD_ENABLE_RSP_AUDIO_TRACE 0
#endif

#if LOD_ENABLE_RSP_AUDIO_TRACE
static const uint8_t* lod_rdram_addr(uint8_t* rdram, uint32_t addr, uint32_t size) {
    uint32_t off = addr & 0x7FFFFF;
    if (off >= 0x800000 || size > 0x800000 - off) {
        return nullptr;
    }
    return rdram + off;
}

static void lod_dump_rdram_range(uint8_t* rdram, uint32_t addr, uint32_t size, const char* path) {
    const uint8_t* ptr = lod_rdram_addr(rdram, addr, size);
    if (ptr == nullptr || size == 0) {
        fprintf(stderr, "[RSP_AUDIO] dump skipped path=%s addr=0x%08" PRIX32 " size=0x%X\n",
                path, addr, size);
        return;
    }

    FILE* f = fopen(path, "wb");
    if (f == nullptr) {
        fprintf(stderr, "[RSP_AUDIO] dump failed path=%s\n", path);
        return;
    }
    fwrite(ptr, 1, size, f);
    fclose(f);
    fprintf(stderr, "[RSP_AUDIO] dumped path=%s addr=0x%08" PRIX32 " size=0x%X\n",
            path, addr, size);
}
#endif

// Audio RSP stub — returns Broke immediately (no audio processing).
// TODO: Use RSPRecomp tool to generate proper audio RSP recompilation from the ROM's audio microcode.
RspExitReason aspMain(uint8_t* rdram, uint32_t ucode_addr) {
#if LOD_ENABLE_RSP_AUDIO_TRACE
    static uint32_t task_count = 0;
    task_count++;
    const OSTask* task = reinterpret_cast<const OSTask*>(dmem + 0xFC0);
    if (task_count <= 20 || (task_count % 120) == 0) {
        uint32_t out_size_value = 0;
        if (const uint8_t* out_size_ptr = lod_rdram_addr(rdram, task->t.output_buff_size, sizeof(uint32_t))) {
            std::memcpy(&out_size_value, out_size_ptr, sizeof(out_size_value));
        }
        fprintf(stderr,
                "[RSP_AUDIO] task#%u arg_ucode=0x%08" PRIX32
                " type=%" PRIu32 " flags=0x%08" PRIX32
                " boot=0x%08" PRIX32 "/0x%X ucode=0x%08" PRIX32 "/0x%X"
                " data=0x%08" PRIX32 "/0x%X stack=0x%08" PRIX32 "/0x%X"
                " out=0x%08" PRIX32 " out_size_ptr=0x%08" PRIX32 " out_size_val=0x%08" PRIX32
                " data_ptr=0x%08" PRIX32 "/0x%X yield=0x%08" PRIX32 "/0x%X\n",
                task_count, ucode_addr, task->t.type, task->t.flags,
                task->t.ucode_boot, task->t.ucode_boot_size,
                task->t.ucode, task->t.ucode_size,
                task->t.ucode_data, task->t.ucode_data_size,
                task->t.dram_stack, task->t.dram_stack_size,
                task->t.output_buff, task->t.output_buff_size, out_size_value,
                task->t.data_ptr, task->t.data_size,
                task->t.yield_data_ptr, task->t.yield_data_size);
    }
    if (task_count == 1) {
        lod_dump_rdram_range(rdram, task->t.ucode_boot, std::min<uint32_t>(task->t.ucode_boot_size, 0x1000),
                             "/tmp/lod_aspMain_ucode_boot.bin");
        const uint32_t ucode_dump_size = task->t.ucode_size != 0 ? std::min<uint32_t>(task->t.ucode_size, 0x2000)
                                                                 : 0x1000;
        lod_dump_rdram_range(rdram, task->t.ucode, ucode_dump_size,
                             "/tmp/lod_aspMain_ucode.bin");
        lod_dump_rdram_range(rdram, task->t.ucode - 0x100, ucode_dump_size + 0x100,
                             "/tmp/lod_aspMain_ucode_with_pre.bin");
        lod_dump_rdram_range(rdram, task->t.ucode_data, std::min<uint32_t>(task->t.ucode_data_size, 0x2000),
                             "/tmp/lod_aspMain_ucode_data.bin");
        lod_dump_rdram_range(rdram, task->t.data_ptr, std::min<uint32_t>(task->t.data_size, 0x4000),
                             "/tmp/lod_aspMain_alist.bin");
    }
#endif
    // Simulate real RSP audio processing time (~1ms) to prevent the high-priority
    // task scheduler thread from starving the lower-priority gfx scheduler thread.
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return RspExitReason::Broke;
}
