#include "core/arm7tdmi/arm7tdmi.h"

#include "core/arm7tdmi/arm/encoding_types.h"
#include "core/arm7tdmi/arm/instruction_table.h"

#include "core/arm7tdmi/thumb/instruction_table.h"
#include "core/arm7tdmi/thumb/encoding_types.h"


static inline uint32_t fetch_arm(struct GBA_Core* gba) {
    const uint32_t opcode = GBA_read32(gba, CPU.registers[REG_PC_INDEX]);
    CPU.registers[REG_PC_INDEX] += sizeof(uint32_t);
    return opcode;
}

static inline uint16_t fetch_thumb(struct GBA_Core* gba) {
    const uint16_t opcode = GBA_read16(gba, CPU.registers[REG_PC_INDEX]);
    CPU.registers[REG_PC_INDEX] += sizeof(uint16_t);
    return opcode;
}

static inline void execute_arm(struct GBA_Core* gba, const uint32_t opcode) {
    // generate decoded opcode hash.
    const uint32_t hash_index = ARM_opcode_hash(opcode);
    // now execute instruction!
    ARM_INSTRUCTION_TABLE[hash_index](gba, opcode);
}

static inline void execute_thumb(struct GBA_Core* gba, const uint16_t opcode) {
    // generate decoded opcode hash.
    const uint16_t hash_index = THUMB_opcode_hash(opcode);
    // now execute instruction!
    THUMB_INSTRUCTION_TABLE[hash_index](gba, opcode);
}

void ARM7_init(struct GBA_Core* gba) {
    enum { ROM_START_ADDR = 0x08000000 };

    ARM7_set_mode(gba, MODE_SYSTEM);
    CPU.registers[REG_PC_INDEX] = ROM_START_ADDR;
}

void ARM7_run(struct GBA_Core* gba) {
    // get which state (ARM, THUMB) we are in
    const enum Arm7tdmiState state = ARM7_get_state(gba);

    switch (state) {
        case STATE_ARM: execute_arm(gba, fetch_arm(gba)); break;
        case STATE_THUMB: execute_thumb(gba, fetch_thumb(gba)); break;
    }
}
