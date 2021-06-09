#include "string.h"

#include <assert.h>


const char* strstr_s(const char* haystack, const size_t haystack_size, const char* needle, const size_t needle_size)
{
    assert(haystack && haystack_size && needle && needle_size);

    for (size_t i = 0; i < haystack_size; i++)
    {
        // if we find the first matching char, start the inner loop
         if (haystack[i] == needle[0])
         {

            for (size_t j = 1; j < needle_size; j++)
            {
                // ensure that we don't go out of bounds of haystack!
                if (i + j >= haystack_size)
                {
                    i += j - 1;
                    goto jmp;
                }

                // if we don't match, exit loop...
                if (haystack[i + j] != needle[j])
                {
                    i += j - 1;
                    goto jmp;
                }
            }

            // if we make it here, we found the string!
            // we return the start of the found string...
            return haystack + i;
        }

        jmp: { /* dummy scope for lable*/ }
    }

    return NULL;
}
