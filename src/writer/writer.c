#include <stdlib.h>
#include <string.h>

#include <lib/writer.h>
#include <lib/engine.h>
#include "format/ihex.h"

static int compare_words(const void* a, const void* b) {

    return (((vmcu_word_t*) a)->addr - ((vmcu_word_t*) b)->addr);
}

void vmcu_write(FILE* stream, vmcu_word_t* words, const uint32_t n_words) {

    vmcu_word_t* copy;

    if (vmcu.writer.words_mutable == false) {

        copy = vmcu.cb.alloc(n_words * sizeof(vmcu_word_t));
        memcpy(copy, words, n_words * sizeof(vmcu_word_t));

        words = copy;
    }

    qsort(words, n_words, sizeof(vmcu_word_t), compare_words);

    switch (vmcu.writer.file_format) {

        case VMCU_FMT_IHEX: vmcu.rc = write_ihex(stream, words, n_words); break;
        default:            vmcu.rc = VMCU_RC_UNSUPPORTED;                break;
    }

    if (vmcu.writer.words_mutable == false)
        vmcu.cb.free(copy);
}
