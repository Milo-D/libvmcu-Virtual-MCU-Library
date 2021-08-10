#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

struct pair {

    VMCU_IKEY ikey;

    int hits;
    char mnem[10];
};

int compare(const void* a, const void* b) {

    struct pair* p1 = (struct pair*)a;
    struct pair* p2 = (struct pair*)b;

    return p1->hits - p2->hits;
}

int spaceIndex(char* mnem) {

    char* end = strchr(mnem, ' ');
    return end - mnem;
}

void init_list(struct pair** list, int nKeys) {

    for(int i=0; i < nKeys; i++) {

        struct pair* mypair = malloc(sizeof(struct pair));

        mypair->ikey = i;
        mypair->hits = 0;
        memset(mypair->mnem, 0, 10*sizeof(char));

        list[i] = mypair;
    }
}

void bubblesort(void **array, int length, int (*compar)(const void *, const void *)) {

    int i, j;
    void* tmp;

    for (i = 1; i < length; i++)
    {
        for (j = 0; j < length - 1 ; j++)
        {
            if (compar(array[j], array[j+1]) < 0)
            {
                tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
}


int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("Expected hex file argument. exiting.\n");
        return 1;
    }

    //instructions to filter for
    int countFilters = argc-2;
    const char* fname = argv[argc-1];

    vmcu_model_t* m328p   = vmcu_model_ctor(VMCU_DEVICE_M328P);
    vmcu_report_t* report = vmcu_analyze_ihex(fname, m328p);

    if(report == NULL) {

        vmcu_model_dtor(m328p);
        return EXIT_FAILURE;
    }

    vmcu_system_t* sys = vmcu_system_ctor(report);

    //highest possible index is 133
    const int nKeys = 134;

    struct pair** list = malloc(sizeof(struct pair*) * nKeys);

    init_list(list, nKeys);

    for(uint32_t i=0; i < report->progsize; i++) {

        vmcu_instr_t* instr = &report->disassembly[i];

        //because VMCU_IKEY_DATA is -1
        if(instr->key == VMCU_IKEY_DATA) {
            continue;
        }

        struct pair* mp = list[instr->key];
        strcpy(mp->mnem, instr->mnem.base);

        mp->hits += 1;
    }

    //sort the instructions
    //qsort(list, nKeys, sizeof(struct pair*), compare);
    bubblesort((void**)list, nKeys, compare);

    for(int i=0; i < nKeys; i++) {

        struct pair* hit = list[i];

        if(hit->hits == 0) {
            free(hit);
            continue;
        }

        bool print = true;
        if(countFilters > 0) {
            print = false;
            for(int i=0; i < countFilters; i++) {
                char* filter = argv[1+i];
                //printf("filter: '%s'\n", filter);
                if(strcmp(filter, hit->mnem) == 0) {
                    print = true;
                }
            }

        }

        if(print) {
            printf("%8s %5d\n", hit->mnem, hit->hits);
        }

        free(hit);
    }

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);
    vmcu_model_dtor(m328p);

    free(list);

    return 0;
}
