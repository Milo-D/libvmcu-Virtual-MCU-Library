/* Disassembly Stepper for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

#define MAX 20

/* libvmcu Structures */

vmcu_report_t *report = NULL;
vmcu_system_t *sys    = NULL;

/* Forward Declaration of static Functions */

static void print_disassembly(const int32_t pc);
static int find_pc(const int32_t pc);
static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./skeleton <hexfile>\n");
        return EXIT_FAILURE;
    }

    atexit(cleanup);

    /* Initialize libvmcu */
    if((report = vmcu_analyze_ihex(argv[1])) == NULL)
        return EXIT_FAILURE;

    sys = vmcu_system_ctor(report);

    char c;

    do {

        int32_t pc = vmcu_system_get_pc(sys);
        print_disassembly(pc);

        c = getchar();

        if(c == '\n')
            vmcu_system_step(sys);

    } while(c != 'q');

    /* Do something... */
    return EXIT_SUCCESS;
}

/* --- Static --- */

static void print_disassembly(const int32_t pc) {

    int index;

    if((index = find_pc(pc)) < 0)
        return;

    system("clear");

    for(int i = (index - MAX); i <= (index + MAX) ; i++) {

        if(i < 0 || i >= report->progsize) {

            printf("\n");
            continue;
        }

        vmcu_plain_t *p = &report->disassembly[i];
        printf("0x%04x", p->addr);

        if(p->addr == pc)
            printf(" [->] ");
        else
            printf("      ");

        printf("%s\n", p->mnem);
    }
}

static int find_pc(const int32_t pc) {

    for(int i = 0; i < report->progsize; i++) {

        if(report->disassembly[i].addr == pc)
            return i;
    }

    return -1;
}

static void cleanup(void) {

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);
}