#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


GBA_INLINE uint8_t palette_ram_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t palette_ram_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t palette_ram_read32(struct GBA_Core* gba, uint32_t addr);

GBA_INLINE uint8_t vram_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t vram_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t vram_read32(struct GBA_Core* gba, uint32_t addr);

GBA_INLINE uint8_t oam_read8(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint16_t oam_read16(struct GBA_Core* gba, uint32_t addr);
GBA_INLINE uint32_t oam_read32(struct GBA_Core* gba, uint32_t addr);

#ifdef __cplusplus
}
#endif
