#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


GBA_INLINE uint8_t rom_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t rom_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t rom_read32(struct GBA_Core* gba, uint32_t addr);

GBA_INLINE uint8_t sram_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t sram_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t sram_read32(struct GBA_Core* gba, uint32_t addr);

#ifdef __cplusplus
}
#endif
