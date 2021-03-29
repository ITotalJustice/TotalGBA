#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/util/bit.h"
#include "core/arm7tdmi/barrel_shifter.h"

// [4.3]
struct Format_4_3 {
    uint8_t rn : 4;
};

static inline struct Format_4_3 gen_Format_4_3(const uint32_t opcode) {
    return (struct Format_4_3){
        .rn = get_bit_range(0, 3, opcode)
    };
}

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
            uint8_t rm : 4;
            uint8_t shift_type : 2;
            union {
                uint8_t rs : 4;
                uint8_t shift_amount : 5;
            };
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
static inline struct Format_4_5 gen_Format_4_5_reg_rs(const uint32_t opcode) {
    return (struct Format_4_5){
        .operand2 = {
            .reg = {
                .rm = get_bit_range(0, 3, opcode),
                .shift_type = get_bit_range(5, 6, opcode),
                .rs = get_bit_range(8, 11, opcode)
            }
        },
        .rd = get_bit_range(12, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .s = is_bit_set(20, opcode)
    };
}

static inline struct Format_4_5 gen_Format_4_5_reg_shift(const uint32_t opcode) {
    return (struct Format_4_5){
        .operand2 = {
            .reg = {
                .rm = get_bit_range(0, 3, opcode),
                .shift_type = get_bit_range(5, 6, opcode),
                .shift_amount = get_bit_range(7, 11, opcode)
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


// [4.6]
struct Format_4_6 {
    union {
        uint8_t rd : 4;
        uint8_t rm : 4;
        struct {
            uint8_t imm;
            uint8_t rotate : 4;
        };
    };
};

static inline struct Format_4_6 gen_Format_4_6_MRS(const uint32_t opcode) {
    return (struct Format_4_6){
        .rd = get_bit_range(12, 15, opcode)
    };
}

static inline struct Format_4_6 gen_Format_4_6_MSR(const uint32_t opcode) {
    return (struct Format_4_6){
        .rm = get_bit_range(0, 3, opcode)
    };
}

static inline struct Format_4_6 gen_Format_4_6_MSR_reg_flags(const uint32_t opcode) {
    return (struct Format_4_6){
        .rm = get_bit_range(0, 3, opcode)
    };
}

static inline struct Format_4_6 gen_Format_4_6_MSR_imm_flags(const uint32_t opcode) {
    return (struct Format_4_6){
        .imm = get_bit_range(0, 7, opcode),
        .rotate = get_bit_range(8, 11, opcode)
    };
}

// [4.9]
struct Format_4_9 {
    union {
        struct {
            uint8_t rm : 4;
            struct {
                uint8_t shift_type : 2;
                uint8_t shift_v : 5;
            };
        } reg;

        struct {
            uint16_t offset : 12;
        } imm;

        uint32_t offset;
    };

    uint8_t rd : 4;
    uint8_t rn : 4;
    uint8_t L : 1;
    uint8_t W : 1;
    uint8_t B : 1;
    uint8_t U : 1;
    uint8_t P : 1;
};

static inline struct Format_4_9 gen_Format_4_9_imm(const uint32_t opcode) {
    return (struct Format_4_9){
        .imm = {
            .offset = get_bit_range(0, 11, opcode)
        },
        .rd = get_bit_range(12, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .L = is_bit_set(20, opcode),
        .W = is_bit_set(21, opcode),
        .B = is_bit_set(22, opcode),
        .U = is_bit_set(23, opcode),
        .P = is_bit_set(24, opcode)
    };
}

static inline struct Format_4_9 gen_Format_4_9_reg(const uint32_t opcode) {
    return (struct Format_4_9){
        .reg = {
            .rm = get_bit_range(0, 3, opcode),
            .shift_type = get_bit_range(5, 6, opcode),
            .shift_v = get_bit_range(7, 11, opcode)
        },
        .rd = get_bit_range(12, 15, opcode),
        .rn = get_bit_range(16, 19, opcode),
        .W = is_bit_set(21, opcode),
        .B = is_bit_set(22, opcode),
        .U = is_bit_set(23, opcode),
        .P = is_bit_set(24, opcode)
    };
}

#ifdef __cplusplus
}
#endif
