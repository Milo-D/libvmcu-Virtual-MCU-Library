/* A sandbox for quick driver tests */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

/* libvmcu Structures */

vmcu_report_t *report = NULL;
vmcu_system_t *sys    = NULL;

/* Forward Declaration of static Functions */

static void print_plain_struct(vmcu_plain_t *p, const uint32_t opcode);
static int htoi(const char *input);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./sandbox <opcode>\n");
        return EXIT_FAILURE;
    }

    const uint32_t opc = htoi(argv[1]);

    vmcu_plain_t p;

    if(vmcu_disassemble_bytes(opc, &p) < 0) {

        printf("Could not decode.\n");
        return EXIT_FAILURE;
    }

    print_plain_struct(&p, opc);
    free(p.mnem);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void print_plain_struct(vmcu_plain_t *p, const uint32_t opcode) {

    printf("---- Plain representation of 0x%04x ----\n", opcode);

    printf("opcode:     0x%04x\n", p->opcode);
    printf("address:    0x%04x\n", p->addr);
    printf("mnemonic:   %s\n", p->mnem);
    printf("executable: ");

    if(p->exec == true)
        printf("yes\n");
    else
        printf("no\n");

    printf("size:       ");

    if(p->dword == true)
        printf("32-bit\n");
    else
        printf("16-bit\n");
}

int htoi(const char *input) {

    unsigned int len = strlen(input);
    int dec = 0; int base = 1; int end = 0;

    if(len > 1) {

        char sub[3] = { input[0], input[1], '\0' };

        if(strcmp(sub, "0x") == 0)
            end += 2;
    }

    for(int i = len - 1; i >= end; i--) {

        if(input[i] >= '0' && input[i] <= '9') {

            dec += (input[i] - '0') * base;
            base *= 16;

        } else if(input[i] >= 'A' && input[i] <= 'F') {

            dec += (10 + input[i] - 'A') * base;
            base *= 16;

        } else if(input[i] >= 'a' && input[i] <= 'f') {

            dec += (10 + input[i] - 'a') * base;
            base *= 16;

        } else {

            return -1;
        }
    }

    return dec;
}