#include "core/arm7tdmi/arm7tdmi.h"
#include "core/arm7tdmi/arm/encoding_types.h"

#include <stdio.h>
#include <assert.h>


enum {
    STORE,
    LOAD
};

enum {
    NO_WRITE_BACK,
    WRITE_BACK
};

enum {
    WORD_TRANSFER,
    BYTE_TRANSFER
};

enum {
    SUB,
    ADD
};

enum {
    POST,
    PRE
};


static inline void single_data_transfer(struct GBA_Core* gba, const struct Format_4_9* format) {
    uint32_t base_offset = CPU.registers[format->rn];

    // check if we modify the offset BEFORE transfer
    if (format->P == PRE) {
        // check if its add or sub
        if (format->U == SUB) {
            base_offset -= format->offset;
        }
        else {
            base_offset += format->offset;
        }
    }

    // check if instruction is store or load
    if (format->L == STORE) {
        if (format->B == WORD_TRANSFER) {
            GBA_write32(gba, base_offset, CPU.registers[format->rd]);
        }
        else {
            GBA_write8(gba, base_offset, CPU.registers[format->rd]);
        }
    }
    else {
        if (format->B == WORD_TRANSFER) {
            CPU.registers[format->rd] = GBA_read32(gba, base_offset);
        }
        else {
            CPU.registers[format->rd] = GBA_read8(gba, base_offset);
        }
    }

    // check if we modify the offset AFTER transfer
    if (format->P == POST) {
        // check if its add or sub
        if (format->U == SUB) {
            base_offset -= format->offset;
        }
        else {
            base_offset += format->offset;
        }
    }

    // check if we need to write the offset back to rn
    if (format->W == WRITE_BACK) {
        CPU.registers[format->rn] = base_offset;
    }
}

// [4.9]
void ARM_instruction_single_data_transfer_imm(struct GBA_Core* gba, uint32_t opcode) {
    struct Format_4_9 format = gen_Format_4_9_imm(opcode);
    
    // UB if we access the union out of order!
    format.offset = format.imm.offset;

    // do transfer
    single_data_transfer(gba, &format);
}

void ARM_instruction_single_data_transfer_reg(struct GBA_Core* gba, uint32_t opcode) {
    struct Format_4_9 format = gen_Format_4_9_reg(opcode);

    // perform shift, discarding the carry result
    const struct ShiftResult result = barrel_shift(
        format.reg.shift_type,
        CPU.registers[format.reg.rm], format.reg.shift_v, 0
    );

    // save the result as offset
    format.offset = result.value;

    // do transfer
    single_data_transfer(gba, &format);
}
