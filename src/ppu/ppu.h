#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


void ppu_init(struct GBA_Core* gba);
void ppu_run(struct GBA_Core* gba, uint16_t cycles);

void ppu_ly_lyc_compare(struct GBA_Core* gba);

#ifdef __cplusplus
}
#endif
