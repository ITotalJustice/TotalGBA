#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/internal.h"


#define CPU gba->cpu


enum Arm7tdmiMode {
    MODE_USER = 16,
    MODE_FIQ = 17,
    MODE_IRQ = 18,
    MODE_SUPERVISOR = 19,
    MODE_ABORT = 23,
    MODE_UNDEFINED = 27,
    MODE_SYSTEM = 31,
};

enum Arm7tdmiState {
    STATE_ARM = 0,
    STATE_THUMB = 1,
};

enum Arm7tdmiExceptionVectors {
    EXCEPTION_RESET = 0x00000000,
    EXCEPTION_UNDEFINED_INSTRUCTION = 0x00000004,
    EXCEPTION_SOFTWARE_INTERRUPT = 0x00000008,
    EXCEPTION_ABORT_PREFETCH = 0x0000000C,
    EXCEPTION_ABORT_DATA = 0x00000010,
    EXCEPTION_RESERVED = 0x00000014,
    EXCEPTION_IRQ = 0x00000018,
    EXCEPTION_FIQ = 0x0000001C,
};

enum Arm7tdmiCondition {
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

enum Arm7tdmiFlag {
    FLAG_N, // negative
    FLAG_Z, // zero
    FLAG_C, // carry
    FLAG_V, // overflow
};


static inline bool ARM7_get_flag(const struct GBA_Core* gba, const enum Arm7tdmiFlag flag) {
    switch (flag) {
        case FLAG_N: return CPU.cpsr.N;
        case FLAG_Z: return CPU.cpsr.N;
        case FLAG_C: return CPU.cpsr.N;
        case FLAG_V: return CPU.cpsr.N; 
    }

    UNREACHABLE(0);
}

static inline void ARM7_set_flag(struct GBA_Core* gba, const enum Arm7tdmiFlag flag, const bool value) {
    switch (flag) {
        case FLAG_N: CPU.cpsr.N = value; break;
        case FLAG_Z: CPU.cpsr.N = value; break;
        case FLAG_C: CPU.cpsr.N = value; break;
        case FLAG_V: CPU.cpsr.N = value; break; 
    }
}

static inline bool ARM7_get_cond(const struct GBA_Core* gba, const enum Arm7tdmiCondition cond) {
    switch (cond) {
        case COND_EQ: return CPU.cpsr.Z;
        case COND_NE: return !CPU.cpsr.Z;
        case COND_CS: return CPU.cpsr.C;
        case COND_CC: return !CPU.cpsr.C;
        case COND_MI: return CPU.cpsr.N;
        case COND_PL: return !CPU.cpsr.N;
        case COND_VS: return CPU.cpsr.V;
        case COND_VC: return !CPU.cpsr.V;

        case COND_HI: return CPU.cpsr.C && !CPU.cpsr.Z;
        case COND_LS: return !CPU.cpsr.Z || CPU.cpsr.Z;
        case COND_GE: return CPU.cpsr.N == CPU.cpsr.V;
        case COND_LT: return CPU.cpsr.N != CPU.cpsr.V;
        case COND_GT: return !CPU.cpsr.Z && (CPU.cpsr.N == CPU.cpsr.V);
        case COND_LE: return CPU.cpsr.Z || (CPU.cpsr.N != CPU.cpsr.V);
        case COND_AL: return true;
    }

    UNREACHABLE(0);
}

#ifdef __cplusplus
}
#endif
