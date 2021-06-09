#include "halfword_and_signed_data.h"
#include "arm7tdmi/arm7tdmi.h"
#include "internal.h"
#include "util/bit.h"


struct format4_10
{
    uint8_t off_lo;
    uint8_t off_hi;
    uint8_t rd;
    uint8_t rn;
    bool h;
    bool s;
    bool l;
    bool w;
    bool u;
    bool p;
};

static inline struct format4_10 gen_format4_10(const uint32_t opcode)
{
    return (struct format4_10)
    {
        .off_lo = get_bit_range(0, 3, opcode),
        .off_hi = get_bit_range(8, 11, opcode),
        .rd = get_bit_range(12, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .h = is_bit_set(5, opcode),
        .s = is_bit_set(6, opcode),
        .l = is_bit_set(20, opcode),
        .w = is_bit_set(21, opcode),
        .u = is_bit_set(23, opcode),
        .p = is_bit_set(24, opcode),
    };
}

static inline void arm_halfword_and_signed_data_transfer(struct GBA_Core* gba, uint32_t opcode, uint32_t offset)
{
    const struct format4_10 d = gen_format4_10(opcode);

    uint32_t addr = CPU.registers[d.rn];
    uint32_t final_addr = addr;

    if (d.rn == REG_PC_INDEX)
    {
        GBA_log("\tsingle data %s RN is r15!\n", d.l ? "LDR" : "STR");
        addr += 4;
        final_addr += 4;
    }
    if (d.rd == REG_PC_INDEX)
    {
        GBA_log("\tsingle data %s RD is r15!\n", d.l ? "LDR" : "STR");
        R15_CHECK(d.rd);
    }

    // if set, add offset to base, else subtract
    if (d.u)
    {
        final_addr += offset;
    }
    else
    {
        final_addr -= offset;
    }

    // if set, add the offset before transfer
    if (d.p)
    {
        addr = final_addr;
    }

    // if set, it's an LDR, else, STR
    if (d.l)
    {
        uint32_t result = 0;

        // if set, 16-bit transfer, else, 8-bit
        if (d.h)
        {
            result = GBA_read16(gba, addr);
        }
        else
        {
            result = GBA_read8(gba, addr);
        }

        // if set, this was a signed load
        if (d.s)
        {
            // we need to sign extend the result, if h, 16,31, else, 8,31
            if (d.h)
            {
                result = sign_extend(16, result);
            }
            else
            {
                result = sign_extend(8, result);
            }
        }

        CPU.registers[d.rd] = result;
    }
    else
    {
        // if set, 16-bit transfer, else, 8-bit
        if (d.h)
        {
            GBA_write16(gba, addr, CPU.registers[d.rd]);
        }
        else
        {
            GBA_write8(gba, addr, CPU.registers[d.rd]);
        }
    }

    // if set, write back the final_addr to rn
    // if p is unset (post) then 
    if (d.w || !d.p)
    {

        CPU.registers[d.rn] = final_addr;
    }
}

static inline void arm_half_signed_imm(struct GBA_Core* gba, uint32_t opcode)
{
    const uint8_t lo = get_bit_range(0, 3, opcode);
    const uint8_t hi = get_bit_range(8, 11, opcode);
    const uint8_t offset = (hi << 4) | lo;

    arm_halfword_and_signed_data_transfer(gba, opcode, offset);
}

void arm_ldrh_imm(struct GBA_Core* gba, uint32_t opcode)
{
    arm_half_signed_imm(gba, opcode);
}

void arm_strh_imm(struct GBA_Core* gba, uint32_t opcode)
{
    arm_half_signed_imm(gba, opcode);
}
