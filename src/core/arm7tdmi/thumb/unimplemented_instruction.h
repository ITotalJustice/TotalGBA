#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/arm7tdmi/thumb/opcode_hash.h"

#include <stdio.h>
#include <assert.h>


static void THUMB_instruction_not_implemented(struct GBA_Core* gba, uint16_t opcode) {
    fprintf(stderr,
        "\n[THUMB-ERROR] unimp instruction 0x%04X, hash: 0x%03X\n",
        opcode, THUMB_opcode_hash(opcode)
    );
    
    assert(0 && "unimplemented THUMB instruction");
}

#ifdef __cplusplus
}
#endif
