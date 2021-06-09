#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


GBA_INLINE uint32_t bit_rotr(const uint32_t shift, const uint32_t value);

//
GBA_INLINE bool is_bit_set(const uint8_t bit, const uint32_t value);

//
GBA_INLINE uint32_t get_bit_range(const uint8_t start, const uint8_t end, const uint32_t value);

//
GBA_INLINE int32_t sign_extend(const uint8_t start_size, const uint32_t value);

#ifdef __cplusplus
}
#endif
