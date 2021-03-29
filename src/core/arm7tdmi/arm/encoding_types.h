#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/util/bit.h"
#include "core/arm7tdmi/barrel_shifter.h"

// [4.4]
struct Format_B {
    int32_t offset;
};

static inline struct Format_B gen_Format_B(const uint32_t opcode) {
    return (struct Format_B){
        .offset = sign_extend(25, opcode << 2)
    };
}

// [4.5]
struct Format_4_5 {
    union {
        struct {
            uint8_t rm;
            uint8_t shift;
        } reg;

        struct {
            uint8_t imm;
            uint8_t rotate : 4;
        } imm;
    } operand2;

    uint8_t rd : 4;
    uint8_t rn : 4;
    uint8_t s : 1;
};

/* gen type functions */
static inline struct Format_4_5 gen_Format_4_5_reg(const uint32_t opcode) {
    return (struct Format_4_5){
        .operand2 = {
            .reg = {
                .rm = get_bit_range(0, 3, opcode),
                .shift = get_bit_range(4, 11, opcode)
            }
        },
        .rd = get_bit_range(12, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .s = is_bit_set(20, opcode)
    };
}

static inline struct Format_4_5 gen_Format_4_5_imm(const uint32_t opcode) {
    return (struct Format_4_5){
        .operand2 = {
            .imm = {
                .imm = get_bit_range(0, 7, opcode),
                .rotate = get_bit_range(8, 11, opcode)
            }
        },
        .rd = get_bit_range(12, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .s = is_bit_set(20, opcode)
    };
}
    

#ifdef __cplusplus
}
#endif
