/* Instruction Decoder Header */

#ifndef DECODER_HPP
#define DECODER_HPP

// C Headers
#include <inttypes.h>

// C++ Headers
#include <vector>

/*
* Atmel AVR Opcodes to parse Assembly
* Instructions. Leaving variable bits
* blank (marked as an integer greater
* than one).
*/

struct plain {

    int opcode;
    int key;
    int addr;
};

std::vector <struct plain> decode_file(std::string hex_file);

#endif
