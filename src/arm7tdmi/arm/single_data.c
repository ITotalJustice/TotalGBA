#include "single_data.h"
#include "arm7tdmi/arm7tdmi.h"
#include "internal.h"
#include "util/bit.h"


struct format4_9
{
    uint8_t rd;
    uint8_t rn;
    bool l;
    bool w;
    bool b;
    bool u;
    bool p;
};

static inline struct format4_9 gen_format4_9(const uint32_t opcode)
{
    return (struct format4_9)
    {
        .rd = get_bit_range(12, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .l = is_bit_set(20, opcode),
        .w = is_bit_set(21, opcode),
        .b = is_bit_set(22, opcode),
        .u = is_bit_set(23, opcode),
        .p = is_bit_set(24, opcode),
    };
}

static inline void arm_single_data(struct GBA_Core* gba, uint32_t opcode, uint32_t offset)
{
    const struct format4_9 d = gen_format4_9(opcode);

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

        // if set, 8-bit transfer, else, 32-bit
        if (d.b)
        {
            result = GBA_read8(gba, addr);
        }
        else
        {
            result = GBA_read32(gba, addr &~ 0x3);
            // the result is actually rotated if not word alligned!!!
            result = bit_rotr(addr & 0x3, result);
        }

        CPU.registers[d.rd] = result;
    }
    else
    {
        // if set, 8-bit transfer, else, 32-bit
        if (d.b)
        {
            GBA_write8(gba, addr, CPU.registers[d.rd]);
        }
        else
        {
            GBA_write32(gba, addr, CPU.registers[d.rd]);
        }
    }

    // if set, write back the final_addr to rn
    if (d.w || !d.p)
    {
        CPU.registers[d.rn] = final_addr;
    }
}

void arm_single_data_imm(struct GBA_Core* gba, uint32_t opcode)
{
    arm_single_data(gba, opcode, get_bit_range(0, 11, opcode));
}
