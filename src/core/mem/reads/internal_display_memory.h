#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/mem/common.h"

/*
Internal Display Memory

  05000000-050003FF   BG/OBJ Palette RAM        (1 Kbyte)
  05000400-05FFFFFF   Not used
  06000000-06017FFF   VRAM - Video RAM          (96 KBytes)
  06018000-06FFFFFF   Not used
  07000000-070003FF   OAM - OBJ Attributes      (1 Kbyte)
  07000400-07FFFFFF   Not used

*/

/* palette_ram reads */
static inline uint8_t palette_ram_read8(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x050003FF) {
        return read_array8(MMIO.palette_ram, addr, 0x3FF);
    }
    return 0xFF; // unused
}

static inline uint16_t palette_ram_read16(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x050003FF) {
        return read_array16(MMIO.palette_ram, addr, 0x3FF);
    }
    return 0xFF; // unused
}

static inline uint32_t palette_ram_read32(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x050003FF) {
        return read_array32(MMIO.palette_ram, addr, 0x3FF);
    }
    return 0xFF; // unused
}

/* vram reads */
static inline uint8_t vram_read8(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x06017FFF) {
        return read_array8(MMIO.vram, addr, 0x17FFF);
    }
    return 0xFF; // unused
}

static inline uint16_t vram_read16(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x06017FFF) {
        return read_array16(MMIO.vram, addr, 0x17FFF);
    }
    return 0xFF; // unused
}

static inline uint32_t vram_read32(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x06017FFF) {
        return read_array32(MMIO.vram, addr, 0x17FFF);
    }
    return 0xFF; // unused
}

/* oam reads */
static inline uint8_t oam_read8(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x070003FF) {
        return read_array8(MMIO.oam, addr, 0x3FF);
    }
    return 0xFF; // unused
}

static inline uint16_t oam_read16(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x070003FF) {
        return read_array16(MMIO.oam, addr, 0x3FF);
    }
    return 0xFF; // unused
}

static inline uint32_t oam_read32(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x070003FF) {
        return read_array32(MMIO.oam, addr, 0x3FF);
    }
    return 0xFF; // unused
}

#ifdef __cplusplus
}
#endif
