#ifndef VMCU_WRITER_IHEX_H
#define VMCU_WRITER_IHEX_H

#include <inttypes.h>

#include <lib/rc.h>
#include <lib/word.h>

vmcu_rc_t write_ihex(FILE* stream, vmcu_word_t* words, const uint32_t n_words);

#endif
