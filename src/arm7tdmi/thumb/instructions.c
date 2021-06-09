// will split these up into diff files at some point.
#include "instructions.h"
#include "arm7tdmi/arm7tdmi.h"
#include "arm7tdmi/barrel_shifter.h"
#include "arm7tdmi/arm/branch.h"
#include "internal.h"
#include "util/bit.h"


struct ThumbFormat1
{
    uint8_t rd;
    uint8_t rs;
    uint8_t offset;
};

struct ThumbFormat2
{
    uint8_t rd;
    uint8_t rs;
    uint8_t rn;
};

struct ThumbFormat3
{
    uint8_t offset;
    uint8_t rd;
};

struct ThumbFormat4
{
    uint8_t rd;
    uint8_t rs;
};

struct ThumbFormat5
{
    uint8_t rd; // hd
    uint8_t rs; // hs
};

struct ThumbFormat9
{
    uint8_t rd;
    uint8_t rb;
    uint8_t offset;
};

struct ThumbFormat14
{
    uint16_t rlist; // 16-bit because bit14/15 may be set if pcbit=1
    bool pc_lr_bit;
};

struct ThumbFormat15
{
    uint8_t rlist;
    uint8_t rb;
};


static inline struct ThumbFormat1 thumb_gen_format1(const uint16_t opcode)
{
    return (struct ThumbFormat1)
    {
        .rd = get_bit_range(0, 2, opcode),
        .rs = get_bit_range(3, 5, opcode),
        .offset = get_bit_range(6, 10, opcode),
    };
}

static inline struct ThumbFormat2 thumb_gen_format2(const uint16_t opcode)
{
    return (struct ThumbFormat2)
    {
        .rd = get_bit_range(0, 2, opcode),
        .rs = get_bit_range(3, 5, opcode),
        .rn = get_bit_range(6, 8, opcode),
    };
}

static inline struct ThumbFormat3 thumb_gen_format3(const uint16_t opcode)
{
    return (struct ThumbFormat3)
    {
        .offset = get_bit_range(0, 7, opcode),
        .rd = get_bit_range(8, 10, opcode),
    };
}

static inline struct ThumbFormat4 thumb_gen_format4(const uint16_t opcode)
{
    return (struct ThumbFormat4)
    {
        .rd = get_bit_range(0, 2, opcode),
        .rs = get_bit_range(3, 5, opcode),
    };
}

static inline struct ThumbFormat5 thumb_gen_format5(const uint16_t opcode)
{
    return (struct ThumbFormat5)
    {
        .rd = get_bit_range(0, 2, opcode),
        .rs = get_bit_range(3, 5, opcode),
    };
}

static inline struct ThumbFormat9 thumb_gen_format9(const uint16_t opcode)
{
    return (struct ThumbFormat9)
    {
        .rd = get_bit_range(0, 2, opcode),
        .rb = get_bit_range(3, 5, opcode),
        .offset = get_bit_range(6, 10, opcode),
    };
}

static inline struct ThumbFormat14 thumb_gen_format14(const uint16_t opcode)
{
    return (struct ThumbFormat14)
    {
        .rlist = get_bit_range(0, 7, opcode),
        .pc_lr_bit = is_bit_set(8, opcode),
    };
}

static inline struct ThumbFormat15 thumb_gen_format15(const uint16_t opcode)
{
    return (struct ThumbFormat15)
    {
        .rlist = get_bit_range(0, 7, opcode),
        .rb = get_bit_range(8, 10, opcode),
    };
}

static inline bool calc_vflag(const uint32_t a, const uint32_t b, const uint32_t r)
{
    return (is_bit_set(31, a) == is_bit_set(31, b)) && (is_bit_set(31, a) != is_bit_set(31, r));
}

static inline uint32_t thumb_rlist_store(struct GBA_Core* gba, uint32_t addr, uint32_t rlist, bool stack)
{
    assert(rlist && "rlist is 0!");

    uint32_t final_addr = 0;

    // stack gets pushed down!
    if (stack)
    {
        final_addr = addr - (__builtin_popcount(rlist) * sizeof(uint32_t));
        addr = final_addr;
    }
    else
    {
        final_addr = addr + (__builtin_popcount(rlist) * sizeof(uint32_t));
    }

    while (rlist)
    {
        const uint8_t reg_index = __builtin_ctz(rlist);

        GBA_write32(gba, addr, CPU.registers[reg_index]);

        addr += sizeof(uint32_t);
        rlist &= ~(1 << reg_index);
    }

    return final_addr;
}

static inline uint32_t thumb_rlist_load(struct GBA_Core* gba, uint32_t addr, uint32_t rlist)
{
    assert(rlist && "rlist is 0!");

    while (rlist)
    {
        const uint8_t reg_index = __builtin_ctz(rlist);

        CPU.registers[reg_index] = GBA_read32(gba, addr);

        addr += sizeof(uint32_t);
        rlist &= ~(1 << reg_index);
    }

    return addr;
}

void thumb_pop(struct GBA_Core* gba, uint16_t opcode)
{
    struct ThumbFormat14 d = thumb_gen_format14(opcode);

    // if set, PC reg is pushed, so we enable that bit in the rlist!
    if (d.pc_lr_bit)
    {
        d.rlist |= 1 << REG_PC_INDEX;
    }

    assert(d.rlist != 0 && "rlist edge case!");

    CPU.registers[REG_SP_INDEX] = thumb_rlist_load(gba, CPU.registers[REG_SP_INDEX], d.rlist);
}

void thumb_push(struct GBA_Core* gba, uint16_t opcode)
{
    struct ThumbFormat14 d = thumb_gen_format14(opcode);

    // if set, LR reg is pushed, so we enable that bit in the rlist!
    if (d.pc_lr_bit)
    {
        d.rlist |= 1 << REG_LR_INDEX;
    }

    assert(d.rlist != 0 && "rlist edge case!");

    CPU.registers[REG_SP_INDEX] = thumb_rlist_store(gba, CPU.registers[REG_SP_INDEX], d.rlist, true);
}

void thumb_stmia(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat15 d = thumb_gen_format15(opcode);

    if (CPU.registers[d.rb] == 0x03000020)
    {
        GBA_log("\tstmia found addr at idx %u rlist: %u\n", d.rb, d.rlist);
    }

    assert(d.rlist != 0 && "rlist edge case!");
    R15_CHECK(d.rb);

    CPU.registers[d.rb] = thumb_rlist_store(gba, CPU.registers[d.rb], d.rlist, false);
}

void thumb_ldmia(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat15 d = thumb_gen_format15(opcode);

    assert(d.rlist != 0 && "rlist edge case!");
    R15_CHECK(d.rb);

    GBA_log("\tLDMIA rb: %u rlist: %u\n", d.rb, d.rlist);

    CPU.registers[d.rb] = thumb_rlist_load(gba, CPU.registers[d.rb], d.rlist);
}

void thumb_str_imm(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat9 d = thumb_gen_format9(opcode);
    const uint32_t addr = CPU.registers[d.rb] + (d.offset << 2);

    R15_CHECK(d.rb);
    R15_CHECK(d.rd);

    GBA_write32(gba, addr, CPU.registers[d.rd]);
}

static inline void thumb_move_shifted_reg(struct GBA_Core* gba, struct ThumbFormat1 d, struct ShiftResult s)
{
    CPU.cpsr.N = is_bit_set(31, s.value);
    CPU.cpsr.Z = s.value == 0;
    CPU.cpsr.C = s.carry;

    R15_CHECK(d.rs);
    R15_CHECK(d.rd);

    CPU.registers[d.rd] = s.value;
}

void thumb_lsl(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat1 d = thumb_gen_format1(opcode);
    const struct ShiftResult s = barrel_shift_lsl(CPU.registers[d.rs], d.offset, CPU.cpsr.C);

    thumb_move_shifted_reg(gba, d, s);
}

void thumb_lsr(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat1 d = thumb_gen_format1(opcode);
    const struct ShiftResult s = barrel_shift_lsr(CPU.registers[d.rs], d.offset, CPU.cpsr.C);

    thumb_move_shifted_reg(gba, d, s);
}

void thumb_asr(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat1 d = thumb_gen_format1(opcode);
    const struct ShiftResult s = barrel_shift_asr(CPU.registers[d.rs], d.offset, CPU.cpsr.C);

    thumb_move_shifted_reg(gba, d, s);
}

static inline uint32_t thumb_add(struct GBA_Core* gba, uint32_t a, uint32_t b, bool cond)
{
    const uint64_t result = a + b;

    if (cond)
    {
        CPU.cpsr.Z = (result & 0xFFFFFFFF) == 0;
        CPU.cpsr.C = (result) > sizeof(UINT32_MAX);
        CPU.cpsr.N = is_bit_set(31, result);
        CPU.cpsr.V = calc_vflag(a, b, result);
    }

    return (uint32_t)result;
}

static inline uint32_t thumb_sub(struct GBA_Core* gba, uint32_t a, uint32_t b, bool cond)
{
    const uint32_t result = a - b;

    if (cond)
    {
        CPU.cpsr.Z = a == b;
        CPU.cpsr.C = a < b;
        CPU.cpsr.N = is_bit_set(31, result);
        CPU.cpsr.V = calc_vflag(a, b, result);
    }

    return result;
}

static inline void thumb_hi_reg_sub(struct GBA_Core* gba, uint16_t opcode, uint8_t off_dst, uint8_t off_src, bool cond)
{
    assert((off_dst == 0 || off_dst == 8) && (off_src == 0 || off_src == 8));

    const struct ThumbFormat5 d = thumb_gen_format5(opcode);
    const uint8_t dst = d.rd + off_dst;
    const uint8_t src = d.rs + off_src;

    R15_CHECK(dst);
    R15_CHECK(src);

    CPU.registers[dst] = thumb_sub(gba, CPU.registers[dst], CPU.registers[src], cond);
}

static inline void thumb_hi_reg_cmp(struct GBA_Core* gba, uint16_t opcode, uint8_t off_dst, uint8_t off_src)
{
    assert((off_dst == 0 || off_dst == 8) && (off_src == 0 || off_src == 8));

    const struct ThumbFormat5 d = thumb_gen_format5(opcode);
    const uint8_t dst = d.rd + off_dst;
    const uint8_t src = d.rs + off_src;

    R15_CHECK(dst);
    R15_CHECK(src);

    thumb_sub(gba, CPU.registers[dst], CPU.registers[src], true);
}

static inline void thumb_hi_reg_mov(struct GBA_Core* gba, uint16_t opcode, uint8_t off_dst, uint8_t off_src)
{
    assert((off_dst == 0 || off_dst == 8) && (off_src == 0 || off_src == 8));

    const struct ThumbFormat5 d = thumb_gen_format5(opcode);
    const uint8_t dst = d.rd + off_dst;
    const uint8_t src = d.rs + off_src;

    R15_CHECK(dst);
    R15_CHECK(src);

    GBA_log("\tdst: %u src: %u dst_v: 0x%08X src_v: 0x%08X\n", dst, src, CPU.registers[dst], CPU.registers[src]);

    CPU.registers[dst] = CPU.registers[src];
}

void thumb_cmp_rd_rs(struct GBA_Core* gba, uint16_t opcode)
{
    thumb_hi_reg_cmp(gba, opcode, 0, 0);
}

void thumb_mov_rd_rs(struct GBA_Core* gba, uint16_t opcode)
{
    thumb_hi_reg_mov(gba, opcode, 0, 0);
}

void thumb_mov_hd_rs(struct GBA_Core* gba, uint16_t opcode)
{
    thumb_hi_reg_mov(gba, opcode, 8, 0);
}

void thumb_mov_hd_hs(struct GBA_Core* gba, uint16_t opcode)
{
    thumb_hi_reg_mov(gba, opcode, 8, 8);
}

void thumb_bx_rs(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat5 d = thumb_gen_format5(opcode);
    
    GBA_log("bx_rs, rs: %u value: 0x%08X\n", d.rs, CPU.registers[d.rs]);

    arm_bx_generic(gba, CPU.registers[d.rs]);
}

void thumb_bx_hs(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat5 d = thumb_gen_format5(opcode);
    const uint8_t hs = d.rs + 8;
    
    GBA_log("bx_hs, hs: %u rs: %u value: 0x%08X\n", hs, d.rs, CPU.registers[hs]);

    R15_CHECK(hs);

    arm_bx_generic(gba, CPU.registers[hs]);
}

void thumb_bic(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat4 d = thumb_gen_format4(opcode);
    const uint32_t result = CPU.registers[d.rd] & (~CPU.registers[d.rs]);

    R15_CHECK(d.rd);
    R15_CHECK(d.rs);

    CPU.cpsr.Z = (result & 0xFFFFFFFF) == 0;
    CPU.cpsr.N = is_bit_set(31, result);

    CPU.registers[d.rd] = result;
}

void thumb_add_imm(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat3 d = thumb_gen_format3(opcode);

    R15_CHECK(d.rd);

    CPU.registers[d.rd] = thumb_add(gba, CPU.registers[d.rd], d.offset, true);
}

void thumb_sub_imm(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat3 d = thumb_gen_format3(opcode);

    R15_CHECK(d.rd);

    CPU.registers[d.rd] = thumb_sub(gba, CPU.registers[d.rd], d.offset, true);
}

void thumb_cmp_imm(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat3 d = thumb_gen_format3(opcode);

    R15_CHECK(d.rd);

    thumb_sub(gba, CPU.registers[d.rd], d.offset, true);
}

void thumb_mov_imm(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat3 d = thumb_gen_format3(opcode);

    R15_CHECK(d.rd);

    CPU.registers[d.rd] = d.offset;
}

void thumb_add_reg(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat2 d = thumb_gen_format2(opcode);

    R15_CHECK(d.rd);
    R15_CHECK(d.rs);
    R15_CHECK(d.rn);

    CPU.registers[d.rd] = thumb_add(gba, CPU.registers[d.rs], CPU.registers[d.rn], true);
}

void thumb_sub_reg(struct GBA_Core* gba, uint16_t opcode)
{
    const struct ThumbFormat2 d = thumb_gen_format2(opcode);

    R15_CHECK(d.rd);
    R15_CHECK(d.rs);
    R15_CHECK(d.rn);

    CPU.registers[d.rd] = thumb_sub(gba, CPU.registers[d.rs], CPU.registers[d.rn], true);
}

void thumb_cond(struct GBA_Core* gba, uint16_t opcode)
{
    const uint16_t off = get_bit_range(0, 7, opcode) << 1;
    const int16_t soff = sign_extend(8, off);//((off & ~0x80) << 1) | ((off & 0x80) << 8);
    const uint8_t cond = get_bit_range(8, 11, opcode);

    if (ARM7_get_cond(gba, cond))
    {
        // GBA_log("we branch, old pc %u new pc %u\n", CPU.registers[REG_PC_INDEX], CPU.registers[REG_PC_INDEX] + 2 + soff);
        CPU.registers[REG_PC_INDEX] += 2 + soff; 
    }
}

void thumb_pc_relative_load(struct GBA_Core* gba, uint16_t opcode)
{
    // this is a 10-bit offset
    const uint32_t offset = get_bit_range(0, 7, opcode) << 2;
    const uint8_t rd = get_bit_range(8, 10, opcode);
    // + 4 because of pipeline
    const uint32_t addr = CPU.registers[REG_PC_INDEX] + 2 + offset;

    R15_CHECK(rd);

    // bit 1 is forced to 0 to ensure word aligned
    CPU.registers[rd] = GBA_read32(gba, addr & ~0x2);
    // TODO: is this like arm ldr which rotates the value?
    // CPU.registers[rd] = bit_rotr((addr & 0x1) , CPU.registers[rd]);
}

void thumb_bl_high(struct GBA_Core* gba, uint16_t opcode)
{
    GBA_log("\tBL hi\n");

    const uint32_t offset = get_bit_range(0, 10, opcode) << 12;
    // top half is sign extened.
    const int32_t signed_offset = sign_extend(22, offset);
    // +2 because of pipeline
    CPU.registers[REG_LR_INDEX] = 2 + CPU.registers[REG_PC_INDEX] + signed_offset;
}

// oldpc: 08000340
// oldlink: 08000187
void thumb_bl_low(struct GBA_Core* gba, uint16_t opcode)
{
    GBA_log("\tBL lo\n");

    const uint32_t offset = CPU.registers[REG_LR_INDEX] + (get_bit_range(0, 10, opcode) << 1);
    // save the next instruction
    const uint32_t tmp = CPU.registers[REG_PC_INDEX];
    // should this be OR'd instead?
    CPU.registers[REG_PC_INDEX] = offset;
    CPU.registers[REG_LR_INDEX] = tmp | 1;
}
