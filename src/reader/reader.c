/* Implements the reader.h API */

#include <stdlib.h>

// Project Headers
#include <lib/reader.h>
#include "format/ihex.h"
#include "format/srec.h"

static int compare_words(const void* a, const void* b) {

    return (((vmcu_word_t*) a)->addr - ((vmcu_word_t*) b)->addr);
}

/* Exposed Functions */

vmcu_word_t* vmcu_read(vmcu_engine_t* e, FILE* stream, uint32_t* size) {

    vmcu_word_t* words;
    *size = 0;

    switch(e->reader.file_format) {

        case VMCU_FMT_IHEX: words = read_ihex(e, stream, size); break;
        case VMCU_FMT_SREC: words = read_srec(e, stream, size); break;
        default:            words = NULL;                       break;
    }

    if (e->reader.sort == true && words != NULL)
        qsort(words, *size, sizeof(vmcu_word_t), compare_words);

    return words;
}

