#include "bit.h"

#include <assert.h>


#define STATIC_ASSERT(COND,MSG) typedef int static_assertion_##MSG[(COND)?1:-1]

STATIC_ASSERT(1,this_should_be_true);


// NOTE: value might need to be cast to u64 first on 32bit!
uint32_t bit_rotr(const uint32_t shift, const uint32_t value)
{
    assert(shift < 31 && "invalid bit rotr");

    return (value >> shift) | ((uint64_t)value << (32 - shift));
}

bool is_bit_set(const uint8_t bit, const uint32_t value)
{
    assert(bit < (sizeof(uint32_t) * 8) && "bit value out of bounds!");
    
    return !!(value & (1U << bit));
}

uint32_t get_bit_range(const uint8_t start, const uint8_t end, const uint32_t value)
{
    assert(end > start && end < 31 && "invalid bit range!");

    return (value & (0xFFFFFFFF >> (31 - end))) >> start;
}

int32_t sign_extend(const uint8_t start_size, const uint32_t value)
{
    assert(start_size < 31 && "bit start size is out of bounds!");

    const uint8_t bits = 31 - start_size;
    
    return ((int32_t)(value << bits)) >> bits;
}
