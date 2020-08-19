/* Mnemonic Generator */

#ifndef MNEMONICS_H
#define MNEMONICS_H

#define INSTR_MAX 111

extern char* (*mnemonics[INSTR_MAX]) (const int opcode);

#endif