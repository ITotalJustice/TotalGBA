#include "branch.h"
#include "arm7tdmi/arm7tdmi.h"
#include "internal.h"
#include "util/bit.h"


void arm_bx_generic(struct GBA_Core* gba, uint32_t addr)
{
    // set PC to the value of the reg
    CPU.registers[REG_PC_INDEX] = addr;
    // if bit-0 == 0, switch to arm, else thumb
    if (addr & 1)
    {
        GBA_log("[ARM-BX] switching to THUMB\n");
        ARM7_set_state(gba, addr & 1);
        CPU.registers[REG_PC_INDEX] &= ~0x1;
    }
    else
    {
        GBA_log("[ARM-BX] switching to ARM\n");
        ARM7_set_state(gba, addr & 1);
        CPU.registers[REG_PC_INDEX] &= ~0x3;
    }
}

void arm_bx(struct GBA_Core* gba, uint32_t opcode)
{
    const uint8_t rn = get_bit_range(0, 3, opcode);
    arm_bx_generic(gba, CPU.registers[rn]);
}


// [4.4]
void arm_branch(struct GBA_Core* gba, uint32_t opcode)
{
    const int32_t offset = sign_extend(25, opcode << 2);
    // PC + 4 because of prefetch
    CPU.registers[REG_PC_INDEX] += offset + 4;
}

void arm_branch_link(struct GBA_Core* gba, uint32_t opcode)
{
    const int32_t offset = sign_extend(25, opcode << 2);
    // save the PC (next instruction) to LR
    CPU.registers[REG_LR_INDEX] = CPU.registers[REG_PC_INDEX];
    // PC + 4 because of prefetch
    CPU.registers[REG_PC_INDEX] += offset + 4;
}
