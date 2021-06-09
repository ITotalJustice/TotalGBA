#include "psr_transfer.h"
#include "arm7tdmi/arm7tdmi.h"
#include "internal.h"
#include "util/bit.h"


void arm_msr(struct GBA_Core* gba, uint32_t opcode)
{
    const uint8_t rm = get_bit_range(0, 3, opcode);

    R15_CHECK(rm);

    ARM7_u32_to_psr(&CPU.cpsr, CPU.registers[rm]);
}
