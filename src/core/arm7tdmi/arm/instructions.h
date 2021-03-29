#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"

// [4.4]
void ARM_instruction_B(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_BL(struct GBA_Core* gba, uint32_t opcode);

// [4.5]
void ARM_instruction_MOV_imm(struct GBA_Core* gba, uint32_t opcode);

// [4.6]
void ARM_instruction_MRS_CPSR(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_MRS_SPSR(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_MSR_CPSR(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_MSR_SPSR(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_MSR_CPSR_reg_flags(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_MSR_CPSR_imm_flags(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_MSR_SPSR_reg_flags(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_MSR_SPSR_imm_flags(struct GBA_Core* gba, uint32_t opcode);

// [4.9]
void ARM_instruction_single_data_transfer_imm(struct GBA_Core* gba, uint32_t opcode);
void ARM_instruction_single_data_transfer_reg(struct GBA_Core* gba, uint32_t opcode);

// UB
void ARM_instruction_UNDEFINED(struct GBA_Core* gba, uint32_t opcode);

// DEBUG
void ARM_instruction_not_implemented(struct GBA_Core* gba, uint32_t opcode);

#ifdef __cplusplus
}
#endif