#include "gba.h"
#include "internal.h"

#include "arm7tdmi/arm7tdmi.h"
#include "util/string.h"

#include <string.h>
#include <assert.h>


bool GBA_init(struct GBA_Core* gba)
{
    if (!gba)
    {
        return false;
    }
    
    memset(gba, 0, sizeof(struct GBA_Core));

    return true;
}

void GBA_reset(struct GBA_Core* gba)
{
    memset(gba, 0, sizeof(struct GBA_Core));

    ARM7_init(gba);
}

static void print_header(const struct GBA_CartHeader* header)
{
    GBA_log("\nGBA CART HEADER INFO\n");
        GBA_log("\trom_entry_point: 0x%08X\n", header->rom_entry_point);
        GBA_log("\tgame_title: %.*s\n", (int)sizeof(header->game_title), header->game_title);
        GBA_log("\tgame_code: %.*s\n", (int)sizeof(header->game_code), header->game_code);
        GBA_log("\tmaker_code: %.*s\n", (int)sizeof(header->maker_code), header->maker_code);
        GBA_log("\tfixed_value: 0x%02X\n", header->fixed_value);
        GBA_log("\tmain_unit_code: 0x%02X\n", header->main_unit_code);
        GBA_log("\tdevice_type: 0x%02X\n", header->device_type);
        GBA_log("\tsoftware_version: 0x%02X\n", header->software_version);
        GBA_log("\tcomplement_check: 0x%02X\n", header->complement_check);
    GBA_log("\n");
}

// static bool validate_header_logo(const struct GBA_CartHeader* header)
// {
//     return false;
// }

static bool validate_header_fixed_value(const struct GBA_CartHeader* header)
{
    enum { FIXED_VALUE = 0x96 };
    
    return FIXED_VALUE == header->fixed_value;
}

static bool validate_header_checksum(const struct GBA_CartHeader* header)
{
    enum { OFFSET = 0xA0, SIZE = 0x1D };

    const uint8_t* data = ((const uint8_t*)header) + OFFSET;
    uint8_t hash = -0x19;

    for (size_t i = 0; i < SIZE; ++i)
    {
        hash -= data[i];
    }

    return hash == header->complement_check;
}

static enum SramType get_sram_type(const uint8_t* data, size_t size)
{
    const struct
    {
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

    for (size_t i = 0; i < ARRAY_SIZE(entries); ++i)
    {
        // there is a much faster way of doing this, but for now this works.
        const char* id = strstr_s(
            str, size, // rom data + size
            entries[i].str_id, strlen(entries[i].str_id) // entry + size
        );

        // check if we found it, if so, log it!
        if (id != NULL)
        {
            GBA_log("\n[INFO] found sram type! %s\n", entries[i].str_id);
            return entries[i].type;
        }
    }

    GBA_log("\n[INFO] failed to find a valid sram type...\n");
    return SRAM_TYPE_NONE;
}

bool GBA_loadrom(struct GBA_Core* gba, const uint8_t* data, size_t rom_size)
{
    if (rom_size > MAX_ROM_SIZE)
    {
        GBA_log_err("[ERROR] rom_size is too big! got: %zu want: %d\n", rom_size, MAX_ROM_SIZE);
        return false;
    }

    if (rom_size < sizeof(struct GBA_CartHeader))
    {
        GBA_log_err("[ERROR] rom_size is too small for header! got: %zu want: %zu\n", rom_size, sizeof(struct GBA_CartHeader));
        return false;
    }

    // the header starts at the beginning of the rom!
    const struct GBA_CartHeader* header = (const struct GBA_CartHeader*)data;
    // log the header to stdout
    print_header(header);

    if (!validate_header_fixed_value(header))
    {
        GBA_log_err("[ERROR] invalid header fixed_value!\n");
        return false;
    }

    if (!validate_header_checksum(header))
    {
        GBA_log_err("[ERROR] invalid header checksum!\n");
        return false;
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

    return true;
}

void GBA_run_frame(struct GBA_Core* gba)
{
    // todo: count cycles!
    static int c = 0;
    static size_t ticks = 0;

    for (;;)
    {
        if (CPU.registers[15] == 0x08000130)
        {
            c++;
        }
        ARM7_run(gba);

        if (c > 0)
        GBA_log("  PC: 0x%08X LR: 0x%08X SP: 0x%08X R0: 0x%08X R1: 0x%08X TICKS: %zu Z: %u\n", CPU.registers[15], CPU.registers[14], CPU.registers[13], CPU.registers[0], CPU.registers[1], ticks, CPU.cpsr.Z);

        if (ticks > 196817)
        {
            assert(0 && "fin");
        }

        ticks++;
    }
}
