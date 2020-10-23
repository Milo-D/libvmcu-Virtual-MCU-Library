/* Mnemonic Generator */

#ifndef MNEMONICS_H
#define MNEMONICS_H

#define INSTR_MAX 145

extern char* mnem_dw(const int opcode);

extern char* (*mnemonics[INSTR_MAX]) (const int opcode);

#endif
