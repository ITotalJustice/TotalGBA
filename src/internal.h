#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#if defined __has_builtin
    #define GBA_HAS_BUILTIN(x) __has_builtin(x)
#else
    #define GBA_HAS_BUILTIN(x) (0)
#endif // __has_builtin

#if GBA_HAS_BUILTIN(__builtin_expect)
    #define LIKELY(c) (__builtin_expect(c,1))
    #define UNLIKELY(c) (__builtin_expect(c,0))
#else
    #define LIKELY(c) (c)
    #define UNLIKELY(c) (c)
#endif // __has_builtin(__builtin_expect)

#if GBA_HAS_BUILTIN(__builtin_unreachable)
    #define UNREACHABLE(ret) __builtin_unreachable()
#else
    #define UNREACHABLE(ret) return ret
#endif // __has_builtin(__builtin_unreachable)

// used mainly in debugging when i want to quickly silence
// the compiler about unsed vars.
#define UNUSED(var) ((void)(var))

// ONLY use this for C-arrays, not pointers, not structs
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#if GBA_DEBUG
    #include <stdio.h>
    #include <assert.h>
    #define GBA_log(...) fprintf(stdout, __VA_ARGS__)
    #define GBA_log_err(...) fprintf(stderr, __VA_ARGS__)
    #define GBA_log_fatal(...) fprintf(stderr, __VA_ARGS__); assert(0)
#else
    #define GBA_log(...)
    #define GBA_log_err(...)
    #define GBA_log_fatal(...)
#endif // NES_DEBUG

// 32-kib
#define MAX_ROM_SIZE 0x02000000

// mmio reads
GBA_INLINE uint8_t GBA_read8(struct GBA_Core* gba, const uint32_t addr);
GBA_INLINE uint16_t GBA_read16(struct GBA_Core* gba, const uint32_t addr);
GBA_INLINE uint32_t GBA_read32(struct GBA_Core* gba, const uint32_t addr);
// mmio writes
GBA_INLINE void GBA_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
GBA_INLINE void GBA_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
GBA_INLINE void GBA_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);

#ifdef __cplusplus
}
#endif
