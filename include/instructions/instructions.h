/* Assembly Instruction Set Header */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define INSTR_MAX 130

typedef struct _system system_t;

extern void (*instructions[INSTR_MAX]) (system_t *sys, const int opcode);

#endif
