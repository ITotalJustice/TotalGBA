#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/types.h"
#include "core/util/bit.h"

// [4.4]
struct Format_B {
    int32_t offset;
};

/* gen type functions */
static inline struct Format_B gen_Format_B(const uint32_t opcode) {
    return (struct Format_B){
        .offset = sign_extend(25, opcode << 2)
    };
}
    

#ifdef __cplusplus
}
#endif
