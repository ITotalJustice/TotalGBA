#include "core/types.h"
#include "core/arm7tdmi/arm/opcode_hash.h"

#include <stdio.h>
#include <assert.h>


void ARM_instruction_not_implemented(struct GBA_Core* gba, uint32_t opcode) {
    fprintf(stderr,
        "\n[ARM-ERROR] unimp instruction: 0x%08X, hash: 0x%03X\n",
        opcode, ARM_opcode_hash(opcode)
    );
    
    assert(0 && "unimplemented ARM instruction");
}
