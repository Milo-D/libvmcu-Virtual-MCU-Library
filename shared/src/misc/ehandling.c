/* Implementation of Error Handling Header */

// C Headers
#include <stdio.h>
#include <stdlib.h>

// Project Headers (shared)
#include "shared/include/misc/ehandling.h"

void print_status(const char *msg, const bool fatal) {

    printf("MDX: %s\n", msg);

    if(fatal == true)
        exit(EXIT_FAILURE);
}


