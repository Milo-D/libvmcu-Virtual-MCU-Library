#ifndef VMCU_READER_H
#define VMCU_READER_H

#include <stdio.h>
#include <inttypes.h>

#include "engine.h"
#include "word.h"

/*
 * vmcu_read - read raw words from a binary file
 * @e       engine object
 * @stream  a file stream to read from
 * @size    number of vmcu_word_t extracted from the file stream. NOT set to zero on failure
 * @retval  a list containing @size words, NULL on failure
 *
 * Configurable options:
 * - engine.reader.file_format
 * - engine.reader.endianness
 * - engine.reader.sort
 * */
vmcu_word_t* vmcu_read(vmcu_engine_t* e, FILE* stream, uint32_t* size);

#endif
