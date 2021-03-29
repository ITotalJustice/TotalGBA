#include "core/arm7tdmi/arm7tdmi.h"
#include "core/arm7tdmi/arm/encoding_types.h"

#include <stdio.h>
#include <assert.h>


static inline void _update_logical_flags(struct GBA_Core* gba, const uint32_t result, const bool sbit, const bool carry) {
    if (sbit == true) {
        ARM7_set_flags_nzc(gba,
            is_bit_set(31, result),
            result == 0,
            carry 
        );
    }
}

static inline struct ShiftResult _logical_imm_shift(const struct GBA_Core* gba, const struct Format_4_5* format) {
    return barrel_shift_ROR(
        format->operand2.imm.imm,
        format->operand2.imm.rotate,
        ARM7_get_flag(gba, FLAG_C)
    );
}

void ARM_instruction_MOV_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _logical_imm_shift(gba, &format);
    
    CPU.registers[format.rd] = shift.value;

    _update_logical_flags(gba,
        CPU.registers[format.rd],
        format.s,
        shift.carry
    );
}
