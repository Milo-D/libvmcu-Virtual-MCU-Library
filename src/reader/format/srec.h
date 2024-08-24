/* Motorola SREC Reader */

#ifndef VMCU_SREC_H
#define VMCU_SREC_H

#include <stdio.h>
#include <inttypes.h>

typedef struct vmcu_word vmcu_word_t;
typedef struct vmcu_engine vmcu_engine_t;

/*
 * read_srec - read words from a S-record stream
 * @e       engine object
 * @stream  opened file stream to an intel hex file
 * @size    number of vmcu_word_t extracted from the file stream
 * */
vmcu_word_t* read_srec(vmcu_engine_t* e, FILE* stream, uint32_t* size);

#endif
