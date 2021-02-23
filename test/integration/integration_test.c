/* Integration Test Driver for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>

// Test Headers
#include "decoder/decoder_test.h"

int main(void) {

    printf("Running libvmcu integration tests...\n");

    /* calls to unit-tests */

    test_decoder();

    /* calls to unit-tests */

    printf("Done. All tests passing.\n");
    return EXIT_SUCCESS;
}