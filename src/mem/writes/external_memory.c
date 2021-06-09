#include "mem/writes/external_memory.h"
#include "mem/mmio.h"


/*
External Memory (Game Pak)

  08000000-09FFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 0
  0A000000-0BFFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 1
  0C000000-0DFFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 2
  0E000000-0E00FFFF   Game Pak SRAM    (max 64 KBytes) - 8bit Bus width
  0E010000-0FFFFFFF   Not used
*/

void sram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value)
{
    if (addr >= SRAM_BEGIN && addr <= SRAM_END)
    {
        write_array8(MMIO.sram, addr, value, SRAM_MASK);
    }
}

void sram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value)
{
    if (addr >= SRAM_BEGIN && addr <= SRAM_END)
    {
        write_array16(MMIO.sram, addr, value, SRAM_MASK);
    }
}

void sram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value)
{
    if (addr >= SRAM_BEGIN && addr <= SRAM_END)
    {
        write_array32(MMIO.sram, addr, value, SRAM_MASK);
    }
}
