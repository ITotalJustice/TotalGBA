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
    if (addr <= 0x050003FF) {
        write_array8(MMIO.palette_ram, addr, value, 0x3FF);
    }
}

static inline void palette_ram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    if (addr <= 0x050003FF) {
        write_array16(MMIO.palette_ram, addr, value, 0x3FF);
    }
}

static inline void palette_ram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    if (addr <= 0x050003FF) {
        write_array32(MMIO.palette_ram, addr, value, 0x3FF);
    }
}

/* vram writes */
static inline void vram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value) {
    if (addr <= 0x06017FFF) {
        write_array8(MMIO.vram, addr, value, 0x17FFF);
    }
}

static inline void vram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    if (addr <= 0x06017FFF) {
        write_array16(MMIO.vram, addr, value, 0x17FFF);
    }
}

static inline void vram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    if (addr <= 0x06017FFF) {
        write_array32(MMIO.vram, addr, value, 0x17FFF);
    }
}

/* oam writes */
static inline void oam_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value) {
    if (addr <= 0x070003FF) {
        write_array8(MMIO.oam, addr, value, 0x3FF);
    }
}

static inline void oam_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    if (addr <= 0x070003FF) {
        write_array16(MMIO.oam, addr, value, 0x3FF);
    }
}

static inline void oam_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    if (addr <= 0x070003FF) {
        write_array32(MMIO.oam, addr, value, 0x3FF);
    }
}

#ifdef __cplusplus
}
#endif
