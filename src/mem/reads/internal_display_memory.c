#include "mem/reads/internal_display_memory.h"
#include "mem/mmio.h"


/*
Internal Display Memory

  05000000-050003FF   BG/OBJ Palette RAM        (1 Kbyte)
  05000400-05FFFFFF   Not used
  06000000-06017FFF   VRAM - Video RAM          (96 KBytes)
  06018000-06FFFFFF   Not used
  07000000-070003FF   OAM - OBJ Attributes      (1 Kbyte)
  07000400-07FFFFFF   Not used

*/

uint8_t palette_ram_read8(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= PALETTE_RAM_BEGIN && addr <= PALETTE_RAM_END)
    {
        return read_array8(MMIO.palette_ram, addr, PALETTE_RAM_MASK);
    }
    return 0xFF; // unused
}

uint16_t palette_ram_read16(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= PALETTE_RAM_BEGIN && addr <= PALETTE_RAM_END)
    {
        return read_array16(MMIO.palette_ram, addr, PALETTE_RAM_MASK);
    }
    return 0xFF; // unused
}

uint32_t palette_ram_read32(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= PALETTE_RAM_BEGIN && addr <= PALETTE_RAM_END)
    {
        return read_array32(MMIO.palette_ram, addr, PALETTE_RAM_MASK);
    }
    return 0xFF; // unused
}

uint8_t vram_read8(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= VRAM_BEGIN && addr <= VRAM_END)
    {
        return read_array8(MMIO.vram, addr, VRAM_MASK);
    }
    return 0xFF; // unused
}

uint16_t vram_read16(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= VRAM_BEGIN && addr <= VRAM_END)
    {
        return read_array16(MMIO.vram, addr, VRAM_MASK);
    }
    return 0xFF; // unused
}

uint32_t vram_read32(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= VRAM_BEGIN && addr <= VRAM_END)
    {
        return read_array32(MMIO.vram, addr, VRAM_MASK);
    }
    return 0xFF; // unused
}

uint8_t oam_read8(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= OAM_BEGIN && addr <= OAM_END)
    {
        return read_array8(MMIO.oam, addr, OAM_MASK);
    }
    return 0xFF; // unused
}

uint16_t oam_read16(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= OAM_BEGIN && addr <= OAM_END)
    {
        return read_array16(MMIO.oam, addr, OAM_MASK);
    }
    return 0xFF; // unused
}

uint32_t oam_read32(struct GBA_Core* gba, uint32_t addr)
{
    if (addr <= OAM_END)
    {
        return read_array32(MMIO.oam, addr, OAM_MASK);
    }
    return 0xFF; // unused
}
