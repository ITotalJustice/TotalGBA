#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <assert.h>

/*
static inline unsigned get_bit_range(const uint8_t start, const uint8_t end, const unsigned v) {
    assert(end > start && "invalid bit range!");

    return (v & (0xFFFFFFFF >> (31 - end))) >> start;

    return (v >> start) & (0xFFFFFFFF >> (31 - (start + end)));
    
    return (v >> start) & MASK_RANGE_TABLE[end - start];
}
*/

static inline uint32_t get_bit_range(const uint8_t start, const uint8_t end, const uint32_t value) {
    assert(end > start && "invalid bit range!");

    return (value & (0xFFFFFFFF >> (31 - end))) >> start;
}

static inline uint32_t sign_extend(const uint8_t start_size, const uint32_t value) {
    assert(start_size < 31 && "bit start size is out of bounds!");

    const uint8_t bits = 31 - start_size;
    return (uint32_t)(((int32_t)(value << bits)) >> bits);
}

#ifdef __cplusplus
}
#endif
