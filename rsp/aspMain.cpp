#include "librecomp/rsp.hpp"

// Audio RSP stub — returns Broke immediately (no audio processing).
// TODO: Use RSPRecomp tool to generate proper audio RSP recompilation from the ROM's audio microcode.
RspExitReason aspMain(uint8_t* rdram, uint32_t ucode_addr) {
    return RspExitReason::Broke;
}
