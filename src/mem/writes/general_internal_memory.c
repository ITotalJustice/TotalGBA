#include "mem/writes/general_internal_memory.h"
#include "mem/mmio.h"
#include "util/bit.h"
#include "internal.h"


/*
General Internal Memory

  00000000-00003FFF   BIOS - System ROM         (16 KBytes)
  00004000-01FFFFFF   Not used
  02000000-0203FFFF   WRAM - On-board Work RAM  (256 KBytes) 2 Wait
  02040000-02FFFFFF   Not used
  03000000-03007FFF   WRAM - On-chip Work RAM   (32 KBytes)
  03008000-03FFFFFF   Not used
  04000000-040003FE   I/O Registers
  04000400-04FFFFFF   Not used
*/

static inline void dispcnt_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    switch (i & 1)
    {
        case 0:
            IO_DISPCNT.bg_mode = get_bit_range(0, 2, v);
            IO_DISPCNT.display_frame_select = is_bit_set(4, v);
            IO_DISPCNT.hblank_interval_free = is_bit_set(5, v);
            IO_DISPCNT.obj_character_vram_mapping = is_bit_set(6, v);
            IO_DISPCNT.forced_blank = is_bit_set(7, v);
            break;

        case 1:
            IO_DISPCNT.screen_display_bg0 = is_bit_set(0, v);
            IO_DISPCNT.screen_display_bg1 = is_bit_set(1, v);
            IO_DISPCNT.screen_display_bg2 = is_bit_set(2, v);
            IO_DISPCNT.screen_display_bg3 = is_bit_set(3, v);
            IO_DISPCNT.screen_display_obj = is_bit_set(4, v);
            IO_DISPCNT.window_0_display_flag = is_bit_set(5, v);
            IO_DISPCNT.window_1_display_flag = is_bit_set(6, v);
            IO_DISPCNT.obj_window_display_flag = is_bit_set(7, v);
            break;
    }

    if (i == 1)
    {
        GBA_log("\nIO_DISPCNT: WRITE\n");
        GBA_log("\tbg_mode: %u\n", IO_DISPCNT.bg_mode);
        GBA_log("\tcgb_mode: %u\n", IO_DISPCNT.cgb_mode);
        GBA_log("\tdisplay_frame_select: %u\n", IO_DISPCNT.display_frame_select);
        GBA_log("\thblank_interval_free: %u\n", IO_DISPCNT.hblank_interval_free);
        GBA_log("\tobj_character_vram_mapping: %u\n", IO_DISPCNT.obj_character_vram_mapping);
        GBA_log("\tforced_blank: %u\n", IO_DISPCNT.forced_blank);
        GBA_log("\tscreen_display_bg0: %u\n", IO_DISPCNT.screen_display_bg0);
        GBA_log("\tscreen_display_bg1: %u\n", IO_DISPCNT.screen_display_bg1);
        GBA_log("\tscreen_display_bg2: %u\n", IO_DISPCNT.screen_display_bg2);
        GBA_log("\tscreen_display_bg3: %u\n", IO_DISPCNT.screen_display_bg3);
        GBA_log("\tscreen_display_obj: %u\n", IO_DISPCNT.screen_display_obj);
        GBA_log("\twindow_0_display_flag: %u\n", IO_DISPCNT.window_0_display_flag);
        GBA_log("\twindow_1_display_flag: %u\n", IO_DISPCNT.window_1_display_flag);
        GBA_log("\tobj_window_display_flag: %u\n", IO_DISPCNT.obj_window_display_flag);
    }
}

static inline void dispstat_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    switch (i & 1)
    {
        case 0:
            IO_DISPSTAT.vblank_irq = is_bit_set(3, v);
            IO_DISPSTAT.hblank_irq = is_bit_set(4, v);
            IO_DISPSTAT.vcounter_irq = is_bit_set(5, v);
            break;

        case 1:
            IO_DISPSTAT.lyc = v;
            break;
    }
}

static inline void bgxcnt_write(struct BG0CNT* bg, bool bg2_3, uint8_t v, uint8_t i)
{
    switch (i & 1)
    {
        case 0:
            bg->bg_priority = get_bit_range(0, 1, v);
            bg->char_base_block = get_bit_range(2, 3, v);
            bg->mosaic = is_bit_set(6, v);
            bg->colours_palettes = is_bit_set(7, v);
            break;

        case 1:
            bg->screen_base_block = get_bit_range(0, 4, v);
            if (bg2_3)
            {
                bg->display_area_overflow = is_bit_set(5, v);
            }
            bg->screen_size = get_bit_range(6, 7, v);
            break;
    }
}

static inline void bg0cnt_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxcnt_write(&IO_BG0CNT, false, v, i);
}

static inline void bg1cnt_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxcnt_write(&IO_BG1CNT, false, v, i);
}

static inline void bg2cnt_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxcnt_write(&IO_BG2CNT, true, v, i);
}

static inline void bg3cnt_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxcnt_write(&IO_BG3CNT, true, v, i);
}

static inline void bgxhofs_write(struct BG0HOFS* bg, uint8_t v, uint8_t i)
{
    switch (i & 1)
    {
        case 0:
            bg->offset = v;
            break;

        case 1:
            bg->offset = (bg->offset & 0xFF) | ((v & 1) << 8);
            break;
    }
}

static inline void bg0hofs_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxhofs_write(&IO_BG0HOFS, v, i);
}

static inline void bg0vofs_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxhofs_write(&IO_BG0VOFS, v, i);
}

static inline void bg1hofs_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxhofs_write(&IO_BG1HOFS, v, i);
}

static inline void bg1vofs_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxhofs_write(&IO_BG1VOFS, v, i);
}

static inline void bg2hofs_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxhofs_write(&IO_BG2HOFS, v, i);
}

static inline void bg2vofs_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxhofs_write(&IO_BG2VOFS, v, i);
}

static inline void bg3hofs_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxhofs_write(&IO_BG3HOFS, v, i);
}

static inline void bg3vofs_write(struct GBA_Core* gba, uint8_t v, uint8_t i)
{
    bgxhofs_write(&IO_BG3VOFS, v, i);
}


void ewram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value)
{
    if (addr >= EWRAM_BEGIN && addr <= EWRAM_END)
    {
        write_array8(MMIO.ewram, addr, value, EWRAM_MASK);
    }
}

void ewram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value)
{
    if (addr >= EWRAM_BEGIN && addr <= EWRAM_END)
    {
        write_array16(MMIO.ewram, addr, value, EWRAM_MASK);
    }
}

void ewram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value)
{
    if (addr >= EWRAM_BEGIN && addr <= EWRAM_END)
    {
        write_array32(MMIO.ewram, addr, value, EWRAM_MASK);
    }
}

void iwram_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value)
{
    if (addr >= IWRAM_BEGIN && addr <= IWRAM_END)
    {
        write_array8(MMIO.iwram, addr, value, IWRAM_MASK);
    }
}

void iwram_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value)
{
    if (addr >= IWRAM_BEGIN && addr <= IWRAM_END)
    {
        write_array16(MMIO.iwram, addr, value, IWRAM_MASK);
    }
}

void iwram_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value)
{
    if (addr >= IWRAM_BEGIN && addr <= IWRAM_END)
    {
        write_array32(MMIO.iwram, addr, value, IWRAM_MASK);
    }
}

// NOTE: if accessing a 2byte io reg using a 32-bit access, then the r/w will
// "spill" over into the next address area.
void io_write8(struct GBA_Core* gba, const uint32_t addr, const uint8_t value)
{
    switch (addr)
    {
        case 0x4000000: dispcnt_write(gba, value, 0); break;
        case 0x4000001: dispcnt_write(gba, value, 1); break;
        case 0x4000002: break; // not used
        case 0x4000003: break; // not used
        case 0x4000004: dispstat_write(gba, value, 0); break;
        case 0x4000005: dispstat_write(gba, value, 1); break;
        case 0x4000006: break; // VCOUNT is read only
        case 0x4000007: break; // VCOUNT is read only
        
        case 0x4000008: bg0cnt_write(gba, value, 0); break;
        case 0x4000009: bg0cnt_write(gba, value, 1); break;
        case 0x400000A: bg1cnt_write(gba, value, 0); break;
        case 0x400000B: bg1cnt_write(gba, value, 1); break;
        case 0x400000C: bg2cnt_write(gba, value, 0); break;
        case 0x400000D: bg2cnt_write(gba, value, 1); break;
        case 0x400000E: bg3cnt_write(gba, value, 0); break;
        case 0x400000F: bg3cnt_write(gba, value, 1); break;

        case 0x4000010: bg0hofs_write(gba, value, 0); break;
        case 0x4000011: bg0hofs_write(gba, value, 1); break;
        case 0x4000012: bg0vofs_write(gba, value, 0); break;
        case 0x4000013: bg0vofs_write(gba, value, 1); break;
        case 0x4000014: bg1hofs_write(gba, value, 0); break;
        case 0x4000015: bg1hofs_write(gba, value, 1); break;
        case 0x4000016: bg1vofs_write(gba, value, 0); break;
        case 0x4000017: bg1vofs_write(gba, value, 1); break;
        case 0x4000018: bg2hofs_write(gba, value, 0); break;
        case 0x4000019: bg2hofs_write(gba, value, 1); break;
        case 0x400001A: bg2vofs_write(gba, value, 0); break;
        case 0x400001B: bg2vofs_write(gba, value, 1); break;
        case 0x400001C: bg3hofs_write(gba, value, 0); break;
        case 0x400001D: bg3hofs_write(gba, value, 1); break;
        case 0x400001E: bg3vofs_write(gba, value, 0); break;
        case 0x400001F: bg3vofs_write(gba, value, 1); break;

        case 0x4000208: gba->mmio.IME.enable = value & 1; break;
        case 0x4000209: break;
        case 0x400020A: break; // not used
        case 0x400020B: break; // not used

        default:
            GBA_log_fatal("[io write 8]\taddr: 0x%04X v: 0x%02X\n", addr, value);
            break;
    }
}

void io_write16(struct GBA_Core* gba, const uint32_t addr, const uint16_t value)
{
    GBA_log("[io write 16]\taddr: 0x%04X v: 0x%02X\n", addr, value);
    
    io_write8(gba, addr + 0, (value >> 0) & 0xFF);
    io_write8(gba, addr + 1, (value >> 8) & 0xFF);
}

void io_write32(struct GBA_Core* gba, const uint32_t addr, const uint32_t value)
{
    GBA_log("[io write 32]\taddr: 0x%04X v: 0x%02X\n", addr, value);

    io_write8(gba, addr + 0, (value >> 0x00) & 0xFF);
    io_write8(gba, addr + 1, (value >> 0x08) & 0xFF);
    io_write8(gba, addr + 2, (value >> 0x10) & 0xFF);
    io_write8(gba, addr + 3, (value >> 0x18) & 0xFF);
}
