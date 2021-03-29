#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <assert.h>


// NOTE: value might need to be cast to u64 first on 32bit!
static inline uint32_t bit_rotr(const uint32_t value, const uint32_t shift) {
    return (value >> shift) | (value << (32 - shift));
}

static inline bool is_bit_set(const uint8_t bit, const uint32_t value) {
    assert(bit < (sizeof(uint32_t) * 8) && "bit value out of bounds!");
    return (value & (1U << bit)) > 0;
}

static inline uint32_t get_bit_range(const uint8_t start, const uint8_t end, const uint32_t value) {
    assert(end > start && "invalid bit range!");

    return (value & (0xFFFFFFFF >> (31 - end))) >> start;
}

static inline int32_t sign_extend(const uint8_t start_size, const uint32_t value) {
    assert(start_size < 31 && "bit start size is out of bounds!");

    const uint8_t bits = 31 - start_size;
    return ((int32_t)(value << bits)) >> bits;
}

#ifdef __cplusplus
}
#endif
