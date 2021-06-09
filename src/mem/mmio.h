#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../types.h"

#define MMIO gba->mmio


enum
{
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

enum
{
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

GBA_INLINE uint8_t read_array8(const uint8_t* array, const uint32_t addr, const uint32_t mask);
GBA_INLINE uint16_t read_array16(const uint8_t* array, const uint32_t addr, const uint32_t mask);
GBA_INLINE uint32_t read_array32(const uint8_t* array, const uint32_t addr, const uint32_t mask);


GBA_INLINE void write_array8(uint8_t* array, const uint32_t addr, const uint8_t value, const uint32_t mask);
GBA_INLINE void write_array16(uint8_t* array, const uint32_t addr, const uint16_t value, const uint32_t mask);
GBA_INLINE void write_array32(uint8_t* array, const uint32_t addr, const uint32_t value, const uint32_t mask);

#ifdef __cplusplus
}
#endif
