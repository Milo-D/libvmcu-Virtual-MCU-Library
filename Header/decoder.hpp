/* Instruction Decoder Header */

#ifndef DECODER_HPP
#define DECODER_HPP

#define TOTAL_OP 111
#define BUS 16

// C Headers
#include <inttypes.h>

class Flash;

/*
* Atmel AVR Opcodes to parse Assembly
* Instructions. Leaving variable bits
* blank (marked as an Integer greater
* than one).
*/

extern const int opcode[TOTAL_OP][BUS];

void decode(Flash *flash, std::string hex_line);

#endif
