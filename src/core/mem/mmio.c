#include "core/gba.h"
#include "core/internal.h"
#include "core/mem/common.h"
#include "core/mem/reads/general_interal_memory.h"
#include "core/mem/reads/internal_display_memory.h"
#include "core/mem/reads/external_memory.h"
#include "core/mem/writes/general_interal_memory.h"
#include "core/mem/writes/internal_display_memory.h"
#include "core/mem/writes/external_memory.h"

#include <stdio.h>
#include <assert.h>

// SOURCE: https://problemkaputt.de/gbatek.htm#gbamemorymap

/* reads */
uint8_t GBA_read8(struct GBA_Core* gba, const uint32_t addr) {
    switch ((addr >> 24) & 0xFF) {
        // General Internal Memory
        case 0x0: return bios_read8(gba, addr);
        case 0x2: return ewram_read8(gba, addr);
        case 0x3: return iwram_read8(gba, addr);
        case 0x4: return 0xFF;
        
        // Internal Display Memory
        case 0x5: return palette_ram_read8(gba, addr);
        case 0x6: return vram_read8(gba, addr);
        case 0x7: return oam_read8(gba, addr);
        
        // External Memory (Game Pak)
        case 0x8: case 0x9: return rom_read8(gba, addr);
        case 0xA: case 0xB: return rom_read8(gba, addr);
        case 0xC: case 0xD: return rom_read8(gba, addr);
        case 0xE: return sram_read8(gba, addr);

        // Unused Memory Area
        default: return 0xFF;
    }
}

uint16_t GBA_read16(struct GBA_Core* gba, const uint32_t addr) {
    switch ((addr >> 24) & 0xFF) {
        // General Internal Memory
        case 0x0: return bios_read16(gba, addr);
        case 0x2: return ewram_read16(gba, addr);
        case 0x3: return iwram_read16(gba, addr);
        case 0x4: return 0xFF;
        
        // Internal Display Memory
        case 0x5: return palette_ram_read16(gba, addr);
        case 0x6: return vram_read16(gba, addr);
        case 0x7: return oam_read16(gba, addr);
        
        // External Memory (Game Pak)
        case 0x8: case 0x9: return rom_read16(gba, addr);
        case 0xA: case 0xB: return rom_read16(gba, addr);
        case 0xC: case 0xD: return rom_read16(gba, addr);
        case 0xE: return sram_read16(gba, addr);

        // Unused Memory Area
        default: return 0xFF;
    }
}

uint32_t GBA_read32(struct GBA_Core* gba, const uint32_t addr) {
    switch ((addr >> 24) & 0xFF) {
        // General Internal Memory
        case 0x0: return bios_read32(gba, addr);
        case 0x2: return ewram_read32(gba, addr);
        case 0x3: return iwram_read32(gba, addr);
        case 0x4: return 0xFF;
        
        // Internal Display Memory
        case 0x5: return palette_ram_read32(gba, addr);
        case 0x6: return vram_read32(gba, addr);
        case 0x7: return oam_read32(gba, addr);
        
        // External Memory (Game Pak)
        case 0x8: case 0x9: return rom_read32(gba, addr);
        case 0xA: case 0xB: return rom_read32(gba, addr);
        case 0xC: case 0xD: return rom_read32(gba, addr);
        case 0xE: return sram_read32(gba, addr);

        // Unused Memory Area
        default: return 0xFF;
    }
}

/* writes */
void GBA_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value) {
    switch ((addr >> 24) & 0xFF) {
        // General Internal Memory
        case 0x2: ewram_write8(gba, addr, value); break;
        case 0x3: iwram_write8(gba, addr, value); break;
        case 0x4: // 0xFF;
        
        // Internal Display Memory
        case 0x5: palette_ram_write8(gba, addr, value); break;
        case 0x6: vram_write8(gba, addr, value); break;
        case 0x7: oam_write8(gba, addr, value); break;
        
        // External Memory (Game Pak)
        case 0xE: sram_write8(gba, addr, value); break;
    }
}

void GBA_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value) {
    switch ((addr >> 24) & 0xFF) {
        // General Internal Memory
        case 0x2: ewram_write16(gba, addr, value); break;
        case 0x3: iwram_write16(gba, addr, value); break;
        case 0x4: // 0xFF;
        
        // Internal Display Memory
        case 0x5: palette_ram_write16(gba, addr, value); break;
        case 0x6: vram_write16(gba, addr, value); break;
        case 0x7: oam_write16(gba, addr, value); break;
        
        // External Memory (Game Pak)
        case 0xE: sram_write16(gba, addr, value); break;
    }
}

void GBA_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value) {
    switch ((addr >> 24) & 0xFF) {
        // General Internal Memory
        case 0x2: ewram_write32(gba, addr, value); break;
        case 0x3: iwram_write32(gba, addr, value); break;
        case 0x4: // 0xFF;
        
        // Internal Display Memory
        case 0x5: palette_ram_write32(gba, addr, value); break;
        case 0x6: vram_write32(gba, addr, value); break;
        case 0x7: oam_write32(gba, addr, value); break;
        
        // External Memory (Game Pak)
        case 0xE: sram_write32(gba, addr, value); break;
    }
}
