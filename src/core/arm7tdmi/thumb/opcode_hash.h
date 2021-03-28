#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"


// creates a 10-bit value which is used to index the instruction table
static inline uint16_t THUMB_opcode_hash(const uint16_t opcode) {
	return (opcode >> 6) & 0x3FF;
}


#ifdef __cplusplus
}
#endif
