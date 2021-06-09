#include "barrel_shifter.h"
#include "internal.h"


struct ShiftResult barrel_shift_lsl(const uint32_t value, const uint32_t shift_v, const bool old_carry)
{
    if (shift_v == 0)
    {
        return (struct ShiftResult)
        {
            .value = value,
            .carry = old_carry
        };
    }
    else if (shift_v >= 1 && shift_v <= 31)
    {
        return (struct ShiftResult)
        {
            .value = value << shift_v,
            .carry = is_bit_set(32 - shift_v, value)
        };
    }
    else if (shift_v == 32)
    {
        return (struct ShiftResult)
        {
            .value = 0,
            .carry = is_bit_set(0, value)
        };
    }
    else
    {
        return (struct ShiftResult)
        {
            .value = 0,
            .carry = false
        };
    }
}

struct ShiftResult barrel_shift_lsr(const uint32_t value, const uint32_t shift_v, const bool old_carry)
{
    if (shift_v == 0)
    {
        return (struct ShiftResult)
        {
            .value = value,
            .carry = old_carry
        };
    }
    
    else if (shift_v >= 1 && shift_v <= 31)
    {
        return (struct ShiftResult)
        {
            .value = value >> shift_v,
            .carry = is_bit_set(shift_v, value)
        };
    }
    else if (shift_v == 32)
    {
        return (struct ShiftResult)
        {
            .value = 0,
            .carry = is_bit_set(31, value)
        };
    }
    else
    {
        return (struct ShiftResult)
        {
            .value = 0,
            .carry = false
        };
    }
}

struct ShiftResult barrel_shift_asr(const uint32_t value, const uint32_t shift_v, const bool old_carry)
{
    if (shift_v == 0)
    {
        return (struct ShiftResult)
        {
            .value = value,
            .carry = old_carry
        };
    }
    else if (shift_v >= 1 && shift_v <= 31)
    {
        return (struct ShiftResult)
        {
            .value = (uint32_t)((int32_t)(value) >> shift_v),
            .carry = is_bit_set(shift_v - 1, value)
        };
    }
    else
    {
        return (struct ShiftResult)
        {
            .value = (uint32_t)((int32_t)(value) >> 31),
            .carry = is_bit_set(31, value)
        };
    }
}

struct ShiftResult barrel_shift_ror(const uint32_t value, const uint32_t shift_v, const bool old_carry)
{
    if (shift_v == 0)
    {
        return (struct ShiftResult)
        {
            .value = value,
            .carry = old_carry
        };
    }
    else if (shift_v >= 1 && shift_v <= 31)
    {
        return (struct ShiftResult)
        {
            .value = bit_rotr(shift_v, value),
            .carry = is_bit_set(shift_v - 1, value)
        };
    }
    else if (shift_v == 32)
    {
        return (struct ShiftResult)
        {
            .value = value,
            .carry = is_bit_set(31, value)
        };
    }

    else
    {
        // todo: remove uneeded recursion
        return barrel_shift_ror(value, shift_v - 32, old_carry);
    }
}

struct ShiftResult barrel_shift(const enum ShiftType type, const uint32_t value, const uint32_t shift_v, const bool old_carry)
{
    switch (type)
    {
        case LSL: return barrel_shift_lsl(value, shift_v, old_carry);
        case LSR: return barrel_shift_lsr(value, shift_v, old_carry);
        case ASR: return barrel_shift_asr(value, shift_v, old_carry);
        case ROR: return barrel_shift_ror(value, shift_v, old_carry); 
    }

    UNREACHABLE((struct ShiftResult){});
}
