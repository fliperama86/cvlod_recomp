#include <thread>
#include <chrono>
#include "librecomp/rsp.hpp"

// Audio RSP stub — returns Broke immediately (no audio processing).
// TODO: Use RSPRecomp tool to generate proper audio RSP recompilation from the ROM's audio microcode.
RspExitReason aspMain(uint8_t* rdram, uint32_t ucode_addr) {
    // Simulate real RSP audio processing time (~1ms) to prevent the high-priority
    // task scheduler thread from starving the lower-priority gfx scheduler thread.
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return RspExitReason::Broke;
}
