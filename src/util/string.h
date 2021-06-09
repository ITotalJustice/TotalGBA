#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


// simple impl of strstr but with size param passed in
// this does NOT check for NULL term, the size must be valid!
GBA_STATIC const char* strstr_s(const char* haystack, const size_t haystack_size, const char* needle, const size_t needle_size);

#ifdef __cplusplus
}
#endif
