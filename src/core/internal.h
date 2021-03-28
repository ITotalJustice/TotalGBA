#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// msvc prepro has a hard time with (macro && macro), so they have to be
// split into different if, else chains...
#if defined(__has_builtin)
#if __has_builtin(__builtin_expect)
#define LIKELY(c) (__builtin_expect(c,1))
#define UNLIKELY(c) (__builtin_expect(c,0))
#else
#define LIKELY(c) ((c))
#define UNLIKELY(c) ((c))
#endif // __has_builtin(__builtin_expect)
#else
#define LIKELY(c) ((c))
#define UNLIKELY(c) ((c))
#endif // __has_builtin

#if defined(__has_builtin)
#if __has_builtin(__builtin_unreachable)
#define UNREACHABLE(ret) __builtin_unreachable()
#else
#define UNREACHABLE(ret) return ret
#endif // __has_builtin(__builtin_unreachable)
#else
#define UNREACHABLE(ret) return ret
#endif // __has_builtin

// used mainly in debugging when i want to quickly silence
// the compiler about unsed vars.
#define UNUSED(var) ((void)(var))

// ONLY use this for C-arrays, not pointers, not structs
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))


// mmio reads
uint8_t GBA_read8(struct GBA_Core* gba, const uint32_t addr);
uint16_t GBA_read16(struct GBA_Core* gba, const uint32_t addr);
uint32_t GBA_read32(struct GBA_Core* gba, const uint32_t addr);
// mmio writes
void GBA_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
void GBA_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
void GBA_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);

#ifdef __cplusplus
}
#endif
