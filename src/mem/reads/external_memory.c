#include "mem/reads/external_memory.h"
#include "mem/mmio.h"

/*
External Memory (Game Pak)

  08000000-09FFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 0
  0A000000-0BFFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 1
  0C000000-0DFFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 2
  0E000000-0E00FFFF   Game Pak SRAM    (max 64 KBytes) - 8bit Bus width
  0E010000-0FFFFFFF   Not used
*/

/* rom reads */
uint8_t rom_read8(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= ROM1_BEGIN && addr <= ROM3_END)
    {
        return read_array8(MMIO.rom, addr, ROM_MASK);
    }
    return 0xFF; // unused
}

uint16_t rom_read16(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= ROM1_BEGIN && addr <= ROM3_END)
    {
        return read_array16(MMIO.rom, addr, ROM_MASK);
    }
    return 0xFF; // unused
}

uint32_t rom_read32(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= ROM1_BEGIN && addr <= ROM3_END)
    {
        return read_array32(MMIO.rom, addr, ROM_MASK);
    }
    return 0xFF; // unused
}

/* sram reads */
uint8_t sram_read8(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= SRAM_BEGIN && addr <= SRAM_END)
    {
        return read_array8(MMIO.sram, addr, SRAM_MASK);
    }
    return 0xFF; // unused
}

uint16_t sram_read16(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= SRAM_BEGIN && addr <= SRAM_END)
    {
        return read_array16(MMIO.sram, addr, SRAM_MASK);
    }
    return 0xFF; // unused
}

uint32_t sram_read32(struct GBA_Core* gba, uint32_t addr)
{
    if (addr >= SRAM_BEGIN && addr <= SRAM_END)
    {
        return read_array32(MMIO.sram, addr, SRAM_MASK);
    }
    return 0xFF; // unused
}

