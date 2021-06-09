#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


#if GBA_DEBUG
    #define R15_CHECK(v) if (v == REG_PC_INDEX) { GBA_log_fatal("R15 in %s\n", __func__); }
#else
    #define R15_CHECK(v)
#endif

#define CPU gba->cpu


enum Arm7tdmiMode
{
    MODE_USER       = 16,
    MODE_FIQ        = 17,
    MODE_IRQ        = 18,
    MODE_SUPERVISOR = 19,
    MODE_ABORT      = 23,
    MODE_UNDEFINED  = 27,
    MODE_SYSTEM     = 31,
};

enum Arm7tdmiState
{
    STATE_ARM   = 0,
    STATE_THUMB = 1,
};

enum Arm7tdmiExceptionVectors
{
    EXCEPTION_RESET                 = 0x00000000,
    EXCEPTION_UNDEFINED_INSTRUCTION = 0x00000004,
    EXCEPTION_SOFTWARE_INTERRUPT    = 0x00000008,
    EXCEPTION_ABORT_PREFETCH        = 0x0000000C,
    EXCEPTION_ABORT_DATA            = 0x00000010,
    EXCEPTION_RESERVED              = 0x00000014,
    EXCEPTION_IRQ                   = 0x00000018,
    EXCEPTION_FIQ                   = 0x0000001C,
};

enum Arm7tdmiCondition
{
    COND_EQ = 0x0, // Z set
    COND_NE = 0x1, // Z clear
    COND_CS = 0x2, // C set
    COND_CC = 0x3, // C clear
    COND_MI = 0x4, // N set
    COND_PL = 0x5, // N clear
    COND_VS = 0x6, // V set
    COND_VC = 0x7, // V clear
    COND_HI = 0x8, // C set and Z clear
    COND_LS = 0x9, // C clear or Z set
    COND_GE = 0xA, // N equals V
    COND_LT = 0xB, // N not equal to V
    COND_GT = 0xC, // Z clear and N equals V
    COND_LE = 0xD, // Z set or N not equal to V
    COND_AL = 0xE, // ignored (AL = always)
};

enum Arm7tdmiFlag
{
    FLAG_N, // negative
    FLAG_Z, // zero
    FLAG_C, // carry
    FLAG_V, // overflow
};


GBA_INLINE uint32_t ARM7_psr_to_u32(const struct GBA_Psr* psr);
GBA_INLINE void ARM7_u32_to_psr(struct GBA_Psr* psr, uint32_t value);

GBA_INLINE enum Arm7tdmiState ARM7_get_state(const struct GBA_Core* gba);
GBA_INLINE void ARM7_set_state(struct GBA_Core* gba, const enum Arm7tdmiState state);
GBA_INLINE bool ARM7_is_state(const struct GBA_Core* gba, const enum Arm7tdmiState wanted);
GBA_INLINE uint8_t ARM7_get_mode(const struct GBA_Core* gba);
GBA_INLINE void ARM7_set_mode(struct GBA_Core* gba, const enum Arm7tdmiMode mode);
GBA_INLINE bool ARM7_get_flag(const struct GBA_Core* gba, const enum Arm7tdmiFlag flag);
GBA_INLINE void ARM7_set_flag(struct GBA_Core* gba, const enum Arm7tdmiFlag flag, const bool value);
GBA_INLINE void ARM7_set_flags_nzc(struct GBA_Core* gba, const bool n, const bool z, const bool c);
GBA_INLINE void ARM7_set_flags_nzcv(struct GBA_Core* gba, const bool n, const bool z, const bool c, const bool v);
GBA_INLINE bool ARM7_get_cond(const struct GBA_Core* gba, const enum Arm7tdmiCondition cond);

GBA_STATIC void ARM7_init(struct GBA_Core* gba);
GBA_INLINE void ARM7_run(struct GBA_Core* gba);

#ifdef __cplusplus
}
#endif
