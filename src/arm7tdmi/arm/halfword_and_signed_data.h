#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


void arm_strh_imm(struct GBA_Core* gba, uint32_t opcode);

#ifdef __cplusplus
}
#endif
