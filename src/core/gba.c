#include "core/gba.h"
#include "core/internal.h"

#include "core/arm7tdmi/arm/arm_instruction_table.h"
#include "core/arm7tdmi/thumb/thumb_instruction_table.h"
#include "core/arm7tdmi/thumb/encoding_types.h"

#include "core/util/string.h"

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

static enum SramType get_sram_type(const uint8_t* data, size_t size) {
    assert(data && size);

    static const struct {
        const char* str_id;
        const enum SramType type;
    } entries[] = {
        {
            .str_id = "EEPROM_V",
            .type = SRAM_TYPE_EEPROM
        },
        {
            .str_id = "SRAM_V",
            .type = SRAM_TYPE_SRAM
        },
        {
            .str_id = "FLASH_V",
            .type = SRAM_TYPE_FLASH
        },
        {
            .str_id = "FLASH512_V",
            .type = SRAM_TYPE_FLASH512
        },
        {
            .str_id = "FLASH1M_V",
            .type = SRAM_TYPE_FLASH1M
        }
    };

    const char* str = (const char*)data;

    for (size_t i = 0; i < ARRAY_SIZE(entries); ++i) {
        // there is a much faster way of doing this, but for now this works.
        const char* id = strstr_s(
            str, size, // rom data + size
            entries[i].str_id, strlen(entries[i].str_id) // entry + size
        );

        // check if we found it, if so, log it!
        if (id != NULL) {
            fprintf(stdout, "\n[INFO] found sram type! %s\n", entries[i].str_id);
            return entries[i].type;
        }
    }

    fprintf(stdout, "\n[INFO] failed to find a valid sram type...\n");
    return SRAM_TYPE_NONE;
}

int GBA_loadrom_data(struct GBA_Core* gba, const uint8_t* data, size_t rom_size) {
    assert(gba && data && rom_size);

    if (rom_size > MAX_ROM_SIZE) {
        fprintf(stderr, "[ERROR] rom_size is too big! got: 0x%llX want: 0x%X\n", rom_size, MAX_ROM_SIZE);
        return -1;
    }

    if (rom_size < sizeof(struct GBA_CartHeader)) {
        fprintf(stderr, "[ERROR] rom_size is too small for header! got: 0x%llX want: 0x%llX\n", rom_size, sizeof(struct GBA_CartHeader));
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
    // - load sram
    // - validate entire rom hash
    // - validate nintendo logo (using my own hash)
    // - validate entry point is in range

    // reset the system, ready to load the new game.
    GBA_reset(gba);

    // load the rom data!
    memcpy(gba->mmio.rom, data, rom_size);

    // save rom size
    gba->cart.rom_size = rom_size;
    // save sram type (if any)
    gba->cart.sram_type = get_sram_type(data, rom_size);
    // set entry point
    gba->cpu.registers[REG_PC_INDEX] = header->rom_entry_point;

    return 0;
}

void GBA_run_frame(struct GBA_Core* gba) {
    assert(gba);

}
