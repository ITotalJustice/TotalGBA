#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"


// creates a 12-bit value which is used to index the instruction table
static inline uint32_t ARM_opcode_hash(const uint32_t opcode) {
	return ((opcode >> 16) & 0xFF0) | ((opcode >> 4) & 0xF);
}


#ifdef __cplusplus
}
#endif
