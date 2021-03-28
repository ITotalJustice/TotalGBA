#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include <assert.h>


#define MMIO gba->mmio


enum {
    // General Internal Memory
    BIOS_BEGIN          = 0x00000000,
    BIOS_END            = 0x00003FFF,
    EWRAM_BEGIN         = 0x02000000,
    EWRAM_END           = 0x0203FFFF,
    IWRAM_BEGIN         = 0x03000000,
    IWRAM_END           = 0x03007FFF,
    IO_BEGIN            = 0x04000000,
    IO_END              = 0x040003FE,
    // Internal Display Memory
    PALETTE_RAM_BEGIN   = 0x05000000,
    PALETTE_RAM_END     = 0x050003FF,
    VRAM_BEGIN          = 0x06000000,
    VRAM_END            = 0x06017FFF,
    OAM_BEGIN           = 0x07000000,
    OAM_END             = 0x070003FF,
    // External Memory (Game Pak)
    ROM1_BEGIN          = 0x08000000, 
    ROM1_END            = 0x09FFFFFF,
    ROM2_BEGIN          = 0x0A000000, 
    ROM2_END            = 0x0BFFFFFF,
    ROM3_BEGIN          = 0x0C000000, 
    ROM3_END            = 0x0DFFFFFF,
    SRAM_BEGIN          = 0x0E000000,
    SRAM_END            = 0x0E00FFFF,
};

enum {
    // General Internal Memory
    BIOS_MASK           = 0x00003FFF, // not checked if this is correct!!!
    EWRAM_MASK          = 0x0003FFFF,
    IWRAM_MASK          = 0x00007FFF,
    // Internal Display Memory
    PALETTE_RAM_MASK    = 0x000003FF,
    VRAM_MASK           = 0x00017FFF,
    OAM_MASK            = 0x000003FF,
    // External Memory (Game Pak)
    ROM_MASK            = 0x01FFFFFF,
    SRAM_MASK           = 0x0000FFFF,
};

/* reads*/
static inline uint8_t read_array8(const uint8_t* array, const uint32_t addr, const uint32_t mask) {
    return array[addr & mask];
}

static inline uint16_t read_array16(const uint8_t* array, const uint32_t addr, const uint32_t mask) {
    const uint8_t hi = array[(addr + 0) & mask];
    const uint8_t lo = array[(addr + 1) & mask];

    return (lo << 8) | hi;
}

static inline uint32_t read_array32(const uint8_t* array, const uint32_t addr, const uint32_t mask) {
    const uint32_t hi_word_hi = array[(addr + 0) & mask];
    const uint32_t hi_word_lo = array[(addr + 1) & mask];
    const uint32_t lo_word_hi = array[(addr + 2) & mask];
    const uint32_t lo_word_lo = array[(addr + 3) & mask];

    return (lo_word_lo << 24) | (lo_word_hi << 16) | (hi_word_lo << 8) | hi_word_hi;
}

/* writes */
static inline void write_array8(uint8_t* array, const uint32_t addr, const uint8_t value, const uint32_t mask) {
    array[addr & mask] = value;
}

static inline void write_array16(uint8_t* array, const uint32_t addr, const uint16_t value, const uint32_t mask) {
    array[(addr + 0) & mask] = (value >> 0) & 0xFF;
    array[(addr + 1) & mask] = (value >> 8) & 0xFF;
}

static inline void write_array32(uint8_t* array, const uint32_t addr, const uint32_t value, const uint32_t mask) {
    array[(addr + 0) & mask] = (value >> 0) & 0xFF;
    array[(addr + 1) & mask] = (value >> 8) & 0xFF;
    array[(addr + 2) & mask] = (value >> 16) & 0xFF;
    array[(addr + 3) & mask] = (value >> 24) & 0xFF;
}

#ifdef __cplusplus
}
#endif
