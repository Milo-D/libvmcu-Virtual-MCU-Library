#ifndef VMCU_WRITER_H
#define VMCU_WRITER_H

#include <stdio.h>
#include <inttypes.h>

#include "word.h"

/*
 * vmcu_write - write raw words to a binary file
 * @stream  a file stream to write the binary to
 * @words   the words to write
 * @n_words the total number of words
 * @vmcu.rc vmcu.rc set accordingly
 *
 * Configurable options:
 * - vmcu.writer.file_format
 * - vmcu.writer.endianness
 * - vmcu.writer.byte_count
 * - vmcu.writer.words_mutable
 * */
void vmcu_write(FILE* stream, vmcu_word_t* words, const uint32_t n_words);

#endif
