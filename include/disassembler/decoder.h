/* Instruction Decoder Header */

#ifndef DECODER_H
#define DECODER_H

/*
* Atmel AVR Opcodes to parse Assembly
* Instructions. Leaving variable bits
* blank (marked as an integer greater
* than one).
*/

// C Headers
#include <stdbool.h>

typedef struct _array array_t;
typedef struct _plain plain_t;

typedef struct _eep {

    int value;
    int addr;

} eep_t;

extern int decode_hex(const char *hex_file, array_t *buffer);
extern int decode_eep(const char *hex_file, array_t *buffer);

#endif