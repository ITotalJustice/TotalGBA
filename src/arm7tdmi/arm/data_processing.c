#include "data_processing.h"
#include "arm7tdmi/arm7tdmi.h"
#include "arm7tdmi/barrel_shifter.h"
#include "internal.h"
#include "util/bit.h"


struct format4_5
{
    uint8_t rd;
    uint8_t rn;
    bool s;
};


static inline struct format4_5 gen_format4_5(const uint32_t opcode)
{
    return (struct format4_5)
    {
        .rd = get_bit_range(12, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .s = is_bit_set(20, opcode),
    };
}

static inline bool calc_vflag(const uint32_t a, const uint32_t b, const uint32_t r)
{
    return (is_bit_set(31, a) == is_bit_set(31, b)) && (is_bit_set(31, a) != is_bit_set(31, r));
}

static inline struct ShiftResult format4_5_get_imm(struct GBA_Core* gba, uint32_t opcode)
{
    const uint8_t imm = get_bit_range(0, 7, opcode);
    // rotated by twice the value
    const uint8_t rotate = get_bit_range(8, 11, opcode) * 2;
    // sign exteneded to 32-bits, still unsigned
    const uint32_t extended_value = (uint32_t)sign_extend(7, imm);
    // finally rotate
    return barrel_shift_ror(extended_value, rotate, CPU.cpsr.C);
}

static inline uint32_t arm_internal_add(struct GBA_Core* gba, uint32_t a, uint32_t b, bool s)
{
    const uint64_t result = a + b;

    // only update cpsr if set
    if (s)
    {
        CPU.cpsr.Z = (result & 0xFFFFFFFF) == 0;
        CPU.cpsr.C = (result) > sizeof(UINT32_MAX);
        CPU.cpsr.N = is_bit_set(31, result);
        CPU.cpsr.V = calc_vflag(a, b, result);
    }

    return (uint32_t)result;
}

void arm_add_imm(struct GBA_Core* gba, uint32_t opcode)
{
    const struct format4_5 d = gen_format4_5(opcode);
    const struct ShiftResult s = format4_5_get_imm(gba, opcode);
    uint32_t oprand1 = CPU.registers[d.rn];

    R15_CHECK(d.rd);

    if (d.rn == REG_PC_INDEX)
    {
        GBA_log("add_imm is using r15 as rm!\n");
        oprand1 += 4;
    }

    CPU.registers[d.rd] = arm_internal_add(gba, oprand1, s.value, d.s);
}

// logical

static inline void arm_set_logical_flags(struct GBA_Core* gba, uint32_t result, bool carry)
{
    CPU.cpsr.Z = result == 0;
    CPU.cpsr.C = carry;
    CPU.cpsr.N = is_bit_set(31, result);
}

void arm_and_imm(struct GBA_Core* gba, uint32_t opcode)
{
    const struct ShiftResult s = format4_5_get_imm(gba, opcode);
    const struct format4_5 d = gen_format4_5(opcode);

    R15_CHECK(d.rd);

    if (d.rn == REG_PC_INDEX)
    {
        GBA_log_fatal("mov_imm is using r15 as rm!\n");
    }


    CPU.registers[d.rd] &= s.value;

    arm_set_logical_flags(gba, CPU.registers[d.rd], s.carry);
}

void arm_mov_logical_left(struct GBA_Core* gba, uint32_t opcode)
{
    const uint8_t shift type;
}

void arm_mov_imm(struct GBA_Core* gba, uint32_t opcode)
{
    const struct ShiftResult s = format4_5_get_imm(gba, opcode);
    const struct format4_5 d = gen_format4_5(opcode);

    R15_CHECK(d.rd);

    if (d.rn == REG_PC_INDEX)
    {
        GBA_log_fatal("mov_imm is using r15 as rm!\n");
    }

    CPU.registers[d.rd] = s.value;

    arm_set_logical_flags(gba, CPU.registers[d.rd], s.carry);
}
