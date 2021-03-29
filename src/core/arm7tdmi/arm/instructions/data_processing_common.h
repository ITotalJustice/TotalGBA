#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/arm7tdmi/arm7tdmi.h"
#include "core/arm7tdmi/arm/encoding_types.h"


static inline struct ShiftResult _imm_shift(const struct GBA_Core* gba, const struct Format_4_5* format) {
    return barrel_shift_ROR(
        format->operand2.imm.imm,
        format->operand2.imm.rotate,
        ARM7_get_flag(gba, FLAG_C)
    );
}

static inline struct ShiftResult _reg_shift(const struct GBA_Core* gba, const struct Format_4_5* format) {
    return barrel_shift(
        (enum ShiftType)format->operand2.reg.shift_type,
        CPU.registers[format->operand2.reg.rm],
        format->operand2.reg.shift_amount,
        ARM7_get_flag(gba, FLAG_C)
    );
}

static inline struct ShiftResult _reg_rs_shift(const struct GBA_Core* gba, const struct Format_4_5* format) {
    return barrel_shift(
        (enum ShiftType)format->operand2.reg.shift_type,
        CPU.registers[format->operand2.reg.rm],
        CPU.registers[format->operand2.reg.rs] & 0xFF,
        ARM7_get_flag(gba, FLAG_C)
    );
}

static inline bool calc_v_flag(const uint32_t a, const uint32_t b, const uint32_t r) {
    return (is_bit_set(31, a) == is_bit_set(31, b)) && (is_bit_set(31, a) != is_bit_set(31, r));
}

static inline void _update_add_flags(struct GBA_Core* gba, const uint32_t a, const uint32_t b, const uint32_t r) {
    ARM7_set_flags_nzcv(gba,
        is_bit_set(31, r),
        r == 0,
        (a + b) > 0xFFFFFFFF,
        calc_v_flag(a, b, r)
    );
}

static inline void _update_sub_flags(struct GBA_Core* gba, const uint32_t a, const uint32_t b, const uint32_t r) {
    ARM7_set_flags_nzcv(gba,
        is_bit_set(31, r),
        r == 0,
        a < b,
        calc_v_flag(a, b, r)
    );
}

static inline void _update_logical_flags(struct GBA_Core* gba, const uint32_t result, const bool sbit, const bool carry) {
    if (sbit == true) {
        ARM7_set_flags_nzc(gba,
            is_bit_set(31, result),
            result == 0,
            carry 
        );
    }
}

#ifdef __cplusplus
}
#endif

