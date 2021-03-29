#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/util/bit.h"


struct Format_1 {
    uint8_t rd : 3;
    uint8_t rs : 3;
    uint8_t offset5 : 5;
};

struct Format_2 {
    uint8_t rd : 3;
    uint8_t rs : 3;
    union {
        uint8_t rn : 3;
        uint8_t offset3 : 3;
    };
};

struct Format_3 {
    uint8_t offset8;
    uint8_t rd : 3;
};

struct Format_4 {
    uint8_t rd : 3;
    uint8_t rs : 3;
};

struct Format_5 {
    union {
        uint8_t rd : 3;
        uint8_t hd : 4;
    };
    union {
        uint8_t rs : 3;
        uint8_t hs : 4;
    };
};

struct Format_6 {
    uint16_t word8 : 10; // already shifted! (10-bit)
    uint8_t rd : 3;
};

struct Format_7 {
    uint8_t rd : 3;
    uint8_t rb : 3;
    uint8_t ro : 3;
};

struct Format_8 {
    uint8_t rd : 3;
    uint8_t rb : 3;
    uint8_t ro : 3;
};

struct Format_9 {
    uint8_t rd : 3;
    uint8_t rb : 3;
    uint8_t offset5 : 7; // can be either 5-bit or 7-bit
};

struct Format_10 {
    uint8_t rd : 3;
    uint8_t rb : 3;
    uint8_t offset5 : 6; // already shifted! (6-bit)
};

struct Format_11 {
    uint16_t word8 : 10; // already shifted! (10-bit)
    uint8_t rd : 3;
};

struct Format_12 {
    uint16_t word8 : 10; // already shifted! (10-bit)
    uint8_t rd : 3;
};

struct Format_13 {
    int16_t sword7 : 9; // already shifted and sign-extended!
};

struct Format_14 {
    uint8_t rlist;
};

struct Format_15 {
    uint8_t rlist;
    uint8_t rb : 3;
};

struct Format_16 {
    int16_t soffset8 : 9; // already shifted! (9-bit)
    uint8_t cond : 4;
};

struct Format_17 {
    uint8_t value8;
};

struct Format_18 {
    uint16_t offset11 : 12; // already shifted! (12-bit)
};

struct Format_19 {
    uint16_t offset : 11;
};


/* gen type functions */
static inline struct Format_1 gen_format_1(const uint16_t opcode) {
    return (struct Format_1){
        .rd = get_bit_range(0, 2, opcode),
        .rs = get_bit_range(3, 5, opcode),
        .offset5 = get_bit_range(6, 10, opcode),
    };
}

static inline struct Format_2 gen_format_2_offset(const uint16_t opcode) {
    return (struct Format_2){
        .rd = get_bit_range(0, 2, opcode),
        .rs = get_bit_range(3, 5, opcode),
        .offset3 = get_bit_range(6, 8, opcode),
    };
}

static inline struct Format_2 gen_format_2_reg(const uint16_t opcode) {
    return (struct Format_2){
        .rd = get_bit_range(0, 2, opcode),
        .rs = get_bit_range(3, 5, opcode),
        .rn = get_bit_range(6, 8, opcode),
    };
}

static inline struct Format_3 gen_format_3(const uint16_t opcode) {
    return (struct Format_3){
        .offset8 = get_bit_range(0, 7, opcode),
        .rd = get_bit_range(8, 10, opcode),
    };
}

static inline struct Format_4 gen_format_4(const uint16_t opcode) {
    return (struct Format_4){
        .rd = get_bit_range(0, 2, opcode),
        .rs = get_bit_range(3, 5, opcode),
    };
}

static inline struct Format_6 gen_format_6(const uint16_t opcode) {
    return (struct Format_6){
        .word8 = get_bit_range(0, 7, opcode) << 2, // 10-bit
        .rd = get_bit_range(8, 10, opcode),
    };
}

static inline struct Format_7 gen_format_7(const uint16_t opcode) {
    return (struct Format_7){
        .rd = get_bit_range(0, 2, opcode),
        .rb = get_bit_range(3, 5, opcode),
        .ro = get_bit_range(6, 8, opcode),
    };
}

static inline struct Format_8 gen_format_8(const uint16_t opcode) {
    return (struct Format_8){
        .rd = get_bit_range(0, 2, opcode),
        .rb = get_bit_range(3, 5, opcode),
        .ro = get_bit_range(6, 8, opcode),
    };
}

static inline struct Format_9 gen_format_9_byte(const uint16_t opcode) {
    return (struct Format_9){
        .rd = get_bit_range(0, 2, opcode),
        .rb = get_bit_range(3, 5, opcode),
        .offset5 = get_bit_range(6, 10, opcode), // 5-bit
    };
}

static inline struct Format_9 gen_format_9_word(const uint16_t opcode) {
    return (struct Format_9){
        .rd = get_bit_range(0, 2, opcode),
        .rb = get_bit_range(3, 5, opcode),
        .offset5 = get_bit_range(6, 10, opcode) << 2, // 7-bit
    };
}

static inline struct Format_10 gen_format_10(const uint16_t opcode) {
    return (struct Format_10){
        .rd = get_bit_range(0, 2, opcode),
        .rb = get_bit_range(3, 5, opcode),
        .offset5 = get_bit_range(6, 10, opcode) << 1, // 6-bit
    };
}

static inline struct Format_11 gen_format_11(const uint16_t opcode) {
    return (struct Format_11){
        .word8 = get_bit_range(0, 7, opcode) << 2, // 10-bit
        .rd = get_bit_range(8, 10, opcode),
    };
}

static inline struct Format_12 gen_format_12(const uint16_t opcode) {
    return (struct Format_12){
        .word8 = get_bit_range(0, 7, opcode) << 2, // 10-bit
        .rd = get_bit_range(8, 10, opcode),
    };
}

static inline struct Format_13 gen_format_13(const uint16_t opcode) {
    return (struct Format_13){
        .sword7 = sign_extend(7, opcode), // 9-bit sign extended
    };
}

static inline struct Format_14 gen_format_14(const uint16_t opcode) {
    return (struct Format_14){
        .rlist = get_bit_range(0, 7, opcode),
    };
}

static inline struct Format_15 gen_format_15(const uint16_t opcode) {
    return (struct Format_15){
        .rlist = get_bit_range(0, 7, opcode),
        .rb = get_bit_range(8, 10, opcode),
    };
}

static inline struct Format_17 gen_format_17(const uint16_t opcode) {
    return (struct Format_17){
        .value8 = get_bit_range(0, 7, opcode),
    };
}

static inline struct Format_18 gen_format_18(const uint16_t opcode) {
    return (struct Format_18){
        .offset11 = get_bit_range(0, 10, opcode) << 1, // 12-bit
    };
}

static inline struct Format_19 gen_format_19(const uint16_t opcode) {
    return (struct Format_19){
        .offset = get_bit_range(0, 10, opcode),
    };
}

#ifdef __cplusplus
}
#endif
