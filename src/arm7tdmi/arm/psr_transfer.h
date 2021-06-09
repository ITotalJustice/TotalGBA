#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


GBA_INLINE void arm_msr(struct GBA_Core* gba, uint32_t opcode);

#ifdef __cplusplus
}
#endif
