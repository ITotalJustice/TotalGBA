#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

// used in thumb as well
void arm_bx_generic(struct GBA_Core* gba, uint32_t addr);

void arm_bx(struct GBA_Core* gba, uint32_t opcode);
void arm_branch(struct GBA_Core* gba, uint32_t opcode);
void arm_branch_link(struct GBA_Core* gba, uint32_t opcode);

#ifdef __cplusplus
}
#endif
