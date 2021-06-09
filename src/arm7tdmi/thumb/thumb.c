#include "thumb.h"
#include "instructions.h"
#include "arm7tdmi/arm7tdmi.h"
#include "internal.h"


static inline uint16_t thumb_fetch(struct GBA_Core* gba);
static inline void thumb_decode(struct GBA_Core* gba, uint16_t opcode);


void thumb_execute(struct GBA_Core* gba)
{
    thumb_decode(gba, thumb_fetch(gba));
}

uint16_t thumb_fetch(struct GBA_Core* gba)
{
    const uint16_t opcode = GBA_read16(gba, CPU.registers[REG_PC_INDEX]);

    CPU.registers[REG_PC_INDEX] += sizeof(uint16_t);
    
    return opcode;
}

void thumb_decode(struct GBA_Core* gba, uint16_t opcode)
{
    // if (CPU.registers[REG_PC_INDEX] == 0x080041D4)
    // {
    //     GBA_log("\top: 0x%03X\n", (opcode >> 6) & 0x3FF);
    // }

    switch ((opcode >> 6) & 0x3FF)
    {
    // [5.1]
        // LSL Rd, Rs, #Offset5
        case 0x000 ... 0x01F:
            thumb_lsl(gba, opcode);
            break;

        // LSR Rd, Rs, #Offset5
        case 0x020 ... 0x03F:
            thumb_lsr(gba, opcode);
            break;

        // ASR Rd, Rs, #Offset5
        case 0x040 ... 0x05F:
            thumb_asr(gba, opcode);
            break;
    
    // [5.2]
        // ADD Rd, Rs, Rn
        case 0x060 ... 0x067:
            thumb_add_reg(gba, opcode);
            break;
        
        // ADD Rd, Rs, #Offset3
        case 0x070 ... 0x077:
            GBA_log("[THUMB-ADD] 0x%03X\n", 0x070); goto unk;
            break;

        // SUB Rd, Rs, Rn
        case 0x068 ... 0x06F:
            thumb_sub_reg(gba, opcode);
            break;
        
        // SUB Rd, Rs, #Offset3
        case 0x078 ... 0x07F:
            GBA_log("[THUMB-SUB] 0x%03X\n", 0x078); goto unk;
            break;

    // [5.3]
        // MOV Rd, #Offset8
        case 0x080 ... 0x09F:
            thumb_mov_imm(gba, opcode);
            break;

        // CMP Rd, #Offset8
        case 0x0A0 ... 0x0BF:
            thumb_cmp_imm(gba, opcode);
            break;

        // ADD Rd, #Offset8
        case 0x0C0 ... 0x0DF:
            thumb_add_imm(gba, opcode);
            break;

        // SUB Rd, #Offset8
        case 0x0E0 ... 0x0FF:
            thumb_sub_imm(gba, opcode);
            break;
    
    // [5.4]
        // AND Rd, Rs
        case 0x100:
            GBA_log("[THUMB-AND] 0x%03X\n", 0x100); goto unk;
            break;

        // EOR Rd, Rs
        case 0x101:
            GBA_log("[THUMB-EOR] 0x%03X\n", 0x101); goto unk;
            break;

        // LSL Rd, Rs
        case 0x102:
            GBA_log("[THUMB-LSL] 0x%03X\n", 0x102); goto unk;
            break;

        // LSR Rd, Rs
        case 0x103:
            GBA_log("[THUMB-LSR] 0x%03X\n", 0x103); goto unk;
            break;
    
        // ASR Rd, Rs
        case 0x104:
            GBA_log("[THUMB-ASR] 0x%03X\n", 0x104); goto unk;
            break;

        // ADC Rd, Rs
        case 0x105:
            GBA_log("[THUMB-ADC] 0x%03X\n", 0x105); goto unk;
            break;

        // SBC Rd, Rs
        case 0x106:
            GBA_log("[THUMB-SBC] 0x%03X\n", 0x106); goto unk;
            break;

        // ROR Rd, Rs
        case 0x107:
            GBA_log("[THUMB-ROR] 0x%03X\n", 0x107); goto unk;
            break;

        // TST Rd, Rs
        case 0x108:
            GBA_log("[THUMB-TST] 0x%03X\n", 0x108); goto unk;
            break;
        
        // NEG Rd, Rs
        case 0x109:
            GBA_log("[THUMB-NEG] 0x%03X\n", 0x109); goto unk;
            break;

        // CMP Rd, Rs
        case 0x10A:
            // GBA_log("[THUMB-CMP] 0x%03X\n", 0x10A); goto unk;
            thumb_cmp_rd_rs(gba, opcode);
            break;

        // CMN Rd, Rs
        case 0x10B:
            GBA_log("[THUMB-CMN] 0x%03X\n", 0x10B); goto unk;
            break;

        // ORR Rd, Rs
        case 0x10C:
            GBA_log("[THUMB-ORR] 0x%03X\n", 0x10C); goto unk;
            break;

        // MUL Rd, Rs
        case 0x10D:
            GBA_log("[THUMB-MUL] 0x%03X\n", 0x10D); goto unk;
            break;

        // BIC Rd, Rs
        case 0x10E:
            thumb_bic(gba, opcode);
            break;

        // MVN Rd, Rs
        case 0x10F:
            GBA_log("[THUMB-MVN] 0x%03X\n", 0x10F); goto unk;
            break;
    
    // [5.5]
        // ADD Rd, Hs
        case 0x111:
            GBA_log("[THUMB-ADD] 0x%03X\n", 0x111); goto unk;
            break;

        // ADD Hd, Rs
        case 0x112:
            GBA_log("[THUMB-ADD] 0x%03X\n", 0x112); goto unk;
            break;

        // ADD Hd, Hs
        case 0x113:
            GBA_log("[THUMB-ADD] 0x%03X\n", 0x113); goto unk;
            break;

        // CMP Rd, Hs
        case 0x115:
            GBA_log("[THUMB-CMP] 0x%03X\n", 0x115); goto unk;
            break;

        // CMP Hd, Rs
        case 0x116:
            GBA_log("[THUMB-CMP] 0x%03X\n", 0x116); goto unk;
            break;

        // CMP Hd, Hs
        case 0x117:
            GBA_log("[THUMB-CMP] 0x%03X\n", 0x117); goto unk;
            break;

        // MOV Rd, Hs
        case 0x119:
            GBA_log("[THUMB-MOV] 0x%03X\n", 0x119); goto unk;
            break;

        // MOV Hd, Rs
        case 0x11A:
            thumb_mov_hd_rs(gba, opcode);
            break;

        // MOV Hd, Hs
        case 0x11B:
            thumb_mov_hd_hs(gba, opcode);
            break;

        // BX Rs
        case 0x11C:
            thumb_bx_rs(gba, opcode);
            break;

        // BX Hs
        case 0x11D:
            thumb_bx_hs(gba, opcode);
            break;

    // [5.6]
        // LDR Rd, [PC, #Imm]
        case 0x120 ... 0x13F:
            thumb_pc_relative_load(gba, opcode);
            break;

    // [5.7]
        // STR Rd, [Rb, Ro]
        case 0x140 ... 0x147:
            GBA_log("[THUMB-STR] 0x%03X\n", 0x140); goto unk;
            break;

        // STRB Rd, [Rb, Ro]
        case 0x150 ... 0x157:
            GBA_log("[THUMB-STRB] 0x%03X\n", 0x150); goto unk;
            break;
    
        // LDR Rd [Rb, Ro]
        case 0x160 ... 0x167:
            GBA_log("[THUMB-LDR] 0x%03X\n", 0x160); goto unk;
            break;
    
        // LDRB Rd [Rb, Ro]
        case 0x170 ... 0x177:
            GBA_log("[THUMB-LDRB] 0x%03X\n", 0x170); goto unk;
            break;
    
    // [5.8]
        // STRH Rd, [Rb, Ro] 
        case 0x148 ... 0x14F:
            GBA_log("[THUMB-STRH] 0x%03X\n", 0x148); goto unk;
            break;

        // LDRH Rd, [Rb, Ro]
        case 0x158 ... 0x15F:
            GBA_log("[THUMB-LDRH] 0x%03X\n", 0x158); goto unk;
            break;

        // LDSB Rd, [Rb, Ro]
        case 0x168 ... 0x16F:
            GBA_log("[THUMB-LDSB] 0x%03X\n", 0x168); goto unk;
            break;
    
        // LDSH Rd, [Rb, Ro]
        case 0x178 ... 0x17F:
            GBA_log("[THUMB-LDSH] 0x%03X\n", 0x178); goto unk;
            break;
    
    // [5.9]
        // STR Rd, [Rb, #Imm]
        case 0x180 ... 0x19F:
            thumb_str_imm(gba, opcode);
            break;
        
        // STRB Rd, [Rb, #Imm]
        case 0x1C0 ... 0x1DF:
            GBA_log("[THUMB-STRB] 0x%03X\n", 0x1C0); goto unk;
            break;

        // LDR Rd, [Rb, #Imm]
        case 0x1A0 ... 0x1BF:
            GBA_log("[THUMB-LDR] 0x%03X\n", 0x1A0); goto unk;
            break;

        // LDRB Rd, [Rb, #Imm]
        case 0x1E0 ... 0x1FF:
            GBA_log("[THUMB-LDRB] 0x%03X\n", 0x1E0); goto unk;
            break;
    
    // [5.10]
        // STRH Rd, [Rb, #Imm]
        case 0x200 ... 0x21F:
            GBA_log("[THUMB-STRH] 0x%03X\n", 0x200); goto unk;
            break;

        // LDRH Rd, [Rb, #Imm]
        case 0x220 ... 0x23F:
            GBA_log("[THUMB-LDRH] 0x%03X\n", 0x220); goto unk;
            break;
    
    // [5.11]
        // STR Rd, [SP, #Imm]
        case 0x240 ... 0x25F:
            GBA_log("[THUMB-STR] 0x%03X\n", 0x240); goto unk;
            break;   
        
        // LDR Rd, [SP, #Imm]
        case 0x260 ... 0x27F:
            GBA_log("[THUMB-LDR] 0x%03X\n", 0x260); goto unk;
            break;

    // [5.12]
        // ADD Rd, PC, #Imm
        case 0x280 ... 0x29F:
            GBA_log("[THUMB-ADD] 0x%03X\n", 0x280); goto unk;
            break;   
        
        // ADD Rd, SP, #Imm
        case 0x2A0 ... 0x2BF:
            GBA_log("[THUMB-ADD] 0x%03X\n", 0x2A0); goto unk;
            break;   
    
    // [5.13]
        // ADD SP, #Imm
        case 0x2C0 ... 0x2C1:
            GBA_log("[THUMB-ADD] 0x%03X\n", 0x2C0); goto unk;
            break;
        
        // ADD SP, #-Imm
        case 0x2C2 ... 0x2C3:
            GBA_log("[THUMB-ADD] 0x%03X\n", 0x2C2); goto unk;
            break;

    // [5.14]
        // PUSH { Rlist }
        case 0x2D0 ... 0x2D3:
            GBA_log("[THUMB-PUSH] 0x%03X\n", 0x2D0); goto unk;
            break;

        // PUSH { Rlist, LR }
        case 0x2D4 ... 0x2D7:
            thumb_push(gba, opcode);
            break;

        // POP { Rlist }
        case 0x2F0 ... 0x2F3:
            thumb_pop(gba, opcode);
            break;

        // POP { Rlist, PC }
        case 0x2F4 ... 0x2F7:
            GBA_log("[THUMB-POP] 0x%03X\n", 0x2F4); goto unk;
            break;
    
    // [5.15]
        // STMIA Rb!, { Rlist }
        case 0x300 ... 0x31F:
            thumb_stmia(gba, opcode);
            break;

        // LDMIA Rb!, { Rlist }
        case 0x320 ... 0x33F:
            thumb_ldmia(gba, opcode);
            break;
    
    // [5.16]
        // [COND] branch
        case 0x340 ... 0x377:
            thumb_cond(gba, opcode);
            break;

    // [5.17]
        // SWI Value8
        case 0x37C ... 0x37F:
            GBA_log("[THUMB-SWI] 0x%03X\n", 0x37C); goto unk;
            break;
    
    // [5.18]
        // B (unconditional branch)
        case 0x380 ... 0x39F:
            GBA_log("[THUMB-B] 0x%03X\n", 0x380); goto unk;
            break;

    // [5.19]
        // BL (offset high)
        case 0x3C0 ... 0x3DF:
            thumb_bl_high(gba, opcode);
            break;

        // BL (offset low)
        case 0x3E0 ... 0x3FF:
            thumb_bl_low(gba, opcode);
            break;

        default:
            goto unk;
            break;
    }

    return;

unk:
    GBA_log_fatal("unk THUMB instruction! 0x%03X DEC: 0x%03X\n", opcode, (opcode >> 6) & 0x3FF);
}
