#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/mem/common.h"

/*
General Internal Memory

  00000000-00003FFF   BIOS - System ROM         (16 KBytes)
  00004000-01FFFFFF   Not used
  02000000-0203FFFF   WRAM - On-board Work RAM  (256 KBytes) 2 Wait
  02040000-02FFFFFF   Not used
  03000000-03007FFF   WRAM - On-chip Work RAM   (32 KBytes)
  03008000-03FFFFFF   Not used
  04000000-040003FE   I/O Registers
  04000400-04FFFFFF   Not used
*/

/* bios reads */
static inline uint8_t bios_read8(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x00003FFF) {
        assert(0 && "bios not implemented!");
    }
    return 0xFF; // unused
}

static inline uint16_t bios_read16(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x00003FFF) {
        assert(0 && "bios not implemented!");
    }
    return 0xFF; // unused
}

static inline uint32_t bios_read32(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x00003FFF) {
        assert(0 && "bios not implemented!");
    }
    return 0xFF; // unused
}

/* ewram reads */
static inline uint8_t ewram_read8(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x01FFFFFF) {
        return read_array8(MMIO.ewram, addr, 0x3FFFF);
    }
    return 0xFF; // unused
}

static inline uint16_t ewram_read16(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x01FFFFFF) {
        return read_array16(MMIO.ewram, addr, 0x3FFFF);
    }
    return 0xFF; // unused
}

static inline uint32_t ewram_read32(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x01FFFFFF) {
        return read_array32(MMIO.ewram, addr, 0x3FFFF);
    }
    return 0xFF; // unused
}

/* iwram reads */
static inline uint8_t iwram_read8(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x03007FFF) {
        return read_array8(MMIO.iwram, addr, 0x7FFF);
    }
    return 0xFF; // unused
}

static inline uint16_t iwram_read16(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x03007FFF) {
        return read_array16(MMIO.iwram, addr, 0x7FFF);
    }
    return 0xFF; // unused
}

static inline uint32_t iwram_read32(struct GBA_Core* gba, uint32_t addr) {
    if (addr <= 0x03007FFF) {
        return read_array32(MMIO.iwram, addr, 0x7FFF);
    }
    return 0xFF; // unused
}

#ifdef __cplusplus
}
#endif