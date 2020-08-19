/* Disassembler Header */

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

/*
* This Disassembler re-creates the Assembly
* Source Code out of the Hexcode found in Hex File.
* Then it automatically generates Labels after
* Jumps and Branches and replaces the relative addresses
* of these instructions.s
*/

typedef struct _array array_t;

extern int disassemble(const char *hex_file, array_t *buffer);

#endif