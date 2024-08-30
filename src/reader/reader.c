#include <stdlib.h>

#include <lib/reader.h>
#include <lib/engine.h>
#include "format/ihex.h"
#include "format/srec.h"

static int compare_words(const void* a, const void* b) {

    return (((vmcu_word_t*) a)->addr - ((vmcu_word_t*) b)->addr);
}

/* Exposed Functions */

vmcu_word_t* vmcu_read(FILE* stream, uint32_t* size) {

    vmcu_word_t* words;
    *size = 0;

    switch(vmcu.reader.file_format) {

        case VMCU_FMT_IHEX: words = read_ihex(stream, size); break;
        case VMCU_FMT_SREC: words = read_srec(stream, size); break;
        default:            words = NULL;                    break;
    }

    if (vmcu.reader.sort == true && words != NULL)
        qsort(words, *size, sizeof(vmcu_word_t), compare_words);

    return words;
}

