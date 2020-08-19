/* Opcode Table Header */

#ifndef OPCODE_H
#define OPCODE_H

#define D 2
#define R 3
#define K 4
#define A 5
#define S 6
#define B 7
#define Q 8

#define SET_SIZE 111
#define WORD 16

extern const int opcode[SET_SIZE][WORD];

#endif