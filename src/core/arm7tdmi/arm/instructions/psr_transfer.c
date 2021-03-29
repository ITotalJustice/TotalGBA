#include "core/arm7tdmi/arm7tdmi.h"
#include "core/arm7tdmi/arm/encoding_types.h"

#include <stdio.h>
#include <assert.h>


static inline void _psr_to_u32(const struct GBA_Psr psr, uint32_t* reg) {
    *reg = (psr.N << 31) | (psr.Z << 30) | (psr.C << 29) | (psr.V << 28) |
        (psr.I << 7) | (psr.F << 6) | (psr.T << 5) | (psr.M);
}

static inline void _u32_to_psr(struct GBA_Psr* psr, const uint32_t reg) {
    psr->N = is_bit_set(31, reg);
    psr->Z = is_bit_set(30, reg);
    psr->C = is_bit_set(29, reg);
    psr->V = is_bit_set(28, reg);
    psr->I = is_bit_set(7, reg);
    psr->F = is_bit_set(6, reg);
    psr->T = is_bit_set(5, reg);
    psr->M = get_bit_range(0, 4, reg);
}

static inline void _u32_to_psr_flags_only(struct GBA_Psr* psr, const uint32_t reg) {
    psr->N = is_bit_set(31, reg);
    psr->Z = is_bit_set(30, reg);
    psr->C = is_bit_set(29, reg);
    psr->V = is_bit_set(28, reg);
}

// transfer psr to register
void ARM_instruction_MRS_CPSR(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_6 format = gen_Format_4_6_MRS(opcode);
    _psr_to_u32(CPU.cpsr, &CPU.registers[format.rd]);
}

void ARM_instruction_MRS_SPSR(struct GBA_Core* gba, uint32_t opcode) {
    assert(ARM7_get_mode(gba) != MODE_USER);

    const struct Format_4_6 format = gen_Format_4_6_MRS(opcode);
    _psr_to_u32(CPU.spsr, &CPU.registers[format.rd]);
}


// transfer register to psr
void ARM_instruction_MSR_CPSR(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_6 format = gen_Format_4_6_MSR(opcode);
    _u32_to_psr(&CPU.cpsr, CPU.registers[format.rm]);
}

void ARM_instruction_MSR_SPSR(struct GBA_Core* gba, uint32_t opcode) {
    assert(ARM7_get_mode(gba) != MODE_USER);

    const struct Format_4_6 format = gen_Format_4_6_MSR(opcode);
    _u32_to_psr(&CPU.spsr, CPU.registers[format.rm]);
}


// transfer register / imm to psr flags only
void ARM_instruction_MSR_CPSR_reg_flags(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_6 format = gen_Format_4_6_MSR_reg_flags(opcode);
    _u32_to_psr(&CPU.cpsr, CPU.registers[format.rm]);
}

void ARM_instruction_MSR_CPSR_imm_flags(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_6 format = gen_Format_4_6_MSR_imm_flags(opcode);
    const uint32_t rotated_imm = bit_rotr(format.rotate, format.imm);
    _u32_to_psr(&CPU.cpsr, rotated_imm);
}

void ARM_instruction_MSR_SPSR_reg_flags(struct GBA_Core* gba, uint32_t opcode) {
    assert(ARM7_get_mode(gba) != MODE_USER);
    
    const struct Format_4_6 format = gen_Format_4_6_MSR_reg_flags(opcode);
    _u32_to_psr_flags_only(&CPU.spsr, CPU.registers[format.rm]);
}

void ARM_instruction_MSR_SPSR_imm_flags(struct GBA_Core* gba, uint32_t opcode) {
    assert(ARM7_get_mode(gba) != MODE_USER);

    const struct Format_4_6 format = gen_Format_4_6_MSR_imm_flags(opcode);
    const uint32_t rotated_imm = bit_rotr(format.rotate, format.imm);
    _u32_to_psr(&CPU.spsr, rotated_imm);
}
