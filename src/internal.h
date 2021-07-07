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

// PPU REGISTERS
#define IO_DISPCNT gba->mmio.DISPCNT
#define IO_DISPSTAT gba->mmio.DISPSTAT
#define IO_VCOUNT gba->mmio.VCOUNT
#define IO_BG0CNT gba->mmio.BG0CNT
#define IO_BG1CNT gba->mmio.BG1CNT
#define IO_BG2CNT gba->mmio.BG2CNT
#define IO_BG3CNT gba->mmio.BG3CNT
#define IO_BG0HOFS gba->mmio.BG0HOFS
#define IO_BG0VOFS gba->mmio.BG0VOFS
#define IO_BG1HOFS gba->mmio.BG1HOFS
#define IO_BG1VOFS gba->mmio.BG1VOFS
#define IO_BG2HOFS gba->mmio.BG2HOFS
#define IO_BG2VOFS gba->mmio.BG2VOFS
#define IO_BG3HOFS gba->mmio.BG3HOFS
#define IO_BG3VOFS gba->mmio.BG3VOFS

// INTERRUPTS REGISTERS
#define IO_IF gba->mmio.IF
#define IO_IE gba->mmio.IE
// #define IO_IF gba->mmio.IF

// mmio reads
GBA_INLINE uint8_t GBA_read8(struct GBA_Core* gba, const uint32_t addr);
GBA_INLINE uint16_t GBA_read16(struct GBA_Core* gba, const uint32_t addr);
GBA_INLINE uint32_t GBA_read32(struct GBA_Core* gba, const uint32_t addr);
// mmio writes
GBA_INLINE void GBA_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
GBA_INLINE void GBA_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
GBA_INLINE void GBA_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);


GBA_INLINE void GBA_irq(struct GBA_Core* gba, enum InterruptFlag flag);
GBA_INLINE void GBA_handle_irq(struct GBA_Core* gba, enum InterruptFlag flag);

#ifdef __cplusplus
}
#endif
