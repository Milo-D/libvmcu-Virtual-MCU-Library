/* Instruction Decoder Header */

#ifndef VMCU_DECODER_H
#define VMCU_DECODER_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

/*
 * This is a small decoder for the Intel Hex
 * format.
 *
 * Note: The decoder does not care about
 * the checksum.
 *
 * */

typedef struct vmcu_plain vmcu_plain_t;

extern int vmcu_decode_bytes(const uint32_t bytes, vmcu_plain_t *p);
extern vmcu_plain_t* vmcu_decode_ihex(const char *hex_file, int32_t *size);

#endif