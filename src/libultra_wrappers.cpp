/**
 * CVLoD Recomp - libultra Function Overrides (Zelda 64 Style)
 *
 * This file implements direct overrides for recompiled libultra functions.
 * Since recompiled functions are marked as 'weak', these 'strong' definitions
 * will be preferred by the linker, effectively hijacking all calls.
 */

#include "recomp.h"
#include "ultramodern/ultra64.h"
#include <cstdio>

// Forward declarations from ultramodern
namespace ultramodern {
void submit_rsp_task(RDRAM_ARG PTR(OSTask) task);
}

// ============================================================================
// VI (Video Interface) Functions
// ============================================================================

// osViSetEvent
extern "C" void L_8008C4E0(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    OSMesg msg = (OSMesg)ctx->r5;
    u32 retraceCount = (u32)ctx->r6;

    printf("[LIBULTRA] osViSetEvent(mq=0x%08X, msg=0x%08X, retraceCount=%u)\n",
           (uint32_t)mq, (uint32_t)msg, retraceCount);
    
    osViSetEvent(rdram, mq, msg, retraceCount);
}

// osViSwapBuffer
extern "C" void func_8008A690(uint8_t *rdram, recomp_context *ctx) {
    PTR(void) frameBufPtr = (PTR(void))ctx->r4;

    printf("[LIBULTRA] osViSwapBuffer(frameBuf=0x%08X)\n", (uint32_t)frameBufPtr);
    
    osViSwapBuffer(rdram, frameBufPtr);
}

// osViSetMode
extern "C" void func_80083120(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSViMode) mode = (PTR(OSViMode))ctx->r4;

    printf("[LIBULTRA] osViSetMode(mode=0x%08X)\n", (uint32_t)mode);
    
    osViSetMode(rdram, mode);
}

// osViBlack
extern "C" void func_8008BD70(uint8_t *rdram, recomp_context *ctx) {
    uint8_t active = (uint8_t)ctx->r4;

    printf("[LIBULTRA] osViBlack(active=%d)\n", active);
    
    osViBlack(active);
}

// ============================================================================
// RSP Task Functions
// ============================================================================

// osSpTaskLoad
extern "C" void func_8007FFC0(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSTask) task = (PTR(OSTask))ctx->r4;

    printf("[LIBULTRA] osSpTaskLoad(task=0x%08X)\n", (uint32_t)task);
    // ultramodern handles loading during submit
}

// osSpTaskStartGo
extern "C" void func_800802B0(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSTask) task = (PTR(OSTask))ctx->r4;

    printf("[LIBULTRA] osSpTaskStartGo(task=0x%08X)\n", (uint32_t)task);
    
    ultramodern::submit_rsp_task(rdram, task);
}

// ============================================================================
// Message Queue Functions
// ============================================================================

// osCreateMesgQueue
extern "C" void func_80089EC0(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    PTR(OSMesg) msg = (PTR(OSMesg))ctx->r5;
    s32 count = (s32)ctx->r6;

    printf("[LIBULTRA] osCreateMesgQueue(mq=0x%08X, count=%d)\n", (uint32_t)mq, count);
    
    osCreateMesgQueue(rdram, mq, msg, count);
}

// osSendMesg
extern "C" void func_8007F2E0(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    OSMesg msg = (OSMesg)ctx->r5;
    s32 flags = (s32)ctx->r6;

    static int send_count = 0;
    send_count++;
    if (send_count <= 20) {
        printf("[LIBULTRA] osSendMesg(mq=0x%08X, msg=0x%08X, flags=%d)\n",
               (uint32_t)mq, (uint32_t)msg, flags);
    }

    s32 result = osSendMesg(rdram, mq, msg, flags);
    ctx->r2 = result;
}

// osRecvMesg
extern "C" void func_8007F860(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    PTR(OSMesg) msg_ptr = (PTR(OSMesg))ctx->r5;
    s32 flags = (s32)ctx->r6;

    static int recv_count = 0;
    recv_count++;
    if (recv_count <= 20) {
        printf("[LIBULTRA] osRecvMesg(mq=0x%08X, flags=%d) - waiting...\n",
               (uint32_t)mq, flags);
    }

    s32 result = osRecvMesg(rdram, mq, msg_ptr, flags);

    if (recv_count <= 20) {
        printf("[LIBULTRA] osRecvMesg returned %d\n", result);
    }
    ctx->r2 = result;
}

// ============================================================================
// Thread Functions
// ============================================================================

// osInitialize
extern "C" void func_80081D80(uint8_t *rdram, recomp_context *ctx) {
    printf("[LIBULTRA] osInitialize()\n");
    osInitialize();
}

// osCreateThread
extern "C" void func_800812C0(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSThread) t = (PTR(OSThread))ctx->r4;
    OSId id = (OSId)ctx->r5;
    PTR(thread_func_t) entry = (PTR(thread_func_t))ctx->r6;
    PTR(void) arg = (PTR(void))ctx->r7;
    PTR(void) sp = (PTR(void))MEM_W(0x10, ctx->r29);
    OSPri pri = (OSPri)MEM_W(0x14, ctx->r29);

    printf("[LIBULTRA] osCreateThread(t=0x%08X, id=%d, entry=0x%08X)\n", (uint32_t)t, id, (uint32_t)entry);
    
    osCreateThread(rdram, t, id, entry, arg, sp, pri);
}

// osStartThread
extern "C" void func_80090490(uint8_t *rdram, recomp_context *ctx) {
    PTR(OSThread) t = (PTR(OSThread))ctx->r4;
    printf("[LIBULTRA] osStartThread(t=0x%08X)\n", (uint32_t)t);
    osStartThread(rdram, t);
}

// Legacy install function (now empty as overrides are automatic)
void install_libultra_wrappers() {
    printf("libultra overrides active via linker interposition.\n");
}
