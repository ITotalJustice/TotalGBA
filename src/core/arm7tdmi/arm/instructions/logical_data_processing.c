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

static inline struct ShiftResult _imm_shift(const struct GBA_Core* gba, const struct Format_4_5* format) {
    return barrel_shift_ROR(
        format->operand2.imm.imm,
        format->operand2.imm.rotate,
        ARM7_get_flag(gba, FLAG_C)
    );
}

static inline struct ShiftResult _reg_shift(const struct GBA_Core* gba, const struct Format_4_5* format) {
    return barrel_shift(
        format->operand2.reg.shift_type,
        CPU.registers[format->operand2.reg.rm],
        format->operand2.reg.shift_amount,
        ARM7_get_flag(gba, FLAG_C)
    );
}

static inline struct ShiftResult _reg_rs_shift(const struct GBA_Core* gba, const struct Format_4_5* format) {
    return barrel_shift(
        format->operand2.reg.shift_type,
        CPU.registers[format->operand2.reg.rm],
        CPU.registers[format->operand2.reg.rs] & 0xFF,
        ARM7_get_flag(gba, FLAG_C)
    );
}

// actual instructions
static inline void ARM_instruction_AND(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    CPU.registers[format->rd] = CPU.registers[format->rn] & shift->value;
    _update_logical_flags(gba, CPU.registers[format->rd], format->s, shift->carry);
}

static inline void ARM_instruction_EOR(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    CPU.registers[format->rd] = CPU.registers[format->rn] ^ shift->value;
    _update_logical_flags(gba, CPU.registers[format->rd], format->s, shift->carry);
}

static inline void ARM_instruction_ORR(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    CPU.registers[format->rd] = CPU.registers[format->rn] | shift->value;
    _update_logical_flags(gba, CPU.registers[format->rd], format->s, shift->carry);
}

static inline void ARM_instruction_TST(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t result = CPU.registers[format->rn] & shift->value;
    _update_logical_flags(gba, result, format->s, shift->carry);
}

static inline void ARM_instruction_TEQ(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t result = CPU.registers[format->rn] ^ shift->value;
    _update_logical_flags(gba, result, format->s, shift->carry);
}

static inline void ARM_instruction_BIC(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    CPU.registers[format->rd] = CPU.registers[format->rn] & (~shift->value);
    _update_logical_flags(gba, CPU.registers[format->rd], format->s, shift->carry);
}

static inline void ARM_instruction_MOV(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    CPU.registers[format->rd] = shift->value;
    _update_logical_flags(gba, CPU.registers[format->rd], format->s, shift->carry);
}

static inline void ARM_instruction_MVN(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    CPU.registers[format->rd] = ~shift->value;
    _update_logical_flags(gba, CPU.registers[format->rd], format->s, shift->carry);
}


// AND
void ARM_instruction_AND_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_AND(gba, &format, &shift);
}

void ARM_instruction_AND_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_AND(gba, &format, &shift);
}

void ARM_instruction_AND_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_AND(gba, &format, &shift);
}

// EOR
void ARM_instruction_EOR_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_EOR(gba, &format, &shift);
}

void ARM_instruction_EOR_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_EOR(gba, &format, &shift);
}

void ARM_instruction_EOR_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_EOR(gba, &format, &shift);
}

// TST
void ARM_instruction_TST_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_TST(gba, &format, &shift);
}

void ARM_instruction_TST_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_TST(gba, &format, &shift);
}

void ARM_instruction_TST_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_TST(gba, &format, &shift);
}

// TEQ
void ARM_instruction_TEQ_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_TEQ(gba, &format, &shift);
}

void ARM_instruction_TEQ_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_TEQ(gba, &format, &shift);
}

void ARM_instruction_TEQ_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_TEQ(gba, &format, &shift);
}

// ORR
void ARM_instruction_ORR_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_ORR(gba, &format, &shift);
}

void ARM_instruction_ORR_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_ORR(gba, &format, &shift);
}

void ARM_instruction_ORR_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_ORR(gba, &format, &shift);
}

// MOV
void ARM_instruction_MOV_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_MOV(gba, &format, &shift);
}

void ARM_instruction_MOV_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_MOV(gba, &format, &shift);
}

void ARM_instruction_MOV_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_MOV(gba, &format, &shift);
}

// BIC
void ARM_instruction_BIC_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_BIC(gba, &format, &shift);
}

void ARM_instruction_BIC_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_BIC(gba, &format, &shift);
}

void ARM_instruction_BIC_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_BIC(gba, &format, &shift);
}

// MVN
void ARM_instruction_MVN_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_MVN(gba, &format, &shift);
}

void ARM_instruction_MVN_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_MVN(gba, &format, &shift);
}

void ARM_instruction_MVN_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_MVN(gba, &format, &shift);
}
