/* Implementation of Opcode Table */

// C++ Header
#include <stdio.h>
#include <stdlib.h>

// Project Header
#include "disassembler/opcode.h"

const int opcode[SET_SIZE][WORD] = {

/* <----------------- 16 bit -----------------> */

{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* NOP */
{ 0, 0, 0, 0, 0, 0, 0, 1, D, D, D, D, R, R, R, R }, /* MOVW */
{ 0, 0, 0, 0, 0, 0, 1, 0, D, D, D, D, R, R, R, R }, /* MULS */
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, D, D, D, 0, R, R, R }, /* MULSU */
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, D, D, D, 1, R, R, R }, /* FMUL */
{ 1, 1, 1, 0, K, K, K, K, D, D, D, D, K, K, K, K }, /* LDI */
{ 1, 1, 0, 0, K, K, K, K, K, K, K, K, K, K, K, K }, /* RJMP */
{ 1, 0, 0, 1, 0, 1, 0, K, K, K, K, K, 1, 1, 0, K }, /* JMP */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 }, /* IJMP */
{ 0, 0, 1, 0, 1, 1, R, D, D, D, D, D, R, R, R, R }, /* MOV */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 1, 0, 1, 0 }, /* DEC */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 0, 0, 1, 1 }, /* INC */
{ 0, 0, 0, 0, 1, 1, R, D, D, D, D, D, R, R, R, R }, /* ADD */
{ 0, 0, 0, 1, 1, 1, R, D, D, D, D, D, R, R, R, R }, /* ADC */
{ 1, 0, 0, 1, 0, 1, 1, 0, K, K, D, D, K, K, K, K }, /* ADIW */
{ 0, 0, 0, 1, 1, 0, R, D, D, D, D, D, R, R, R, R }, /* SUB */
{ 0, 1, 0, 1, K, K, K, K, D, D, D, D, K, K, K, K }, /* SUBI */
{ 0, 0, 0, 0, 1, 0, R, D, D, D, D, D, R, R, R, R }, /* SBC */
{ 0, 1, 0, 0, K, K, K, K, D, D, D, D, K, K, K, K }, /* SBCI */
{ 1, 0, 0, 1, 0, 1, 1, 1, K, K, D, D, K, K, K, K }, /* SBIW */
{ 1, 0, 0, 1, 0, 0, 1, D, D, D, D, D, 1, 1, 1, 1 }, /* PUSH */
{ 1, 0, 0, 1, 0, 0, 0, D, D, D, D, D, 1, 1, 1, 1 }, /* POP */
{ 1, 0, 1, 1, 0, A, A, D, D, D, D, D, A, A, A, A }, /* IN */
{ 1, 0, 1, 1, 1, A, A, R, R, R, R, R, A, A, A, A }, /* OUT */
{ 1, 0, 0, 1, 1, 0, 1, 1, A, A, A, A, A, S, S, S }, /* SBIS */
{ 1, 1, 1, 1, 1, 1, 0, R, R, R, R, R, 0, B, B, B }, /* SBRC */
{ 0, 0, 1, 0, 0, 1, D, D, D, D, D, D, D, D, D, D }, /* CLR */
{ 1, 0, 0, 1, 0, 0, 0, D, D, D, D, D, 1, 1, 0, 0 }, /* LD(X) */
{ 1, 0, 0, 1, 0, 0, 0, D, D, D, D, D, 1, 1, 0, 1 }, /* LD(X+) */
{ 1, 0, 0, 1, 0, 0, 0, D, D, D, D, D, 1, 1, 1, 0 }, /* LD(-X) */   
{ 1, 0, 0, 0, 0, 0, 0, D, D, D, D, D, 1, 0, 0, 0 }, /* LD(Y) */
{ 1, 0, 0, 1, 0, 0, 0, D, D, D, D, D, 1, 0, 0, 1 }, /* LD(Y+) */
{ 1, 0, 0, 1, 0, 0, 0, D, D, D, D, D, 1, 0, 1, 0 }, /* LD(-Y) */
{ 1, 0, Q, 0, Q, Q, 0, D, D, D, D, D, 1, Q, Q, Q }, /* LDD(Y+q) */
{ 1, 0, Q, 0, Q, Q, 0, D, D, D, D, D, 0, Q, Q, Q }, /* LDD(Z+q) */
{ 1, 0, 0, 0, 0, 0, 0, D, D, D, D, D, 0, 0, 0, 0 }, /* LD(Z) */
{ 1, 0, 0, 1, 0, 0, 0, D, D, D, D, D, 0, 0, 0, 1 }, /* LD(Z+) */
{ 1, 0, 0, 1, 0, 0, 1, R, R, R, R, R, 1, 1, 0, 0 }, /* ST(X) */
{ 1, 0, 0, 1, 0, 0, 1, R, R, R, R, R, 1, 1, 0, 1 }, /* ST(X+) */
{ 1, 0, Q, 0, Q, Q, 1, R, R, R, R, R, 1, Q, Q, Q }, /* STD(Y+q) */
{ 1, 0, Q, 0, Q, Q, 1, R, R, R, R, R, 0, Q, Q, Q }, /* STD(Z+q) */
{ 1, 0, 1, 0, 1, K, K, K, D, D, D, D, K, K, K, K }, /* STS */
{ 1, 0, 0, 1, 0, 0, 1, D, D, D, D, D, 0, 0, 0, 0 }, /* STS(32) */
{ 1, 0, 0, 1, 0, 0, 1, R, R, R, R, R, 0, 1, 0, 0 }, /* XCH */
{ 1, 1, 1, 1, 0, 1, K, K, K, K, K, K, K, 0, 0, 1 }, /* BRNE */
{ 1, 1, 1, 1, 0, 0, K, K, K, K, K, K, K, 0, 0, 1 }, /* BREQ */
{ 1, 1, 1, 1, 0, 1, K, K, K, K, K, K, K, 1, 0, 0 }, /* BRGE */
{ 1, 1, 1, 1, 0, 1, K, K, K, K, K, K, K, 0, 1, 0 }, /* BRPL */
{ 1, 1, 1, 1, 0, 0, K, K, K, K, K, K, K, 0, 0, 0 }, /* BRLO */
{ 1, 1, 1, 1, 0, 0, K, K, K, K, K, K, K, 1, 0, 0 }, /* BRLT */
{ 1, 1, 1, 1, 0, 1, K, K, K, K, K, K, K, 0, 0, 0 }, /* BRCC */
{ 1, 1, 1, 1, 0, 0, K, K, K, K, K, K, K, 0, 0, 0 }, /* BRCS */
{ 1, 1, 1, 1, 0, 0, K, K, K, K, K, K, K, 0, 1, 1 }, /* BRVS */
{ 1, 1, 1, 1, 0, 0, K, K, K, K, K, K, K, 1, 1, 0 }, /* BRTS */
{ 1, 1, 1, 1, 0, 1, K, K, K, K, K, K, K, 1, 1, 0 }, /* BRTC */
{ 1, 1, 1, 1, 0, 0, K, K, K, K, K, K, K, 0, 1, 0 }, /* BRMI */
{ 1, 1, 0, 1, K, K, K, K, K, K, K, K, K, K, K, K }, /* RCALL */
{ 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 }, /* RET */
{ 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1 }, /* ICALL */
{ 0, 0, 0, 1, 0, 1, R, D, D, D, D, D, R, R, R, R }, /* CP */
{ 0, 0, 1, 1, K, K, K, K, D, D, D, D, K, K, K, K }, /* CPI */
{ 0, 0, 0, 0, 0, 1, R, D, D, D, D, D, R, R, R, R }, /* CPC */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 0, 1, 1, 0 }, /* LSR */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 0, 1, 0, 1 }, /* ASR */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 0, 1, 1, 1 }, /* ROR */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 0, 0, 1, 0 }, /* SWAP */
{ 0, 1, 1, 0, K, K, K, K, D, D, D, D, K, K, K, K }, /* ORI */
{ 0, 0, 1, 0, 1, 0, R, D, D, D, D, D, R, R, R, R }, /* OR */
{ 0, 0, 1, 0, 0, 0, R, D, D, D, D, D, R, R, R, R }, /* AND */
{ 0, 1, 1, 1, K, K, K, K, D, D, D, D, K, K, K, K }, /* ANDI */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 0, 0, 0, 0 }, /* COM */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 0, 0, 0, 1 }, /* NEG */
{ 1, 1, 1, 1, 1, 0, 0, D, D, D, D, D, 0, B, B, B }, /* BLD */
{ 1, 1, 1, 1, 1, 0, 1, D, D, D, D, D, 0, B, B, B }, /* BST */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 }, /* SES */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0 }, /* SET */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0 }, /* SEV */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 }, /* SEZ */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0 }, /* SEH */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, /* SEC */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0 }, /* SEI */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 }, /* SEN */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0 }, /* CLS */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0 }, /* CLT */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0 }, /* CLV */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0 }, /* CLZ */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0 }, /* CLH */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 }, /* CLC */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0 }, /* CLI */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0 }, /* CLN */
{ 1, 0, 0, 1, 0, 1, 0, 0, 1, S, S, S, 1, 0, 0, 0 }, /* BCLR */
{ 1, 0, 0, 1, 0, 1, 0, 0, 0, S, S, S, 1, 0, 0, 0 }, /* BSET */

/* More OPCodes coming soon... */

};