#include "block_data.h"
#include "arm7tdmi/arm7tdmi.h"
#include "internal.h"
#include "util/bit.h"


struct format4_11
{
    uint16_t rlist;
    uint8_t rn;
    bool l;
    bool w;
    bool s;
    bool u;
    bool p;
};

static inline struct format4_11 gen_format4_11(const uint32_t opcode)
{
    return (struct format4_11)
    {
        .rlist = get_bit_range(0, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .l = is_bit_set(20, opcode),
        .w = is_bit_set(21, opcode),
        .s = is_bit_set(22, opcode),
        .u = is_bit_set(23, opcode),
        .p = is_bit_set(24, opcode),
    };
}

// i miss templates...
static inline void arm_rlist(struct GBA_Core* gba, uint32_t opcode)
{
    struct format4_11 d = gen_format4_11(opcode);

    assert(d.rlist && "edge case with rlist==0");
    assert((d.rlist & (1 << REG_PC_INDEX)) == 0 && "PC in rlist!");
    assert((d.rlist & (1 << d.rn)) == 0 && "RN in rlist!");
    assert(!d.s && "PSR bit is set!");

    GBA_log("\trlist: %u\n", d.rlist);
    GBA_log("\trn: %u\n", d.rn);
    GBA_log("\tl: %u\n", d.l);
    GBA_log("\tw: %u\n", d.w);
    GBA_log("\ts: %u\n", d.s);
    GBA_log("\tu: %u\n", d.u);
    GBA_log("\tp: %u\n", d.p);

    uint32_t addr = CPU.registers[d.rn];
    uint32_t final_addr = addr;

    // these are used to avoid having a huge if else tree
    const uint32_t pre = d.p ? 0 : sizeof(uint32_t);
    const uint32_t post = d.p ? sizeof(uint32_t) : 0;

    // if set we are incrementing, else, going down
    if (d.u)
    {
        final_addr = addr + (__builtin_popcount(d.rlist) * sizeof(uint32_t));
    }
    else
    {
        final_addr = addr - (__builtin_popcount(d.rlist) * sizeof(uint32_t));
        addr = final_addr;
    }

    // if set, load, else, store
    if (d.l)
    {
        while (d.rlist)
        {
            const uint8_t reg_index = __builtin_ctz(d.rlist);

            addr += pre;
            CPU.registers[reg_index] = GBA_read32(gba, addr);
            addr += post;

            d.rlist &= ~(1 << reg_index);
        }
    }
    else
    {
        while (d.rlist)
        {
            const uint8_t reg_index = __builtin_ctz(d.rlist);

            addr += pre;
            GBA_write32(gba, addr, CPU.registers[reg_index]);
            addr += post;

            d.rlist &= ~(1 << reg_index);
        }
    }

    // if set, write back
    if (d.w || !d.p)
    {
        CPU.registers[d.rn] = final_addr;
    }
}

void arm_stm_ldm(struct GBA_Core* gba, uint32_t opcode)
{
    arm_rlist(gba, opcode);
}
