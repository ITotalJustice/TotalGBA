#include "ppu.h"
#include "internal.h"


#define PPU gba->ppu


enum PpuMode
{
    PpuMode_VISIBLE,
    PpuMode_HBLANK,
    PpuMode_VBLANK,
};

enum
{
    VISIBLE_CYCLES = 960,
    HBLANK_CYCLES = 272,
    VBLANK_CYCLES = VISIBLE_CYCLES + HBLANK_CYCLES, // 1232
};

enum
{
    VISIBLE_SCANLINES = 160,
    VBLANK_SCANLINES = 68,
    TOTAL_SCANLINES = VISIBLE_SCANLINES + VBLANK_SCANLINES,
};

void ppu_ly_lyc_compare(struct GBA_Core* gba)
{
    if (IO_VCOUNT.value == IO_DISPSTAT.lyc)
    {
        IO_DISPSTAT.vcounter = true;

        if (IO_DISPSTAT.vcounter_irq)
        {
            GBA_irq(gba, InterruptFlag_LYC);
        }
    }
    else
    {
        IO_DISPSTAT.vcounter = false;
    }
}

static void ppu_set_ly(struct GBA_Core* gba, uint8_t new_ly)
{
    IO_VCOUNT.value = new_ly;
    ppu_ly_lyc_compare(gba);
}

static void ppu_on_visible(struct GBA_Core* gba)
{
    IO_DISPSTAT.hblank = false;
    IO_DISPSTAT.vblank = false;
}

static void ppu_on_hblank(struct GBA_Core* gba)
{
    IO_DISPSTAT.hblank = true;

    if (IO_DISPSTAT.hblank_irq)
    {
        GBA_irq(gba, InterruptFlag_HBLANK);
    }
}

static void ppu_on_vblank(struct GBA_Core* gba)
{
    IO_DISPSTAT.vblank = true;
    IO_DISPSTAT.hblank = false;

    if (IO_DISPSTAT.vblank_irq)
    {
        GBA_irq(gba, InterruptFlag_VBLANK);
    }
}

static void ppu_on_switch_mode(struct GBA_Core* gba, enum PpuMode new_mode)
{
    const uint16_t cycles[] =
    {
        [PpuMode_VISIBLE]   = VISIBLE_CYCLES,
        [PpuMode_HBLANK]    = HBLANK_CYCLES,
        [PpuMode_VBLANK]    = HBLANK_CYCLES, // we still need to set hblank flag!
    };

    switch (new_mode)
    {
        case PpuMode_VISIBLE: ppu_on_visible(gba); break;
        case PpuMode_HBLANK: ppu_on_hblank(gba); break;
        case PpuMode_VBLANK: ppu_on_vblank(gba); break;
    }

    PPU.next_cycles += cycles[new_mode];
    PPU.mode = new_mode;
}

// basically same idea as my GB ppu
void ppu_run(struct GBA_Core* gba, uint16_t cycles)
{
    PPU.next_cycles -= cycles;

    if (PPU.next_cycles > 0)
    {
        return;
    }

    switch (PPU.mode)
    {
        case PpuMode_VISIBLE:
            ppu_on_switch_mode(gba, PpuMode_HBLANK);
            break;

        case PpuMode_HBLANK:
            ppu_set_ly(gba, IO_VCOUNT.value + 1);

            if (IO_VCOUNT.value == VISIBLE_SCANLINES)
            {
                ppu_on_switch_mode(gba, PpuMode_VBLANK);
            }
            else
            {
                ppu_on_switch_mode(gba, PpuMode_VISIBLE);
            }
            break;

        case PpuMode_VBLANK:
            // check if we already in hblank (meaning we have fin a line)
            // or if we are now entering hblank.
            if (IO_DISPSTAT.hblank)
            {
                if (IO_VCOUNT.value == TOTAL_SCANLINES)
                {
                    ppu_set_ly(gba, 0);
                    ppu_on_switch_mode(gba, PpuMode_VISIBLE);
                }
                else
                {
                    ppu_set_ly(gba, IO_VCOUNT.value + 1);
                    PPU.next_cycles += HBLANK_CYCLES;
                }
            }
            else
            {
                ppu_on_hblank(gba);
                PPU.next_cycles += VBLANK_CYCLES - HBLANK_CYCLES;
            }
            break;
    }
}

void ppu_init(struct GBA_Core* gba)
{
    ppu_on_switch_mode(gba, PpuMode_VISIBLE);
}
