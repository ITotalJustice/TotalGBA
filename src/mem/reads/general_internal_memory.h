#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


GBA_INLINE uint8_t bios_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t bios_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t bios_read32(struct GBA_Core* gba, uint32_t addr);

GBA_INLINE uint8_t ewram_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t ewram_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t ewram_read32(struct GBA_Core* gba, uint32_t addr);

GBA_INLINE uint8_t iwram_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t iwram_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t iwram_read32(struct GBA_Core* gba, uint32_t addr);

GBA_INLINE uint8_t io_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t io_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t io_read32(struct GBA_Core* gba, uint32_t addr);

#ifdef __cplusplus
}
#endif
