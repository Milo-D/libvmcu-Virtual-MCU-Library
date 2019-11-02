/* Main Assembly Instructions Header */

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#define INSTR_MAX 111

class Sys;

extern void (*instructions[INSTR_MAX]) (Sys *sys, int opcode);

#endif
