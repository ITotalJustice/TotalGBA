#include "gba.h"
#include  "internal.h"

#include "core/arm7tdmi/arm/arm_instruction_table.h"
#include "core/arm7tdmi/thumb/thumb_instruction_table.h"
#include "core/arm7tdmi/thumb/encoding_types.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>


void GBA_init(struct GBA_Core* gba) {
    assert(gba);
    memset(gba, 0, sizeof(struct GBA_Core));
}

void GBA_reset(struct GBA_Core* gba) {
    memset(gba, 0, sizeof(struct GBA_Core));
}

static void print_header(const struct GBA_CartHeader* header) {
    fprintf(stdout, "\nGBA CART HEADER INFO\n");
        fprintf(stdout, "\trom_entry_point: 0x%08X\n", header->rom_entry_point);
        fprintf(stdout, "\tgame_title: %.*s\n", (int)sizeof(header->game_title), header->game_title);
        fprintf(stdout, "\tgame_code: %.*s\n", (int)sizeof(header->game_code), header->game_code);
        fprintf(stdout, "\tmaker_code: %.*s\n", (int)sizeof(header->maker_code), header->maker_code);
        fprintf(stdout, "\tfixed_value: 0x%02X\n", header->fixed_value);
        fprintf(stdout, "\tmain_unit_code: 0x%02X\n", header->main_unit_code);
        fprintf(stdout, "\tdevice_type: 0x%02X\n", header->device_type);
        fprintf(stdout, "\tsoftware_version: 0x%02X\n", header->software_version);
        fprintf(stdout, "\tcomplement_check: 0x%02X\n", header->complement_check);
    fprintf(stdout, "\n");
}

static bool validate_header_logo(const struct GBA_CartHeader* header) {
    return false;
}

static bool validate_header_fixed_value(const struct GBA_CartHeader* header) {
    enum { FIXED_VALUE = 0x96 };
    
    return FIXED_VALUE == header->fixed_value;
}

static bool validate_header_checksum(const struct GBA_CartHeader* header) {
    enum { OFFSET = 0xA0, SIZE = 0x1D };

    const uint8_t* data = ((const uint8_t*)header) + OFFSET;
    uint8_t hash = -0x19;

    for (size_t i = 0; i < SIZE; ++i) {
        hash -= data[i];
    }

    return hash == header->complement_check;
}

int GBA_loadrom_data(struct GBA_Core* gba, const uint8_t* data, size_t size) {
    assert(gba && data && size);

    if (size > MAX_ROM_SIZE) {
        fprintf(stderr, "[ERROR] file-size is too big! got: 0x%llX want: 0x%X\n", size, MAX_ROM_SIZE);
        return -1;
    }

    if (size < sizeof(struct GBA_CartHeader)) {
        fprintf(stderr, "[ERROR] file-size is too small for header! got: 0x%llX want: 0x%llX\n", size, sizeof(struct GBA_CartHeader));
        return -1;
    }

    // the header starts at the beginning of the rom!
    const struct GBA_CartHeader* header = (const struct GBA_CartHeader*)data;
    // log the header to stdout
    print_header(header);

    if (!validate_header_fixed_value(header)) {
        fprintf(stderr, "[ERROR] invalid header fixed_value!\n");
        return -1;
    }

    if (!validate_header_checksum(header)) {
        fprintf(stderr, "[ERROR] invalid header checksum!\n");
        return -1;
    }

    // todo:
    // - check header is valid
    // - save rom size for mask!
    // - get sram type (if any)
    // - load sram
    // - validate entire rom hash
    // - validate nintendo logo (using my own hash)

    // reset the system, ready to load the new game.
    GBA_reset(gba);

    // load the rom data!
    memcpy(gba->mmio.rom, data, size);

    return 0;
}

void GBA_run_frame(struct GBA_Core* gba) {
    assert(gba);

}
