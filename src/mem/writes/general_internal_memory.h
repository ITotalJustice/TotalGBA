#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


GBA_INLINE void ewram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
GBA_INLINE void ewram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
GBA_INLINE void ewram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);

GBA_INLINE void iwram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
GBA_INLINE void iwram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
GBA_INLINE void iwram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);

GBA_INLINE void io_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value);
GBA_INLINE void io_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value);
GBA_INLINE void io_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value);

#ifdef __cplusplus
}
#endif
