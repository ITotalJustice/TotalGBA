#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define GBA_LITTLE_ENDIAN 1
#define GBA_BIG_ENDIAN 2

// todo: add more compilers endianess detection.
#ifndef GBA_ENDIAN
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define GBA_ENDIAN GBA_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define GBA_ENDIAN GBA_BIG_ENDIAN
#endif
#endif /* GBA_ENDIAN */

#ifndef GBA_ENDIAN
#error GBA_ENDIAN IS NOT SET! UNABLE TO DEDUCE PLATFORM ENDIANESS
#endif /* GBA_ENDIAN */

// fwd declare structs
struct GBA_Core;


enum GBA_Button {
	GBA_BUTTON_A = 1 << 0,
    GBA_BUTTON_B = 1 << 1,
    GBA_BUTTON_SELECT = 1 << 2,
    GBA_BUTTON_START = 1 << 3,

    GBA_BUTTON_RIGHT = 1 << 4,
    GBA_BUTTON_LEFT = 1 << 5,
    GBA_BUTTON_UP = 1 << 6,
    GBA_BUTTON_DOWN = 1 << 7
};

enum GBA_ErrorType {
	GBA_ERROR_TYPE_UNKNOWN_INSTRUCTION,
	GBA_ERROR_TYPE_INFO,
	GBA_ERROR_TYPE_WARN,
	GBA_ERROR_TYPE_ERROR,
	GBA_ERROR_TYPE_UNK
};

struct GBA_UnkownInstructionTypeData {
	uint8_t opcode;
	bool cb_prefix;
};

enum GBA_ErrorDataType {
	GBA_ERROR_DATA_TYPE_UNK,
	GBA_ERROR_DATA_TYPE_NULL_PARAM,
	GBA_ERROR_DATA_TYPE_ROM,
	GBA_ERROR_DATA_TYPE_SRAM,
	GBA_ERROR_DATA_TYPE_SAVE,
};

struct GBA_InfoTypeData {
	char message[0x200]; // NULL terminated string
};

struct GBA_WarnTypeData {
	char message[0x200]; // NULL terminated string
};

struct GBA_ErrorTypeData {
	enum GBA_ErrorDataType type;
	char message[0x200]; // NULL terminated string
};

struct GBA_ErrorData {
	enum GBA_ErrorType type;
	union {
		struct GBA_UnkownInstructionTypeData unk_instruction;
		struct GBA_InfoTypeData info;
		struct GBA_WarnTypeData warn;
		struct GBA_ErrorTypeData error;
	};
};


// callbacks
typedef void(*GBA_error_callback_t)(struct GBA_Core* gb, void* user, struct GBA_ErrorData* e);

#define CART_HEADER_SIZE 0xC0
#define CART_HEADER_FIXED_VALUE 0x96
#define CART_HEADER_CHECKSUM_OFFSET 0xA0
#define CART_HEADER_CHECKSUM_SIZE 0x1D

enum GBA_HeaderLanguageCode {
    LANGUAGE_CODE_Japan,
    LANGUAGE_CODE_Europe,
    LANGUAGE_CODE_French,
    LANGUAGE_CODE_Spanish,
    LANGUAGE_CODE_USA,
    LANGUAGE_CODE_German,
    LANGUAGE_CODE_Italian
};

// LCD I/O Display Control 
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiodisplaycontrol
struct GBA_DISPCNT {
    uint16_t bg_mode : 3;
    uint16_t cgb_mode : 1;
    uint16_t display_frame_select : 1;
    uint16_t hblank_interval_free : 1;
    uint16_t obj_character_vram_mapping : 1;
    uint16_t forced_blank : 1;
    uint16_t screen_display_bg0 : 1;
    uint16_t screen_display_bg1 : 1;
    uint16_t screen_display_bg2 : 1;
    uint16_t screen_display_bg3 : 1;
    uint16_t screen_display_obj : 1;
    uint16_t window_0_display_flag : 1;
    uint16_t window_1_display_flag : 1;
    uint16_t obj_window_display_flag : 1;
};

// LCD I/O Interrupts and Status
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiointerruptsandstatus
struct DISPSTAT {
    uint16_t vblank_flag : 1;
    uint16_t hblank_flag : 1;
    uint16_t vcounter_flag : 1;
    uint16_t vblank_irq_enable : 1;
    uint16_t hblank_irq_enable : 1;
    uint16_t vcounter_irq_enable : 1;
    uint16_t vcount_setting : 8;
};

// LCD I/O BG Control
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiobgcontrol
struct BG0CNT {
    uint16_t bg_priority : 2;
    uint16_t char_base_block : 2;
    uint16_t mosaic : 1;
    uint16_t colours_palettes : 1;
    uint16_t screen_base_block : 5;
    uint16_t display_area_overflow : 1;
    uint16_t screen_size : 2;
};

// LCD I/O BG Scrolling
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiobgscrolling
struct BG0HOFS {
    uint16_t value : 9;
};

// LCD I/O BG Rotation/Scaling
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiobgrotationscaling
struct BG2X_L {
    uint32_t fractional : 8;
    uint32_t interger : 19;
    uint32_t sign : 1;
};

struct BG2PA {
    uint16_t fractional : 8;
    uint16_t interger : 7;
    uint16_t sign : 1;
};

// LCD I/O Window Feature
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiowindowfeature
struct WIN0H {
    uint16_t x2 : 8;
    uint16_t x1 : 8;
};

struct WIN0V {
    uint16_t y2 : 8;
    uint16_t y1 : 8;
};

struct WININ {
    uint16_t window0_bg0_enable : 1;
    uint16_t window0_bg1_enable : 1;
    uint16_t window0_bg2_enable : 1;
    uint16_t window0_bg3_enable : 1;
    uint16_t window0_obj_enable : 1;
    uint16_t window0_colour_special_effect : 1;
    uint16_t window1_bg0_enable : 1;
    uint16_t window1_bg1_enable : 1;
    uint16_t window1_bg2_enable : 1;
    uint16_t window1_bg3_enable : 1;
    uint16_t window1_obj_enable : 1;
    uint16_t window1_colour_special_effect : 1;
};

struct WINOUT {
    uint16_t outside_bg0_enable : 1;
    uint16_t outside_bg1_enable : 1;
    uint16_t outside_bg2_enable : 1;
    uint16_t outside_bg3_enable : 1;
    uint16_t outside_obj_enable : 1;
    uint16_t outside_colour_special_effect : 1;
    uint16_t obj_window_bg0_enable : 1;
    uint16_t obj_window_bg1_enable : 1;
    uint16_t obj_window_bg2_enable : 1;
    uint16_t obj_window_bg3_enable : 1;
    uint16_t obj_window_obj_enable : 1;
    uint16_t obj_window_colour_special_effect : 1;
};

// LCD I/O Mosaic Function
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiomosaicfunction
struct MOSAIC {
    uint16_t bg_h_size : 4;
    uint16_t bg_v_size : 4;
    uint16_t obj_h_size : 4;
    uint16_t obj_v_size : 4;
};

// LCD I/O Color Special Effects
// SOURCE: https://problemkaputt.de/gbatek.htm#lcdiocolorspecialeffects
struct BLDCNT {
    uint16_t bg0_1st_target_pixel : 1;
    uint16_t bg1_1st_target_pixel : 1;
    uint16_t bg2_1st_target_pixel : 1;
    uint16_t bg3_1st_target_pixel : 1;
    uint16_t obj_1st_target_pixel : 1;
    uint16_t bd_1st_target_pixel : 1;
    uint16_t colour_special_effect : 2;
    uint16_t bg0_2nd_target_pixel : 1;
    uint16_t bg1_2nd_target_pixel : 1;
    uint16_t bg2_2nd_target_pixel : 1;
    uint16_t bg3_2nd_target_pixel : 1;
    uint16_t obj_2nd_target_pixel : 1;
    uint16_t bd_2nd_target_pixel : 1;
};

struct BLDALPHA {
    uint16_t eva_coefficient_1st : 4;
    uint16_t eva_coefficient_2nd : 4;
};

struct BLDY {
    uint8_t evy_coefficient : 4;
};

// GBA Sound Channel 1 - Tone & Sweep
// SOURCE: https://problemkaputt.de/gbatek.htm#gbasoundchannel1tonesweep
struct SOUND1CNT_L {
    uint16_t num_sweep_shift : 3;
    uint16_t sweep_freq_direction : 1;
    uint16_t sweep_time : 2;
};

struct SOUND1CNT_H {
    uint16_t sound_len : 6;
    uint16_t wave_pattern_duty : 2;
    uint16_t envelope_step_time : 3;
    uint16_t envelope_direction : 1;
    uint16_t initial_vol_of_envelope : 4;
};

struct IME {
    uint8_t value : 1;
};

struct IE {
    uint16_t lcd_vblank : 1;
    uint16_t lcd_hblank : 1;
    uint16_t lcd_vcounter_match : 1;
    uint16_t timer0_overflow : 1;
    uint16_t timer1_overflow : 1;
    uint16_t timer2_overflow : 1;
    uint16_t timer3_overflow : 1;
    uint16_t serial_communication : 1;
    uint16_t dma0 : 1;
    uint16_t dma1 : 1;
    uint16_t dma2 : 1;
    uint16_t dma3 : 1;
    uint16_t keypad : 1;
    uint16_t game_pak : 1;
};

enum SramType {
    SRAM_TYPE_NONE,

    SRAM_TYPE_EEPROM,
    SRAM_TYPE_SRAM,
    SRAM_TYPE_FLASH,
    SRAM_TYPE_FLASH512,
    SRAM_TYPE_FLASH1M,
};

struct GBA_CartHeader {
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

struct GBA_Mmio {
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
};

struct GBA_Psr {
    // condition flags
    uint8_t N : 1; // negative, less than
    uint8_t Z : 1; // zero
    uint8_t C : 1; // carry, borrow, extend
    uint8_t V : 1; // overflow

    // control
    uint8_t I : 1; // IRQ disable (1=off,0=on)
    uint8_t F : 1; // FIQ disable (1=off,0=on)
    uint8_t T : 1; // state bit (1=thumb,0=thumb)
    uint8_t M : 5; // mode
};

enum {
    REG_SP_INDEX = 13, // stack pointer
    REG_LR_INDEX = 14, // gets set to r15 during branch and links
    REG_PC_INDEX = 15, // bits 0-1 are zero in arm state, bit-0 is zero in thumb
};

struct GBA_Cpu {
    uint32_t registers[16];
    struct GBA_Psr cpsr;
    struct GBA_Psr spsr;

    // todo: banked registers...
};

struct GBA_Cart {
    size_t rom_size;
    size_t sram_size;
    enum SramType sram_type;
};

struct GBA_Core {
    struct GBA_Cpu cpu;
    struct GBA_Mmio mmio;
    struct GBA_Cart cart;
};

#ifdef __cplusplus
}
#endif
