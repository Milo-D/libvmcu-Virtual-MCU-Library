/* A basic driver to find and print strings */

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

vmcu_model_t  *m328p  = NULL;
vmcu_report_t *report = NULL;

/* Forward Declaration of static Functions */

static void print_string(const vmcu_string_t *str);
static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./strings <hexfile>\n");
        return EXIT_FAILURE;
    }

    atexit(cleanup);

    m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;

    for(int32_t i = 0; i < report->n_string; i++) {

        vmcu_string_t *str = &report->string[i];

        printf("Found string \"");
        print_string(str);
        printf("\" ");

        printf("l = %d", str->length);
        printf(" @ 0x%04x\n", str->addr);
    }

    printf("\nTotal strings found: %d\n", report->n_string);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void print_string(const vmcu_string_t *str) {

    /* we need a string printer in order to avoid
     * printing ascii control codes.
     *
     * This function only handles '\n'.
     * */

    for(int32_t i = 0; i < str->length; i++) {

        const char ch = str->bytes[i];

        if(ch == '\n') {

            printf("\\n");
            continue;
        }

        if(0x00 <= ch && ch <= 0x1f)
            continue;

        printf("%c", ch);
    }
}

static void cleanup(void) {

    if(report != NULL)
        vmcu_report_dtor(report);

    if(m328p != NULL)
        vmcu_model_dtor(m328p);
}
