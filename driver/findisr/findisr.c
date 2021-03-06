// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"

/* libvmcu Structures */
vmcu_report_t *report = NULL;

/* Forward Declaration of static Functions */

static void cleanup(void);

/* --- Extern --- */

vmcu_label_t* find_label(int address, vmcu_report_t* report) {

    for(int i=0; i < report->n_label; i++) {

        vmcu_label_t* label = &report->label[i];

        if(label->addr == address)
            return label;
    }
}

void analyze_isr(vmcu_plain_t* plain, vmcu_report_t* report) {

    vmcu_operand_t* op = &plain->src;

    int absolute_address = op->value;

    if(plain->key == VMCU_RJMP)
        absolute_address += plain->addr + 1;

    vmcu_label_t* label = find_label(absolute_address, report);

    printf("L%d\t0x%04x\n", label->id, absolute_address);
}

void print_addresses(vmcu_report_t* report, int start_index, int end_index) {

    for(int i=start_index; i <= end_index; i++) {

        vmcu_plain_t* p = &report->disassembly[i];

        analyze_isr(p, report);
    }
}

int main(const int argc, const char **argv) {

    if(argc < 2) {

        printf("Usage: ./findisr <hexfile>\n");
        return EXIT_FAILURE;
    }

    //optional arg for user to specify
    //the desired table size to search for

    int table_size = -1;
    if(argc == 3) {
        table_size = atoi(argv[1]);
    }

    atexit(cleanup);

    /* Initialize libvmcu */

    const char* filename = argv[argc-1];

    if((report = vmcu_analyze_ihex(filename)) == NULL)
        return EXIT_FAILURE;

    int start_index = -1;
    for(int i=0; i < report->progsize; i++) {

        vmcu_plain_t* p = &report->disassembly[i];

        int key = p->key;

        if(key == VMCU_JMP || key == VMCU_RJMP) {

            if(start_index == -1)
                start_index = i;

            continue;
        }

        const int count = i - start_index;

        if(start_index == -1)
            continue;

        bool case1 = table_size == -1 && count >= 14;
        bool case2 = table_size != -1 && count == table_size;

        int end_index = start_index + count - 1;

        if(case1 || case2) {
            print_addresses(report, start_index, end_index);

        }

        start_index = -1;

    }

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void cleanup(void) {

    if(report != NULL)
        vmcu_report_dtor(report);
}
