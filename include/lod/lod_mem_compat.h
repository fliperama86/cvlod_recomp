// Minimal POSIX memory-API compatibility layer for the Windows build.
//
// The LoD runtime un-guards regions of librecomp's big rdram allocation with
// mprotect(..., PROT_READ | PROT_WRITE). librecomp allocates that region with
// VirtualAlloc(PAGE_NOACCESS) on Windows and mmap(PROT_NONE) elsewhere, so the
// Windows equivalent of those mprotect calls is VirtualProtect(PAGE_READWRITE).

#pragma once

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#define PROT_NONE  0x0
#define PROT_READ  0x1
#define PROT_WRITE 0x2

#define _SC_PAGESIZE 30

static inline int mprotect(void* addr, size_t len, int prot) {
    DWORD new_protect = PAGE_NOACCESS;
    if (prot & PROT_WRITE) {
        new_protect = PAGE_READWRITE;
    } else if (prot & PROT_READ) {
        new_protect = PAGE_READONLY;
    }
    DWORD old_protect = 0;
    return VirtualProtect(addr, len, new_protect, &old_protect) ? 0 : -1;
}

static inline long sysconf(int name) {
    if (name == _SC_PAGESIZE) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return (long)si.dwPageSize;
    }
    return -1;
}

#else

#include <unistd.h>
#include <sys/mman.h>

#endif
