/* Motorola SREC Reader */

#ifndef VMCU_SREC_H
#define VMCU_SREC_H

#include <stdio.h>
#include <inttypes.h>

#include <lib/word.h>

/*
 * read_srec - read words from a S-record stream
 * @stream  opened file stream to an intel hex file
 * @size    number of vmcu_word_t extracted from the file stream
 * */
vmcu_word_t* read_srec(FILE* stream, uint32_t* size);

#endif
