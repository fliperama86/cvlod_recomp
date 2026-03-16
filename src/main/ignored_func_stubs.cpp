// Stubs for N64 OS functions that are in N64Recomp's ignored_funcs list.
// The recompiler doesn't generate code for these, but other recompiled OS
// internal functions still reference them via _recomp suffix.
// These are all handled natively by ultramodern, so empty stubs are safe.

#include <cstdint>
#include <cstdio>
#include "ultramodern/ultramodern.hpp"

struct recomp_context;  // forward declare

extern "C" {

void __osDequeueThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osEnqueueThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osDispatchThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osViSwapContext_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSpDeviceBusy_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSetCompare_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __ll_rshift_recomp(uint8_t* rdram, recomp_context* ctx) {}

// Batch 2: additional ignored functions called by recompiled code
void __osSiGetAccess_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSiRelAccess_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSiRawStartDma_recomp(uint8_t* rdram, recomp_context* ctx) {
    // Send SI completion message so the game doesn't hang waiting for SI DMA.
    // The actual DMA (PIF RAM read/write) is a no-op — controller data is
    // populated elsewhere. We just need the completion signal.
    ultramodern::send_si_message();
}
void __osContGetInitData_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPackRequestData_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPiRawReadIo_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPiRawWriteIo_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPfsCheckRamArea_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __f_to_ull_recomp(uint8_t* rdram, recomp_context* ctx) {}
void osPiGetCmdQueue_recomp(uint8_t* rdram, recomp_context* ctx) {}

}
