#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "util/bit.h"


enum ShiftType
{
    LSL,
    LSR,
    ASR,
    ROR, 
};

struct ShiftResult
{
    uint32_t value;
    bool carry;
};

GBA_INLINE struct ShiftResult barrel_shift_lsl(const uint32_t value, const uint32_t shift_v, const bool old_carry);
GBA_INLINE struct ShiftResult barrel_shift_lsr(const uint32_t value, const uint32_t shift_v, const bool old_carry);
GBA_INLINE struct ShiftResult barrel_shift_asr(const uint32_t value, const uint32_t shift_v, const bool old_carry);
GBA_INLINE struct ShiftResult barrel_shift_ror(const uint32_t value, const uint32_t shift_v, const bool old_carry);
GBA_INLINE struct ShiftResult barrel_shift(const enum ShiftType type, const uint32_t value, const uint32_t shift_v, const bool old_carry);

#ifdef __cplusplus
}
#endif
