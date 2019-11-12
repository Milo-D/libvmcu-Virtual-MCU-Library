/* Instruction Decoder Header */

#ifndef DECODER_HPP
#define DECODER_HPP

// C Headers
#include <inttypes.h>

class Flash;

/*
* Atmel AVR Opcodes to parse Assembly
* Instructions. Leaving variable bits
* blank (marked as an integer greater
* than one).
*/

void decode(Flash *flash, std::string hex_line);

#endif
