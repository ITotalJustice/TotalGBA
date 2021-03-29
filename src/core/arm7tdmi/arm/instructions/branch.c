#include "core/arm7tdmi/arm7tdmi.h"
#include "core/arm7tdmi/arm/encoding_types.h"

#include <stdio.h>


// [4.3]
void ARM_instruction_BX(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_4_3 format = gen_Format_4_3(opcode);
    // set PC to the value of the reg
    CPU.registers[REG_PC_INDEX] = CPU.registers[format.rn];
    // if bit-0 == 0, switch to arm, else thumb
    ARM7_set_state(gba, CPU.registers[format.rn] & 1);
}


// [4.4]
void ARM_instruction_B(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_B format = gen_Format_B(opcode);
    // PC + 4 because of prefetch
    CPU.registers[REG_PC_INDEX] += format.offset + 4;
}

void ARM_instruction_BL(struct GBA_Core* gba, uint32_t opcode) {
    const struct Format_B format = gen_Format_B(opcode);
    // save PC in LR, but not the lower 2-bits
    CPU.registers[REG_LR_INDEX] = (CPU.registers[REG_PC_INDEX] + 4) & ~(0x3);
    // PC + 4 because of prefetch
    CPU.registers[REG_PC_INDEX] += format.offset + 4;
}
