/* Intel Hex Reader */

#ifndef VMCU_IHEX_H
#define VMCU_IHEX_H

#include <stdio.h>
#include <inttypes.h>

typedef struct vmcu_word vmcu_word_t;
typedef struct vmcu_engine vmcu_engine_t;

/*
 * read_ihex - read words from an intel hex stream
 * @e       engine object
 * @stream  opened file stream to an intel hex file
 * @size    number of vmcu_word_t extracted from the file stream
 * */
vmcu_word_t* read_ihex(vmcu_engine_t* e, FILE* stream, uint32_t* size);

#endif
