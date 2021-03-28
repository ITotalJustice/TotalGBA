#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


void GBA_init(struct GBA_Core* gba);

void GBA_reset(struct GBA_Core* gba);

int GBA_loadrom_data(struct GBA_Core* gba, const uint8_t* data, size_t size);

void GBA_run_frame(struct GBA_Core* gba);

// these are stubbed atm, but will be added soon
static inline void GBA_set_error_callback(struct GBA_Core* gba, GBA_error_callback_t cb, void* user_data) {}
static inline void GBA_set_buttons(struct GBA_Core* gba, uint8_t buttons, bool pressed) {}

#ifdef __cplusplus
}
#endif
