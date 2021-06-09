#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


GBA_INLINE void palette_ram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
GBA_INLINE void palette_ram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
GBA_INLINE void palette_ram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);

GBA_INLINE void vram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
GBA_INLINE void vram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
GBA_INLINE void vram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);

GBA_INLINE void oam_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
GBA_INLINE void oam_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
GBA_INLINE void oam_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);

#ifdef __cplusplus
}
#endif
