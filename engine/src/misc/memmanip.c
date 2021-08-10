/* Memory Manipulation Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Project Headers (engine)
#include "engine/include/misc/memmanip.h"

void vmcu_nfree(const uint32_t n, ...) {

    va_list var;
    va_start(var, n);

    for(uint32_t i = 0; i < n; i++) {

        void *p = va_arg(var, void*);

        if(p != NULL)
            free(p);
    }

    va_end(var);
}