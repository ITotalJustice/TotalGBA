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

    // [4.5]
        // MOV Logical Left
        case 0x1A0: case 0x1A1: case 0x1A8: case 0x1B0: case 0x1B1: case 0x1B8:
            arm_mov_logical_left(arm, opcode);
            break;

        // // MOV Logical Right
        // case 0x1A2: case 0x1A3: case 0x1AA: case 0x1B2: case 0x1B3: case 0x1BA:
        //     break;

        // // MOV Arithmetic Right
        // case 0x1A4: case 0x1A5: case 0x1AC: case 0x1B4: case 0x1B5: case 0x1BC:
        //     break;

        // // MOV Rotate Right
        // case 0x1A6: case 0x1A7: case 0x1AE: case 0x1B6: case 0x1B7: case 0x1BE:
            break;

        case 0x200 ... 0x21F:
            arm_and_imm(gba, opcode);
            break;

        case 0x280 ... 0x29F:
            arm_add_imm(gba, opcode);
            break;

        case 0x3A0 ... 0x3BF:
            arm_mov_imm(gba, opcode);
            break;

    // []
        case 0x400 ... 0x5FF:
            arm_single_data_imm(gba, opcode);
            break;

    // [4.11]
        case 0x800 ... 0x9FF:
            arm_stm_ldm(gba, opcode);
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
