#include "core/arm7tdmi/arm7tdmi.h"
#include "core/arm7tdmi/arm/encoding_types.h"
#include "core/arm7tdmi/arm/instructions/data_processing_common.h"

#include <stdio.h>
#include <assert.h>


// actual instruction
static inline void ARM_instruction_SUB(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t result = CPU.registers[format->rn] - shift->value;
    _update_sub_flags(gba, CPU.registers[format->rn], shift->value, result);
    CPU.registers[format->rd] = result;
}

static inline void ARM_instruction_RSB(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t result = shift->value - CPU.registers[format->rn];
    _update_sub_flags(gba, shift->value, CPU.registers[format->rn], result);
    CPU.registers[format->rd] = result;
}

static inline void ARM_instruction_ADD(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t result = CPU.registers[format->rn] + shift->value;
    _update_add_flags(gba, CPU.registers[format->rn], shift->value, result);
    CPU.registers[format->rd] = result;
}

static inline void ARM_instruction_ADC(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t op2 = shift->value + CPU.cpsr.C;
    const uint32_t result = CPU.registers[format->rn] + op2;
    _update_add_flags(gba, CPU.registers[format->rn], op2, result);
    CPU.registers[format->rd] = result;
}

static inline void ARM_instruction_SBC(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t op2 = shift->value + CPU.cpsr.C;
    const uint32_t result = CPU.registers[format->rn] - op2;
    _update_sub_flags(gba, CPU.registers[format->rn], op2, result);
    CPU.registers[format->rd] = result;
}

static inline void ARM_instruction_RSC(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t result = shift->value + CPU.registers[format->rn];
    _update_sub_flags(gba, shift->value, CPU.registers[format->rn], result);
    CPU.registers[format->rd] = result;
}

static inline void ARM_instruction_CMP(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t result = CPU.registers[format->rn] - shift->value;
    _update_sub_flags(gba, CPU.registers[format->rn], shift->value, result);
}

static inline void ARM_instruction_CMN(struct GBA_Core* gba, const struct Format_4_5* format, const struct ShiftResult* shift) {
    const uint32_t result = CPU.registers[format->rn] + shift->value;
    _update_add_flags(gba, CPU.registers[format->rn], shift->value, result);
}


// sub
void ARM_instruction_SUB_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_SUB(gba, &format, &shift);
}

void ARM_instruction_SUB_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_SUB(gba, &format, &shift);
}

void ARM_instruction_SUB_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_SUB(gba, &format, &shift);
}

//
void ARM_instruction_RSB_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_RSB(gba, &format, &shift);
}

void ARM_instruction_RSB_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_RSB(gba, &format, &shift);
}

void ARM_instruction_RSB_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_RSB(gba, &format, &shift);
}

//
void ARM_instruction_ADD_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_ADD(gba, &format, &shift);
}

void ARM_instruction_ADD_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_ADD(gba, &format, &shift);
}

void ARM_instruction_ADD_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_ADD(gba, &format, &shift);
}

//
void ARM_instruction_ADC_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_ADC(gba, &format, &shift);
}

void ARM_instruction_ADC_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_ADC(gba, &format, &shift);
}

void ARM_instruction_ADC_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_ADC(gba, &format, &shift);
}

//
void ARM_instruction_SBC_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_SBC(gba, &format, &shift);
}

void ARM_instruction_SBC_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_SBC(gba, &format, &shift);
}

void ARM_instruction_SBC_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_SBC(gba, &format, &shift);
}

//
void ARM_instruction_RSC_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_RSC(gba, &format, &shift);
}

void ARM_instruction_RSC_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_RSC(gba, &format, &shift);
}

void ARM_instruction_RSC_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_RSC(gba, &format, &shift);
}

//
void ARM_instruction_CMP_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_CMP(gba, &format, &shift);
}

void ARM_instruction_CMP_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_CMP(gba, &format, &shift);
}

void ARM_instruction_CMP_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_CMP(gba, &format, &shift);
}

//
void ARM_instruction_CMN_reg_rs(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_rs(opcode);
    const struct ShiftResult shift = _reg_rs_shift(gba, &format);
    ARM_instruction_CMN(gba, &format, &shift);
}

void ARM_instruction_CMN_reg_shift(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_reg_shift(opcode);
    const struct ShiftResult shift = _reg_shift(gba, &format);
    ARM_instruction_CMN(gba, &format, &shift);
}

void ARM_instruction_CMN_imm(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_5 format = gen_Format_4_5_imm(opcode);
    const struct ShiftResult shift = _imm_shift(gba, &format);
    ARM_instruction_CMN(gba, &format, &shift);
}
