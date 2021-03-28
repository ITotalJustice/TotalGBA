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

/* ewram writes */
static inline void ewram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value) {
    if (addr >= EWRAM_BEGIN && addr <= EWRAM_END) {
        write_array8(MMIO.ewram, addr, value, EWRAM_MASK);
    }
}

static inline void ewram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    if (addr >= EWRAM_BEGIN && addr <= EWRAM_END) {
        write_array16(MMIO.ewram, addr, value, EWRAM_MASK);
    }
}

static inline void ewram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    if (addr >= EWRAM_BEGIN && addr <= EWRAM_END) {
        write_array32(MMIO.ewram, addr, value, EWRAM_MASK);
    }
}

/* iwram writes */
static inline void iwram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value) {
    if (addr >= IWRAM_BEGIN && addr <= IWRAM_END) {
        write_array8(MMIO.iwram, addr, value, IWRAM_MASK);
    }
}

static inline void iwram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    if (addr >= IWRAM_BEGIN && addr <= IWRAM_END) {
        write_array16(MMIO.iwram, addr, value, IWRAM_MASK);
    }
}

static inline void iwram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    if (addr >= IWRAM_BEGIN && addr <= IWRAM_END) {
        write_array32(MMIO.iwram, addr, value, IWRAM_MASK);
    }
}

#ifdef __cplusplus
}
#endif
