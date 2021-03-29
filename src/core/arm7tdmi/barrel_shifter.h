#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/util/bit.h"

enum ShiftType {
    LSL, LSR, ASR, ROR, 
};

struct ShiftResult {
    uint32_t value;
    bool carry;
};

static inline struct ShiftResult barrel_shift_LSL(const uint32_t value, const uint32_t shift_v, const bool old_carry) {
    switch (shift_v) {
        case 0:
            return (struct ShiftResult){ .value = value, .carry = old_carry };
        case 1 ... 31:
            return (struct ShiftResult){ .value = value << shift_v, .carry = is_bit_set(32 - shift_v, value) };
        case 32:
            return (struct ShiftResult){ .value = 0, .carry = is_bit_set(0, value) };
        default:
            return (struct ShiftResult){ .value = 0, false };
    }
}

static inline struct ShiftResult barrel_shift_ROR(const uint32_t value, const uint32_t shift_v, const bool old_carry) {
    switch (shift_v) {
        case 0:
            return (struct ShiftResult){ .value = value, .carry = old_carry };
        case 1 ... 31:
            return (struct ShiftResult){ .value = bit_rotr(shift_v, value), .carry = is_bit_set(shift_v - 1, value) };
        case 32:
            return (struct ShiftResult){ .value = value, .carry = is_bit_set(31, value) };
        default: // todo: remove uneeded recursion
            return barrel_shift_ROR(value, shift_v - 32, old_carry);
    }
}

#ifdef __cplusplus
}
#endif
