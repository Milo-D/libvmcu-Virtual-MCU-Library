#ifndef VMCU_READER_H
#define VMCU_READER_H

#include <stdio.h>
#include <inttypes.h>

#include "word.h"

/*
 * vmcu_read - read raw words from a binary file
 * @stream  a file stream to read from
 * @size    number of vmcu_word_t extracted from the file stream. NOT set to zero on failure
 * @retval  a list containing @size words, NULL on failure
 *
 * Configurable options:
 * - vmcu.reader.file_format
 * - vmcu.reader.endianness
 * - vmcu.reader.sort
 * */
vmcu_word_t* vmcu_read(FILE* stream, uint32_t* size);

#endif
