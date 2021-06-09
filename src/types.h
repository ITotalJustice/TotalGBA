#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


#ifndef GBA_DEBUG
    #define GBA_DEBUG 0
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_LIB
    #define GBAAPI __declspec(dllexport)
  #else
    #define GBAAPI __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_LIB
      #define GBAAPI __attribute__ ((visibility ("default")))
  #else
      #define GBAAPI
  #endif
#endif

#ifndef GBA_SINGLE_FILE
    #define GBA_SINGLE_FILE 0
#endif

#if GBA_SINGLE_FILE
    #define GBA_STATIC static
    // todo: detect other compilers force inline stuff (__forceinline)
    #define GBA_INLINE static inline __attribute__((always_inline))
#else
    #define GBA_STATIC
    #define GBA_INLINE
#endif // GB_SINGLE_FILE

// fwd declare structs
struct GBA_Core;


enum GBA_Button
{
    GBA_BUTTON_A        = 1 << 0,
    GBA_BUTTON_B        = 1 << 1,
    GBA_BUTTON_SELECT   = 1 << 2,
    GBA_BUTTON_START    = 1 << 3,

    GBA_BUTTON_RIGHT    = 1 << 4,
    GBA_BUTTON_LEFT     = 1 << 5,
    GBA_BUTTON_UP       = 1 << 6,
    GBA_BUTTON_DOWN     = 1 << 7
};

#define CART_HEADER_SIZE 0xC0
#define CART_HEADER_FIXED_VALUE 0x96
#define CART_HEADER_CHECKSUM_OFFSET 0xA0
#define CART_HEADER_CHECKSUM_SIZE 0x1D

enum GBA_HeaderLanguageCode
{
    LANGUAGE_CODE_Japan,
    LANGUAGE_CODE_Europe,
    LANGUAGE_CODE_French,
    LANGUAGE_CODE_Spanish,
    LANGUAGE_CODE_USA,
    LANGUAGE_CODE_German,
    LANGUAGE_CODE_Italian
};

// todo: remove ALL bitfields, theres no reason to use them...

// LCD I/O Display Control 
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiodisplaycontrol
struct IO_DISPCNT
{
    uint8_t bg_mode;
    bool cgb_mode;
    bool display_frame_select;
    bool hblank_interval_free;
    bool obj_character_vram_mapping;
    bool forced_blank;
    bool screen_display_bg0;
    bool screen_display_bg1;
    bool screen_display_bg2;
    bool screen_display_bg3;
    bool screen_display_obj;
    bool window_0_display_flag;
    bool window_1_display_flag;
    bool obj_window_display_flag;
};

// LCD I/O Interrupts and Status
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiointerruptsandstatus
struct IO_DISPSTAT
{
    bool vblank_flag;
    bool hblank_flag;
    bool vcounter_flag;
    bool vblank_irq_enable;
    bool hblank_irq_enable;
    bool vcounter_irq_enable;
    uint8_t vcount_setting;
};

// LCD I/O BG Control
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiobgcontrol
struct IO_BG0CNT
{
    uint8_t bg_priority;
    uint8_t char_base_block;
    bool mosaic;
    bool colours_palettes;
    uint8_t screen_base_block;
    bool display_area_overflow;
    uint8_t screen_size;
};

// LCD I/O BG Scrolling
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiobgscrolling
struct IO_BG0HOFS
{
    uint16_t offset;
};

// LCD I/O BG Rotation/Scaling
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiobgrotationscaling
struct IO_BG2X_L
{
    uint32_t fractional : 8;
    uint32_t interger : 19;
    uint32_t sign : 1;
};

struct IO_BG2PA
{
    uint16_t fractional : 8;
    uint16_t interger : 7;
    uint16_t sign : 1;
};

// LCD I/O Window Feature
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiowindowfeature
struct IO_WIN0H
{
    uint16_t x2 : 8;
    uint16_t x1 : 8;
};

struct IO_WIN0V
{
    uint16_t y2 : 8;
    uint16_t y1 : 8;
};

struct IO_WININ
{
    bool window0_bg0_enable;
    bool window0_bg1_enable;
    bool window0_bg2_enable;
    bool window0_bg3_enable;
    bool window0_obj_enable;
    bool window0_colour_special_effect;
    bool window1_bg0_enable;
    bool window1_bg1_enable;
    bool window1_bg2_enable;
    bool window1_bg3_enable;
    bool window1_obj_enable;
    bool window1_colour_special_effect;
};

struct IO_WINOUT
{
    bool outside_bg0_enable;
    bool outside_bg1_enable;
    bool outside_bg2_enable;
    bool outside_bg3_enable;
    bool outside_obj_enable;
    bool outside_colour_special_effect;
    bool obj_window_bg0_enable;
    bool obj_window_bg1_enable;
    bool obj_window_bg2_enable;
    bool obj_window_bg3_enable;
    bool obj_window_obj_enable;
    bool obj_window_colour_special_effect;
};

// LCD I/O Mosaic Function
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiomosaicfunction
struct IO_MOSAIC
{
    uint16_t bg_h_size : 4;
    uint16_t bg_v_size : 4;
    uint16_t obj_h_size : 4;
    uint16_t obj_v_size : 4;
};

// LCD I/O Color Special Effects
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiocolorspecialeffects
struct IO_BLDCNT
{
    bool bg0_1st_target_pixel;
    bool bg1_1st_target_pixel;
    bool bg2_1st_target_pixel;
    bool bg3_1st_target_pixel;
    bool obj_1st_target_pixel;
    bool bd_1st_target_pixel;
    uint8_t colour_special_effect;
    bool bg0_2nd_target_pixel;
    bool bg1_2nd_target_pixel;
    bool bg2_2nd_target_pixel;
    bool bg3_2nd_target_pixel;
    bool obj_2nd_target_pixel;
    bool bd_2nd_target_pixel;
};

struct IO_BLDALPHA
{
    uint8_t eva_coefficient_1st;
    uint8_t eva_coefficient_2nd;
};

struct IO_BLDY
{
    uint8_t evy_coefficient : 4;
};

// GBA Sound Channel 1 - Tone & Sweep
// SOURCE: https://problemkaputt.de/gbatek.htm#gbasoundchannel1tonesweep
struct IO_SOUND1CNT_L
{
    uint16_t num_sweep_shift : 3;
    uint16_t sweep_freq_direction : 1;
    uint16_t sweep_time : 2;
};

struct IO_SOUND1CNT_H
{
    uint16_t sound_len : 6;
    uint16_t wave_pattern_duty : 2;
    uint16_t envelope_step_time : 3;
    uint16_t envelope_direction : 1;
    uint16_t initial_vol_of_envelope : 4;
};

struct IO_IME
{
    bool enable;
};

struct IO_IE
{
    bool lcd_vblank;
    bool lcd_hblank;
    bool lcd_vcounter_match;
    bool timer0_overflow;
    bool timer1_overflow;
    bool timer2_overflow;
    bool timer3_overflow;
    bool serial_communication;
    bool dma0;
    bool dma1;
    bool dma2;
    bool dma3;
    bool keypad;
    bool game_pak;
};

enum SramType
{
    SRAM_TYPE_NONE,

    SRAM_TYPE_EEPROM,
    SRAM_TYPE_SRAM,
    SRAM_TYPE_FLASH,
    SRAM_TYPE_FLASH512,
    SRAM_TYPE_FLASH1M,
};

struct GBA_CartHeader
{
    uint32_t rom_entry_point;
    uint8_t nintendo_logo[156];
    // uppercase ascii
    char game_title[12];
    char game_code[4];
    char maker_code[2];
    // this has to be 0x96
    uint8_t fixed_value;
    // should be 0x00
    uint8_t main_unit_code;
    uint8_t device_type;
    // should be all zero
    uint8_t _reserved_area[7];
    // should be 0
    uint8_t software_version;
    // header checksum
    uint8_t complement_check;
    // should be all zero
    uint8_t _reserved_area2[2];
};

struct GBA_Mmio
{
    // 32mb(max), 16-bus
    uint8_t rom[0x2000000];

    // 64kb(max), 8-bus
    uint8_t sram[0x10000];

    // 256kb, 16-bit bus
    uint8_t ewram[0x40000];
    
    // 32kb, 32-bit bus
    uint8_t iwram[0x8000];

    // 1kb, 16-bit
    uint8_t palette_ram[0x400];

    // 96kb, 16-bit bus
    uint8_t vram[0x18000];

    // 1kb, 32-bit
    uint8_t oam[0x400];

    // io stuff
    struct IO_DISPCNT DISPCNT;
    struct IO_DISPSTAT DISPSTAT;
    struct IO_BG0CNT BG0CNT;
    struct IO_BG0CNT BG1CNT;
    struct IO_BG0CNT BG2CNT;
    struct IO_BG0CNT BG3CNT;
    struct IO_BG0HOFS BG0HOFS;
    struct IO_BG0HOFS BG0VOFS;
    struct IO_BG0HOFS BG1HOFS;
    struct IO_BG0HOFS BG1VOFS;
    struct IO_BG0HOFS BG2HOFS;
    struct IO_BG0HOFS BG2VOFS;
    struct IO_BG0HOFS BG3HOFS;
    struct IO_BG0HOFS BG3VOFS;

    struct IO_IE IE;
    struct IO_IE IF;
    struct IO_IME IME;
};

struct GBA_Psr
{
    // condition flags
    bool N;     // negative, less than
    bool Z;     // zero
    bool C;     // carry, borrow, extend
    bool V;     // overflow

    // control
    bool I;     // IRQ disable (1=off,0=on)
    bool F;     // FIQ disable (1=off,0=on)
    bool T;     // state bit (1=thumb,0=thumb)
    uint8_t M;  // mode
};

enum
{
    REG_SP_INDEX = 13, // stack pointer
    REG_LR_INDEX = 14, // gets set to r15 during branch and links
    REG_PC_INDEX = 15, // bits 0-1 are zero in arm state, bit-0 is zero in thumb
};

struct GBA_Cpu
{
    uint32_t registers[16];
    struct GBA_Psr cpsr;
    struct GBA_Psr spsr;

    // todo: banked registers...
};

struct GBA_Cart
{
    size_t rom_size;
    size_t sram_size;
    enum SramType sram_type;
};

struct GBA_Core
{
    struct GBA_Cpu cpu;
    struct GBA_Mmio mmio;
    struct GBA_Cart cart;

    // #if GBA_DEBUG
    //     size_t ticks;
    // #endif
};

#ifdef __cplusplus
}
#endif
