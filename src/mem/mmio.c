#include "gba.h"
#include "internal.h"
#include "mem/mmio.h"
#include "mem/reads/general_internal_memory.h"
#include "mem/reads/internal_display_memory.h"
#include "mem/reads/external_memory.h"
#include "mem/writes/general_internal_memory.h"
#include "mem/writes/internal_display_memory.h"
#include "mem/writes/external_memory.h"

#include <assert.h>

// SOURCE: https://problemkaputt.de/gbatek.htm#gbamemorymap


uint8_t GBA_read8(struct GBA_Core* gba, const uint32_t addr)
{
    switch ((addr >> 24) & 0xFF)
    {
        // General Internal Memory
        case 0x0: return bios_read8(gba, addr);
        case 0x2: return ewram_read8(gba, addr);
        case 0x3: return iwram_read8(gba, addr);
        case 0x4: return io_read8(gba, addr);
        
        // Internal Display Memory
        case 0x5: return palette_ram_read8(gba, addr);
        case 0x6: return vram_read8(gba, addr);
        case 0x7: return oam_read8(gba, addr);
        
        // External Memory (Game Pak)
        case 0x8: case 0x9: return rom_read8(gba, addr);
        case 0xA: case 0xB: return rom_read8(gba, addr);
        case 0xC: case 0xD: return rom_read8(gba, addr);

        // 
        case 0xE: return sram_read8(gba, addr);

        // Unused Memory Area
        default: return 0xFF;
    }
}

uint16_t GBA_read16(struct GBA_Core* gba, const uint32_t addr)
{
    switch ((addr >> 24) & 0xFF)
    {
        // General Internal Memory
        case 0x0: return bios_read16(gba, addr);
        case 0x2: return ewram_read16(gba, addr);
        case 0x3: return iwram_read16(gba, addr);
        case 0x4: return io_read16(gba, addr);
        
        // Internal Display Memory
        case 0x5: return palette_ram_read16(gba, addr);
        case 0x6: return vram_read16(gba, addr);
        case 0x7: return oam_read16(gba, addr);
        
        // External Memory (Game Pak)
        case 0x8: case 0x9: return rom_read16(gba, addr);
        case 0xA: case 0xB: return rom_read16(gba, addr);
        case 0xC: case 0xD: return rom_read16(gba, addr);

        // 
        case 0xE: return sram_read16(gba, addr);

        // Unused Memory Area
        default: return 0xFF;
    }
}

uint32_t GBA_read32(struct GBA_Core* gba, const uint32_t addr)
{
    switch ((addr >> 24) & 0xFF)
    {
        // General Internal Memory
        case 0x0: return bios_read32(gba, addr);
        case 0x2: return ewram_read32(gba, addr);
        case 0x3: return iwram_read32(gba, addr);
        case 0x4: return io_read32(gba, addr);
        
        // Internal Display Memory
        case 0x5: return palette_ram_read32(gba, addr);
        case 0x6: return vram_read32(gba, addr);
        case 0x7: return oam_read32(gba, addr);
        
        // External Memory (Game Pak)
        case 0x8: case 0x9: return rom_read32(gba, addr);
        case 0xA: case 0xB: return rom_read32(gba, addr);
        case 0xC: case 0xD: return rom_read32(gba, addr);

        // 
        case 0xE: return sram_read32(gba, addr);

        // Unused Memory Area
        default: return 0xFF;
    }
}


void GBA_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value)
{
    assert(addr != 0x03000020);

    switch ((addr >> 24) & 0xFF)
    {
        // General Internal Memory
        case 0x2: ewram_write8(gba, addr, value); break;
        case 0x3: iwram_write8(gba, addr, value); break;
        case 0x4: io_write8(gba, addr, value); break;
        
        // Internal Display Memory
        case 0x5: palette_ram_write8(gba, addr, value); break;
        case 0x6: vram_write8(gba, addr, value); break;
        case 0x7: oam_write8(gba, addr, value); break;
        
        // External Memory (Game Pak)
        case 0xE: sram_write8(gba, addr, value); break;
    }
}

void GBA_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value)
{
    assert(addr != 0x03000020);
    
    switch ((addr >> 24) & 0xFF)
    {
        // General Internal Memory
        case 0x2: ewram_write16(gba, addr, value); break;
        case 0x3: iwram_write16(gba, addr, value); break;
        case 0x4: io_write16(gba, addr, value); break;
        
        // Internal Display Memory
        case 0x5: palette_ram_write16(gba, addr, value); break;
        case 0x6: vram_write16(gba, addr, value); break;
        case 0x7: oam_write16(gba, addr, value); break;
        
        // External Memory (Game Pak)
        case 0xE: sram_write16(gba, addr, value); break;
    }
}

void GBA_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value)
{
    switch ((addr >> 24) & 0xFF)
    {
        // General Internal Memory
        case 0x2: ewram_write32(gba, addr, value); break;
        case 0x3: iwram_write32(gba, addr, value); break;
        case 0x4: io_write32(gba, addr, value); break;
        
        // Internal Display Memory
        case 0x5: palette_ram_write32(gba, addr, value); break;
        case 0x6: vram_write32(gba, addr, value); break;
        case 0x7: oam_write32(gba, addr, value); break;
        
        // External Memory (Game Pak)
        case 0xE: sram_write32(gba, addr, value); break;
    }
}

uint8_t read_array8(const uint8_t* array, const uint32_t addr, const uint32_t mask)
{
    return array[addr & mask];
}

uint16_t read_array16(const uint8_t* array, const uint32_t addr, const uint32_t mask)
{
    const uint8_t hi = array[(addr + 0) & mask];
    const uint8_t lo = array[(addr + 1) & mask];

    return (lo << 8) | hi;
}

uint32_t read_array32(const uint8_t* array, const uint32_t addr, const uint32_t mask)
{
    const uint32_t hi_word_hi = array[(addr + 0) & mask];
    const uint32_t hi_word_lo = array[(addr + 1) & mask];
    const uint32_t lo_word_hi = array[(addr + 2) & mask];
    const uint32_t lo_word_lo = array[(addr + 3) & mask];

    return (lo_word_lo << 24) | (lo_word_hi << 16) | (hi_word_lo << 8) | hi_word_hi;
}

void write_array8(uint8_t* array, const uint32_t addr, const uint8_t value, const uint32_t mask)
{
    array[addr & mask] = value;
}

void write_array16(uint8_t* array, const uint32_t addr, const uint16_t value, const uint32_t mask)
{
    array[(addr + 0) & mask] = (value >> 0) & 0xFF;
    array[(addr + 1) & mask] = (value >> 8) & 0xFF;
}

void write_array32(uint8_t* array, const uint32_t addr, const uint32_t value, const uint32_t mask)
{
    array[(addr + 0) & mask] = (value >> 0) & 0xFF;
    array[(addr + 1) & mask] = (value >> 8) & 0xFF;
    array[(addr + 2) & mask] = (value >> 16) & 0xFF;
    array[(addr + 3) & mask] = (value >> 24) & 0xFF;
}
