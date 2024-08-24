/* Misc utility functions for libvmcu */

#include <stdlib.h>
#include <string.h>

// Project Headers
#include "utils/utils.h"

uint64_t htou64(const char* input, int* error) {

    uint64_t     base = 1;
    int          end  = 0;
    uint64_t     res  = 0;
    unsigned int len  = strlen(input);

    *error = -1;

    if (len == 0)
        return 0;

    if (len > 1) {

        if (strncmp(input, "0x", 2) == 0)
            end += 2;

        if (len <= end)
            return 0;
    }

    for (int i = len - 1; i >= end; i--) {

        if (input[i] >= '0' && input[i] <= '9') {

            res  += (input[i] - '0') * base;
            base *= 16;
                        
        } else if (input[i] >= 'A' && input[i] <= 'F') {

            res  += (10 + input[i] - 'A') * base;
            base *= 16;
            
        } else if (input[i] >= 'a' && input[i] <= 'f') {

            res  += (10 + input[i] - 'a') * base;
            base *= 16;

        } else {

            return 0;
        }
    }

    *error = 0;
    return res;
}
