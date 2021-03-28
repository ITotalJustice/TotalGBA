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

/* palette_ram writes */
static inline void palette_ram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value) {
    if (addr >= PALETTE_RAM_BEGIN && addr <= PALETTE_RAM_END) {
        write_array8(MMIO.palette_ram, addr, value, PALETTE_RAM_MASK);
    }
}

static inline void palette_ram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    if (addr >= PALETTE_RAM_BEGIN && addr <= PALETTE_RAM_END) {
        write_array16(MMIO.palette_ram, addr, value, PALETTE_RAM_MASK);
    }
}

static inline void palette_ram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    if (addr >= PALETTE_RAM_BEGIN && addr <= PALETTE_RAM_END) {
        write_array32(MMIO.palette_ram, addr, value, PALETTE_RAM_MASK);
    }
}

/* vram writes */
static inline void vram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value) {
    if (addr >= VRAM_BEGIN && addr <= VRAM_END) {
        write_array8(MMIO.vram, addr, value, VRAM_MASK);
    }
}

static inline void vram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    if (addr >= VRAM_BEGIN && addr <= VRAM_END) {
        write_array16(MMIO.vram, addr, value, VRAM_MASK);
    }
}

static inline void vram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    if (addr >= VRAM_BEGIN && addr <= VRAM_END) {
        write_array32(MMIO.vram, addr, value, VRAM_MASK);
    }
}

/* oam writes */
static inline void oam_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value) {
    if (addr >= OAM_BEGIN && addr <= OAM_END) {
        write_array8(MMIO.oam, addr, value, OAM_MASK);
    }
}

static inline void oam_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    if (addr >= OAM_BEGIN && addr <= OAM_END) {
        write_array16(MMIO.oam, addr, value, OAM_MASK);
    }
}

static inline void oam_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    if (addr >= OAM_BEGIN && addr <= OAM_END) {
        write_array32(MMIO.oam, addr, value, OAM_MASK);
    }
}

#ifdef __cplusplus
}
#endif
