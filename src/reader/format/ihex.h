/* Intel Hex Reader */

#ifndef VMCU_READER_IHEX_H
#define VMCU_READER_IHEX_H

#include <stdio.h>
#include <inttypes.h>

#include <lib/word.h>

/*
 * read_ihex - read words from an intel hex stream
 * @stream  opened file stream to an intel hex file
 * @size    number of vmcu_word_t extracted from the file stream
 * */
vmcu_word_t* read_ihex(FILE* stream, uint32_t* size);

#endif
