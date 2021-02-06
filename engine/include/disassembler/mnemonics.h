/* Mnemonic Generator */

#ifndef MNEMONICS_H
#define MNEMONICS_H

#define INSTR_MAX 145

typedef struct _plain plain_t;

extern char* mnem_dw(plain_t *p);
extern char* (*mnemonics[INSTR_MAX]) (plain_t *p);

#endif
