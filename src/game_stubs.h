/**
 * CVLoD Recomp - Game Stub Declarations
 *
 * External declarations for stub implementations that replace
 * empty recompiled functions.
 */

#ifndef CVLOD_GAME_STUBS_H
#define CVLOD_GAME_STUBS_H

#include "recomp.h"

#ifdef __cplusplus
extern "C" {
#endif

// Game thread system stubs (from game_thread_stubs.cpp)
void impl_createThread(uint8_t* rdram, recomp_context* ctx);
void impl_startThread(uint8_t* rdram, recomp_context* ctx);
void impl_initScheduler(uint8_t* rdram, recomp_context* ctx);
void impl_unknown_800A9BC0(uint8_t* rdram, recomp_context* ctx);
void impl_initAudioScheduler(uint8_t* rdram, recomp_context* ctx);

#ifdef __cplusplus
}
#endif

#endif // CVLOD_GAME_STUBS_H
