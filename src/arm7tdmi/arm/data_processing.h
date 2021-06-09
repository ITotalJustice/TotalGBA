#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


void arm_add_imm(struct GBA_Core* gba, uint32_t opcode);
void arm_mov_imm(struct GBA_Core* gba, uint32_t opcode);

#ifdef __cplusplus
}
#endif
