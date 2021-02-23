/* Integration Test Driver for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>

// Test Headers
#include "decoder/decoder_test.h"
#include "system/system_test.h"

int main(void) {

    printf("Running libvmcu integration tests...\n");

    /* calls to submodule tests */

    test_decoder();
    test_system();

    /* calls to submodule tests */

    printf("Done. All tests passing.\n");
    return EXIT_SUCCESS;
}