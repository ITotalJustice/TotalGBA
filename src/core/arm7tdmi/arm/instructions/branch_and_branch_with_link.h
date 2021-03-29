#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/arm7tdmi/arm7tdmi.h"
#include "core/arm7tdmi/arm/encoding_types.h"

#include <stdio.h>


static void ARM_instruction_B(struct GBA_Core* gba, uint32_t opcode) {
    printf("[ARM-FUNC] %s\n", __func__);
    const struct Format_B format = gen_Format_B(opcode);
    CPU.registers[REG_PC_INDEX] += format.offset;
}

static void ARM_instruction_BL(struct GBA_Core* gba, uint32_t opcode) {
    printf("[ARM-FUNC] %s\n", __func__);
    const struct Format_B format = gen_Format_B(opcode);
    // save PC in LR, but not the lower 2-bits
    CPU.registers[REG_LR_INDEX] = CPU.registers[REG_PC_INDEX] & ~(0x3);
    CPU.registers[REG_PC_INDEX] += format.offset;
}

#ifdef __cplusplus
}
#endif
