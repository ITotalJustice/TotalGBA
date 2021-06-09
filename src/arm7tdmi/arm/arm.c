#include "arm.h"
#include "block_data.h"
#include "branch.h"
#include "data_processing.h"
#include "halfword_and_signed_data.h"
#include "psr_transfer.h"
#include "single_data.h"
#include "arm7tdmi/arm7tdmi.h"
#include "internal.h"


static inline uint32_t arm_fetch(struct GBA_Core* gba);
static inline void arm_decode(struct GBA_Core* gba, uint32_t opcode);


void arm_execute(struct GBA_Core* gba)
{
    const uint32_t opcode = arm_fetch(gba);

    if (ARM7_get_cond(gba, opcode >> 28))
    {
        arm_decode(gba, opcode);
    }
    else
    {
        GBA_log("skipping instruction! 0x%08X\n", opcode);
    }
}

uint32_t arm_fetch(struct GBA_Core* gba)
{
    const uint32_t opcode = GBA_read32(gba, CPU.registers[REG_PC_INDEX]);
    CPU.registers[REG_PC_INDEX] += sizeof(uint32_t);
    
    return opcode;
}

void arm_decode(struct GBA_Core* gba, uint32_t opcode)
{
    switch (((opcode >> 16) & 0xFF0) | ((opcode >> 4) & 0xF))
    {
    // [4.10]
        case 0x0CB: case 0x1DB:
            arm_strh_imm(gba, opcode);
            break;

    // []
        case 0x120:
            arm_msr(gba, opcode); // cpsr
            // arm_msr_cpsr(gba, opcode);
            break;
    // [4.3]
        case 0x121:
            arm_bx(gba, opcode);
            break;

        case 0x280 ... 0x29F:
            arm_add_imm(gba, opcode);
            break;

        case 0x3A0 ... 0x3BF:
            arm_mov_imm(gba, opcode);
            break;

        case 0x400 ... 0x5FF:
            arm_single_data_imm(gba, opcode);
            break;

        case 0x920:
            arm_stm(gba, opcode);
            break;

    // [4.4]
        case 0xA00 ... 0xAFF:
            arm_branch(gba, opcode);
            break;

        case 0xB00 ... 0xBFF:
            arm_branch_link(gba, opcode);
            break;

        default:
           GBA_log_fatal("unk ARM instruction! 0x%08X DEC: 0x%03X\n", opcode, ((opcode >> 16) & 0xFF0) | ((opcode >> 4) & 0xF));
           break; 
    }
}
