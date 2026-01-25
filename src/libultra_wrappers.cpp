/**
 * CVLoD Recomp - libultra Function Wrappers
 *
 * These wrappers redirect stubbed libultra functions to ultramodern implementations.
 * Since we're not linking librecomp, we call ultramodern functions directly.
 */

#include <cstdio>
#include "recomp.h"
#include "ultramodern/ultra64.h"

// ============================================================================
// libultra Function Address Mapping (ROM offset -> VRAM)
// Formula: VRAM = 0x80000000 + ROM_offset - 0xC00
// ============================================================================
// 0x8007F2E0 = osSendMesg
// 0x8007F860 = osRecvMesg
// 0x80089EC0 = osCreateMesgQueue
// 0x800812C0 = osCreateThread
// 0x80081D80 = osInitialize
// 0x8008D800 = osYieldThread
// 0x8008A2E0 = osJamMesg
// 0x8008A430 = osSetThreadPri
// 0x8008A510 = osGetThreadPri
// 0x8008AFB0 = osGetTime
// 0x8008E090 = osSetTimer
// 0x80080CB0 = osDestroyThread
// 0x80081C80 = osVirtualToPhysical

// External function to set a wrapper in the function table
extern void set_libultra_wrapper(int32_t vram, recomp_func_t* wrapper);

// ============================================================================
// Logging and tracing
// ============================================================================

static bool g_trace_libultra = true;

// ============================================================================
// Wrapper Functions - directly calling ultramodern
// ============================================================================

static void wrapper_osCreateMesgQueue(uint8_t* rdram, recomp_context* ctx) {
    // a0=mq, a1=msg, a2=count
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    PTR(OSMesg) msg = (PTR(OSMesg))ctx->r5;
    s32 count = (s32)ctx->r6;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osCreateMesgQueue(mq=0x%08X, msg=0x%08X, count=%d)\n",
               (uint32_t)mq, (uint32_t)msg, count);
    }

    osCreateMesgQueue(rdram, mq, msg, count);
}

static void wrapper_osSendMesg(uint8_t* rdram, recomp_context* ctx) {
    // a0=mq, a1=msg, a2=flags
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    OSMesg msg = (OSMesg)ctx->r5;
    s32 flags = (s32)ctx->r6;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osSendMesg(mq=0x%08X, msg=0x%08X, flags=%d)\n",
               (uint32_t)mq, (uint32_t)msg, flags);
    }

    s32 result = osSendMesg(rdram, mq, msg, flags);
    ctx->r2 = result;  // Return value in $v0
}

static void wrapper_osRecvMesg(uint8_t* rdram, recomp_context* ctx) {
    // a0=mq, a1=msg_ptr, a2=flags
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    PTR(OSMesg) msg_ptr = (PTR(OSMesg))ctx->r5;
    s32 flags = (s32)ctx->r6;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osRecvMesg(mq=0x%08X, msg_ptr=0x%08X, flags=%d)\n",
               (uint32_t)mq, (uint32_t)msg_ptr, flags);
    }

    s32 result = osRecvMesg(rdram, mq, msg_ptr, flags);
    ctx->r2 = result;  // Return value in $v0
}

static void wrapper_osJamMesg(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    OSMesg msg = (OSMesg)ctx->r5;
    s32 flags = (s32)ctx->r6;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osJamMesg(mq=0x%08X, msg=0x%08X, flags=%d)\n",
               (uint32_t)mq, (uint32_t)msg, flags);
    }

    s32 result = osJamMesg(rdram, mq, msg, flags);
    ctx->r2 = result;
}

static void wrapper_osCreateThread(uint8_t* rdram, recomp_context* ctx) {
    // a0=t, a1=id, a2=entry, a3=arg, stack: sp, pri
    PTR(OSThread) t = (PTR(OSThread))ctx->r4;
    OSId id = (OSId)ctx->r5;
    PTR(thread_func_t) entry = (PTR(thread_func_t))ctx->r6;
    PTR(void) arg = (PTR(void))ctx->r7;

    // Stack parameters
    PTR(void) sp = (PTR(void))MEM_W(0x10, ctx->r29);
    OSPri pri = (OSPri)MEM_W(0x14, ctx->r29);

    if (g_trace_libultra) {
        printf("[LIBULTRA] osCreateThread(t=0x%08X, id=%d, entry=0x%08X, arg=0x%08X, sp=0x%08X, pri=%d)\n",
               (uint32_t)t, id, (uint32_t)entry, (uint32_t)arg, (uint32_t)sp, pri);
    }

    osCreateThread(rdram, t, id, entry, arg, sp, pri);
}

static void wrapper_osStartThread(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSThread) t = (PTR(OSThread))ctx->r4;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osStartThread(t=0x%08X)\n", (uint32_t)t);
    }

    osStartThread(rdram, t);
}

static void wrapper_osStopThread(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSThread) t = (PTR(OSThread))ctx->r4;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osStopThread(t=0x%08X)\n", (uint32_t)t);
    }

    osStopThread(rdram, t);
}

static void wrapper_osDestroyThread(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSThread) t = (PTR(OSThread))ctx->r4;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osDestroyThread(t=0x%08X)\n", (uint32_t)t);
    }

    osDestroyThread(rdram, t);
}

static void wrapper_osYieldThread(uint8_t* rdram, recomp_context* ctx) {
    if (g_trace_libultra) {
        printf("[LIBULTRA] osYieldThread()\n");
    }

    osYieldThread(rdram);
}

static void wrapper_osSetThreadPri(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSThread) t = (PTR(OSThread))ctx->r4;
    OSPri pri = (OSPri)ctx->r5;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osSetThreadPri(t=0x%08X, pri=%d)\n", (uint32_t)t, pri);
    }

    osSetThreadPri(rdram, t, pri);
}

static void wrapper_osGetThreadPri(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSThread) t = (PTR(OSThread))ctx->r4;

    OSPri result = osGetThreadPri(rdram, t);

    if (g_trace_libultra) {
        printf("[LIBULTRA] osGetThreadPri(t=0x%08X) -> %d\n", (uint32_t)t, result);
    }

    ctx->r2 = result;
}

static void wrapper_osInitialize(uint8_t* rdram, recomp_context* ctx) {
    if (g_trace_libultra) {
        printf("[LIBULTRA] osInitialize()\n");
    }

    osInitialize();
}

static void wrapper_osGetTime(uint8_t* rdram, recomp_context* ctx) {
    OSTime time = osGetTime();

    // MIPS O32 ABI: 64-bit return in $v0 (hi) and $v1 (lo) for 32-bit mode
    ctx->r2 = (int32_t)(time >> 32);  // $v0 = high 32 bits
    ctx->r3 = (int32_t)(time & 0xFFFFFFFF);  // $v1 = low 32 bits

    if (g_trace_libultra) {
        printf("[LIBULTRA] osGetTime() -> 0x%08X%08X\n",
               (uint32_t)ctx->r2, (uint32_t)ctx->r3);
    }
}

static void wrapper_osGetCount(uint8_t* rdram, recomp_context* ctx) {
    u32 count = osGetCount();
    ctx->r2 = count;
    // Don't log - called too frequently
}

static void wrapper_osSetEventMesg(uint8_t* rdram, recomp_context* ctx) {
    OSEvent event = (OSEvent)ctx->r4;
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r5;
    OSMesg msg = (OSMesg)ctx->r6;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osSetEventMesg(event=%u, mq=0x%08X, msg=0x%08X)\n",
               event, (uint32_t)mq, (uint32_t)msg);
    }

    osSetEventMesg(rdram, event, mq, msg);
}

static void wrapper_osViSetEvent(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))ctx->r4;
    OSMesg msg = (OSMesg)ctx->r5;
    u32 retraceCount = (u32)ctx->r6;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osViSetEvent(mq=0x%08X, msg=0x%08X, retraceCount=%u)\n",
               (uint32_t)mq, (uint32_t)msg, retraceCount);
    }

    osViSetEvent(rdram, mq, msg, retraceCount);
}

static void wrapper_osVirtualToPhysical(uint8_t* rdram, recomp_context* ctx) {
    PTR(void) addr = (PTR(void))ctx->r4;

    u32 result = osVirtualToPhysical(addr);

    if (g_trace_libultra) {
        printf("[LIBULTRA] osVirtualToPhysical(0x%08X) -> 0x%08X\n",
               (uint32_t)addr, result);
    }

    ctx->r2 = result;
}

static void wrapper_osSetTimer(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSTimer) timer = (PTR(OSTimer))ctx->r4;

    // 64-bit values passed in register pairs (MIPS O32)
    // countdown in a1:a2 (r5:r6), interval in a3:stack (r7:stack[0x10])
    OSTime countdown = ((uint64_t)(uint32_t)ctx->r5 << 32) | (uint32_t)ctx->r6;
    OSTime interval = ((uint64_t)(uint32_t)ctx->r7 << 32) | (uint32_t)MEM_W(0x10, ctx->r29);
    PTR(OSMesgQueue) mq = (PTR(OSMesgQueue))MEM_W(0x14, ctx->r29);
    OSMesg msg = (OSMesg)MEM_W(0x18, ctx->r29);

    if (g_trace_libultra) {
        printf("[LIBULTRA] osSetTimer(timer=0x%08X, countdown=%llu, interval=%llu, mq=0x%08X)\n",
               (uint32_t)timer, (unsigned long long)countdown, (unsigned long long)interval,
               (uint32_t)mq);
    }

    int result = osSetTimer(rdram, timer, countdown, interval, mq, msg);
    ctx->r2 = result;
}

static void wrapper_osViSwapBuffer(uint8_t* rdram, recomp_context* ctx) {
    PTR(void) frameBufPtr = (PTR(void))ctx->r4;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osViSwapBuffer(frameBuf=0x%08X)\n", (uint32_t)frameBufPtr);
    }

    osViSwapBuffer(rdram, frameBufPtr);
}

static void wrapper_osViSetMode(uint8_t* rdram, recomp_context* ctx) {
    PTR(OSViMode) mode = (PTR(OSViMode))ctx->r4;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osViSetMode(mode=0x%08X)\n", (uint32_t)mode);
    }

    osViSetMode(rdram, mode);
}

static void wrapper_osViBlack(uint8_t* rdram, recomp_context* ctx) {
    uint8_t active = (uint8_t)ctx->r4;

    if (g_trace_libultra) {
        printf("[LIBULTRA] osViBlack(active=%d)\n", active);
    }

    osViBlack(active);
}

// ============================================================================
// Function to install wrappers into the function table
// ============================================================================

void install_libultra_wrappers() {
    printf("Installing libultra wrappers...\n");

    // Message Queue functions
    set_libultra_wrapper(0x80089EC0, wrapper_osCreateMesgQueue);  // osCreateMesgQueue
    set_libultra_wrapper(0x8007F2E0, wrapper_osSendMesg);         // osSendMesg
    set_libultra_wrapper(0x8007F860, wrapper_osRecvMesg);         // osRecvMesg
    set_libultra_wrapper(0x8008A2E0, wrapper_osJamMesg);          // osJamMesg
    set_libultra_wrapper(0x80088B80, wrapper_osSetEventMesg);     // osSetEventMesg

    // Thread functions
    set_libultra_wrapper(0x800812C0, wrapper_osCreateThread);     // osCreateThread
    set_libultra_wrapper(0x80080CB0, wrapper_osDestroyThread);    // osDestroyThread
    set_libultra_wrapper(0x8008D800, wrapper_osYieldThread);      // osYieldThread
    set_libultra_wrapper(0x8008A430, wrapper_osSetThreadPri);     // osSetThreadPri
    set_libultra_wrapper(0x8008A510, wrapper_osGetThreadPri);     // osGetThreadPri

    // System functions
    set_libultra_wrapper(0x80081D80, wrapper_osInitialize);       // osInitialize
    set_libultra_wrapper(0x80081C80, wrapper_osVirtualToPhysical); // osVirtualToPhysical

    // Timer functions
    set_libultra_wrapper(0x8008AFB0, wrapper_osGetTime);          // osGetTime
    set_libultra_wrapper(0x8008E090, wrapper_osSetTimer);         // osSetTimer
    set_libultra_wrapper(0x80084550, wrapper_osGetCount);         // osGetCount

    // VI functions
    set_libultra_wrapper(0x80088B80, wrapper_osViSetEvent);       // osViSetEvent

    printf("libultra wrappers installed.\n");
}

// Set libultra tracing on/off
void set_libultra_tracing(bool enable) {
    g_trace_libultra = enable;
}
