#include "core/types.h"

#include <stdio.h>
#include <assert.h>


void ARM_instruction_UNDEFINED(struct GBA_Core* gba, uint32_t opcode) {
    fprintf(stderr,
        "\n[ARM-ERROR] UNDEFINED INSTRUCTION: 0x%08X\n",
        opcode
    );
    assert(0 && "undefined instruction");
}
