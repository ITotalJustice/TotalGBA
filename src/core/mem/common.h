#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include <assert.h>


#define MMIO gba->mmio

/* reads*/
static inline uint8_t read_array8(const uint8_t* array, const uint32_t addr, const uint32_t mask) {
    return array[addr & mask];
}

static inline uint16_t read_array16(const uint8_t* array, const uint32_t addr, const uint32_t mask) {
    const uint8_t hi = array[(addr + 0) & mask];
    const uint8_t lo = array[(addr + 1) & mask];

    return (lo << 8) | hi;
}

static inline uint32_t read_array32(const uint8_t* array, const uint32_t addr, const uint32_t mask) {
    const uint32_t hi_word_hi = array[(addr + 0) & mask];
    const uint32_t hi_word_lo = array[(addr + 1) & mask];
    const uint32_t lo_word_hi = array[(addr + 2) & mask];
    const uint32_t lo_word_lo = array[(addr + 3) & mask];

    return (lo_word_lo << 24) | (lo_word_hi << 16) | (hi_word_lo << 8) | hi_word_hi;
}

/* writes */
static inline void write_array8(uint8_t* array, const uint32_t addr, const uint8_t value, const uint32_t mask) {
    array[addr & mask] = value;
}

static inline void write_array16(uint8_t* array, const uint32_t addr, const uint16_t value, const uint32_t mask) {
    array[(addr + 0) & mask] = (value >> 0) & 0xFF;
    array[(addr + 1) & mask] = (value >> 8) & 0xFF;
}

static inline void write_array32(uint8_t* array, const uint32_t addr, const uint32_t value, const uint32_t mask) {
    array[(addr + 0) & mask] = (value >> 0) & 0xFF;
    array[(addr + 1) & mask] = (value >> 8) & 0xFF;
    array[(addr + 2) & mask] = (value >> 16) & 0xFF;
    array[(addr + 3) & mask] = (value >> 24) & 0xFF;
}

#ifdef __cplusplus
}
#endif
