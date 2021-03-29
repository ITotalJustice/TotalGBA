// this is not going to be a real main file
// this is just for quick core testing, without compiling
// the frontend.

// this will support rom loading, for parsing and such
// but no memory will be freed, let the OS handle it!

#include "core/gba.h"
#include "core/util/bit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void _gen_thumb_table() {
    FILE* fout = fopen("thumb_instruction_table.h", "w");

    fprintf(fout, "#pragma once\n\n#include \"core/types.h\"\n\n");
    fprintf(fout, "%s\n%s\n%s\n\n", "#ifdef __cplusplus", "extern \"C\" {", "#endif");
    fprintf(fout, "%s\n\n", "typedef void (*thumb_instruction_t)(struct GBA_Core*, uint16_t opcode);");

    fprintf(fout, "%s\n\n", "static void THUMB_instruction_not_implemented(struct GBA_Core* gba, uint16_t opcode) {}");

    fprintf(fout, "%s\n%s\n", "// 10-bit table", "static const thumb_instruction_t THUMB_INSTRUCTION_TABLE[0x400] = {");

    for (int i = 0; i < 0x400; i++) {
        fprintf(fout, "\t[0x%03X] = %s,\n", i, "THUMB_instruction_not_implemented");
    }
    fprintf(fout, "};\n\n");

    fprintf(fout, "%s\n%s\n%s\n\n", "#ifdef __cplusplus", "}", "#endif");

    fclose(fout);
}

static void gen_empty_tables() {
    _gen_thumb_table();
}

static uint32_t get_mask_range(uint32_t end) {
    uint32_t result = 0;
    for (uint32_t bit = 0; bit < end; ++bit) {
        result |= (1 << bit);
    }

    return result;
}

static void gen_mask_table() {
    FILE* fout = fopen("mask_table.h", "w");

    fprintf(fout, "#pragma once\n\n#include \"core/types.h\"\n\n");
    fprintf(fout, "%s\n%s\n%s\n\n", "#ifdef __cplusplus", "extern \"C\" {", "#endif");

    fprintf(fout, "%s\n%s\n", "// 32-bit mask-range table", "static const uint32_t MASK_RANGE_TABLE[32] = {");

    for (int i = 1; i <= 32; i++) {
        const uint32_t mask = get_mask_range(i);
        fprintf(fout, "\t[0x%02X] = 0x%08XU,\n", i - 1, mask);
    }

    fprintf(fout, "};\n\n");

    fprintf(fout, "%s\n%s\n%s\n\n", "#ifdef __cplusplus", "}", "#endif");

    fclose(fout);
}

static void sanity_tests() {
    // ensure the size of the header is the same across platforms!
    assert(sizeof(struct GBA_CartHeader) == CART_HEADER_SIZE && "Header size has changed!");

    // simple 24-bit asr
    assert(sign_extend(23, 0b110011111111111111111111) == (int32_t)0b1111'1111'1100'1111'1111'1111'1111'1111);
    // set the sign-bit to bit 1, then asr 31-bits
    assert(sign_extend(0, 0b0001) == (int32_t)0b1111'1111'1111'1111'1111'1111'1111'1111);
    // this is used in thumb ldr halword sign
    assert(sign_extend(15, 0b0000'0000'1110'0000'1111'1111'1111'1111) == (int32_t)0b1111'1111'1111'1111'1111'1111'1111'1111);
    // same as above but no sign
    assert(sign_extend(15, 0b0000'0000'1110'0000'0111'1111'1111'1111) == 0b0000'0000'0000'0000'0111'1111'1111'1111);

    // check that bit ranges till work...
    assert(get_bit_range(3, 5, 0b111'000) == 0b000'111);
    assert(get_bit_range(0, 2, 0b000'010) == 0b000'010);
    assert(get_bit_range(1, 5, 0b111'110) == 0b011'111);
    assert(get_bit_range(4, 5, 0b110'000) == 0b000'011);
}

int main(int argc, char** argv) {
    uint8_t* rom_data = NULL;
    uint32_t rom_size = 0;

    // if this asserts, we have big problems
    sanity_tests();


    if (argc > 1) {
        FILE* f = fopen(argv[1], "rb");
        assert(f != NULL);

        fseek(f, 0, SEEK_END);
        rom_size = (uint32_t)ftell(f); // can be -1...
        fseek(f, 0, SEEK_SET);

        rom_data = (uint8_t*)malloc(rom_size);
        fread(rom_data, sizeof(uint8_t), rom_size, f);

        fclose(f);
    }

    struct GBA_Core* gba = (struct GBA_Core*)malloc(sizeof(struct GBA_Core));
    GBA_init(gba);

    if (rom_data != NULL) {
        GBA_loadrom_data(gba, rom_data, rom_size);
        for (;;) {
            GBA_run_frame(gba);
        }
    }

    // todo: loop here...

    // gen_mask_table();
    // gen_empty_tables();

    return 0;
}
