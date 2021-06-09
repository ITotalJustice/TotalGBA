#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


GBAAPI bool GBA_init(struct GBA_Core* gba);

GBAAPI void GBA_reset(struct GBA_Core* gba);

GBAAPI bool GBA_loadrom(struct GBA_Core* gba, const uint8_t* data, size_t size);

GBAAPI void GBA_run_frame(struct GBA_Core* gba);

// these are stubbed atm, but will be added soon
// static inline void GBA_set_error_callback(struct GBA_Core* gba, GBA_error_callback_t cb, void* user_data) {}
// static inline void GBA_set_buttons(struct GBA_Core* gba, uint8_t buttons, bool pressed) {}

#ifdef __cplusplus
}
#endif
