#include "arm7tdmi/arm7tdmi.h"
#include "arm/arm.h"
#include "thumb/thumb.h"
#include "internal.h"
#include "util/bit.h"

// #include "arm7tdmi/arm/encoding_types.h"
// #include "arm7tdmi/arm/instruction_table.h"
// #include "arm7tdmi/arm/opcode_hash.h"

// #include "arm7tdmi/thumb/instruction_table.h"
// #include "arm7tdmi/thumb/encoding_types.h"

#include <assert.h>


uint32_t ARM7_psr_to_u32(const struct GBA_Psr* psr)
{
    return
        (psr->N << 31) |
        (psr->Z << 30) |
        (psr->C << 29) |
        (psr->V << 28) |
        (psr->I << 7) |
        (psr->F << 6) |
        (psr->T << 5) |
        (psr->M);
}

void ARM7_u32_to_psr(struct GBA_Psr* psr, uint32_t value)
{
    psr->N = is_bit_set(31, value);
    psr->Z = is_bit_set(30, value);
    psr->C = is_bit_set(29, value);
    psr->V = is_bit_set(28, value);
    psr->I = is_bit_set(7, value);
    psr->F = is_bit_set(6, value);
    psr->T = is_bit_set(5, value);
    psr->M = get_bit_range(0, 4, value);

    GBA_log("mode after ARM7_u32_to_psr(): %u\n", psr->M);
}

enum Arm7tdmiState ARM7_get_state(const struct GBA_Core* gba)
{
    return (enum Arm7tdmiState)CPU.cpsr.T;
}

void ARM7_set_state(struct GBA_Core* gba, const enum Arm7tdmiState state)
{
    CPU.cpsr.T = state;
}

bool ARM7_is_state(const struct GBA_Core* gba, const enum Arm7tdmiState wanted)
{
    return ARM7_get_state(gba) == wanted;
}

#if GBA_DEBUG
    static bool validate_arm7_mode(uint8_t mode)
    {
        static const uint8_t valid_modes[] =
        {
            MODE_USER,
            MODE_FIQ,
            MODE_IRQ,
            MODE_SUPERVISOR,
            MODE_ABORT,
            MODE_UNDEFINED,
            MODE_SYSTEM,
        };

        for (size_t i = 0; i < ARRAY_SIZE(valid_modes); ++i)
        {
            if (valid_modes[i] == mode)
            {
                return true;
            }
        }

        return false;
    }
#endif // GBA_DEBUG

uint8_t ARM7_get_mode(const struct GBA_Core* gba)
{
    #if GBA_DEBUG
        assert(validate_arm7_mode(CPU.cpsr.M) && "invalid mode");
    #endif       

    return CPU.cpsr.M;
}

// NOTE: this does NOT do a proper mode switch!
void ARM7_set_mode(struct GBA_Core* gba, const enum Arm7tdmiMode mode)
{
    #if GBA_DEBUG
        assert(validate_arm7_mode(mode) && "invalid mode");
    #endif    

    CPU.cpsr.M = mode;
}

bool ARM7_get_flag(const struct GBA_Core* gba, const enum Arm7tdmiFlag flag)
{
    switch (flag)
    {
        case FLAG_N: return CPU.cpsr.N;
        case FLAG_Z: return CPU.cpsr.Z;
        case FLAG_C: return CPU.cpsr.C;
        case FLAG_V: return CPU.cpsr.V; 
    }

    UNREACHABLE(0);
}

void ARM7_set_flag(struct GBA_Core* gba, const enum Arm7tdmiFlag flag, const bool value)
{
    switch (flag)
    {
        case FLAG_N: CPU.cpsr.N = value; break;
        case FLAG_Z: CPU.cpsr.Z = value; break;
        case FLAG_C: CPU.cpsr.C = value; break;
        case FLAG_V: CPU.cpsr.V = value; break; 
    }
}

void ARM7_set_flags_nzc(struct GBA_Core* gba, const bool n, const bool z, const bool c)
{
    ARM7_set_flag(gba, FLAG_N, n);
    ARM7_set_flag(gba, FLAG_Z, z);
    ARM7_set_flag(gba, FLAG_C, c);
}

void ARM7_set_flags_nzcv(struct GBA_Core* gba, const bool n, const bool z, const bool c, const bool v)
{
    ARM7_set_flag(gba, FLAG_N, n);
    ARM7_set_flag(gba, FLAG_Z, z);
    ARM7_set_flag(gba, FLAG_C, c);
    ARM7_set_flag(gba, FLAG_V, v);
}

bool ARM7_get_cond(const struct GBA_Core* gba, const enum Arm7tdmiCondition cond)
{
    switch (cond)
    {
        case COND_EQ: return CPU.cpsr.Z;
        case COND_NE: return !CPU.cpsr.Z;
        case COND_CS: return CPU.cpsr.C;
        case COND_CC: return !CPU.cpsr.C;
        case COND_MI: return CPU.cpsr.N;
        case COND_PL: return !CPU.cpsr.N;
        case COND_VS: return CPU.cpsr.V;
        case COND_VC: return !CPU.cpsr.V;

        case COND_HI: return CPU.cpsr.C && !CPU.cpsr.Z;
        case COND_LS: return !CPU.cpsr.C || CPU.cpsr.Z;
        case COND_GE: return CPU.cpsr.N == CPU.cpsr.V;
        case COND_LT: return CPU.cpsr.N != CPU.cpsr.V;
        case COND_GT: return !CPU.cpsr.Z && (CPU.cpsr.N == CPU.cpsr.V);
        case COND_LE: return CPU.cpsr.Z || (CPU.cpsr.N != CPU.cpsr.V);
        case COND_AL: return true;
    }

    UNREACHABLE(false);
}

void ARM7_init(struct GBA_Core* gba)
{
    enum { ROM_START_ADDR = 0x08000000 };

    ARM7_set_mode(gba, MODE_SYSTEM);
    CPU.registers[REG_PC_INDEX] = ROM_START_ADDR;
    // these values are from no$gba
    CPU.registers[REG_LR_INDEX] = 0x08000000;
    CPU.registers[REG_SP_INDEX] = 0x03007F00;
}

void ARM7_run(struct GBA_Core* gba)
{
    // get which state (ARM, THUMB) we are in
    const enum Arm7tdmiState state = ARM7_get_state(gba);

    switch (state)
    {
        case STATE_ARM:
            arm_execute(gba);
            break;
        
        case STATE_THUMB:
            thumb_execute(gba);
            break;
    }
}
