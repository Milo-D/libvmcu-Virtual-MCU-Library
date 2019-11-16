/* Implementation of Opcode Table */

// C++ Header
#include <iostream>
#include <iomanip>
#include <string>

// Project Header
#include "opcode.hpp"

extern const int opcode[SET_SIZE][WORD] = {

/* <----------------- 16 bit -----------------> */

{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* NOP */
{ 0, 0, 0, 0, 0, 0, 0, 1, D, D, D, D, R, R, R, R }, /* MOVW */
{ 0, 0, 0, 0, 0, 0, 1, 0, D, D, D, D, R, R, R, R }, /* MULS */
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, D, D, D, 0, R, R, R }, /* MULSU */
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, D, D, D, 1, R, R, R }, /* FMUL */
{ 1, 1, 1, 0, K, K, K, K, D, D, D, D, K, K, K, K }, /* LDI */
{ 1, 1, 0, 0, K, K, K, K, K, K, K, K, K, K, K, K }, /* RJMP */
{ 0, 0, 1, 0, 1, 1, R, D, D, D, D, D, R, R, R, R }, /* MOV */
{ 1, 0, 0, 1, 0, 1, 0, D, D, D, D, D, 1, 0, 1, 0 }, /* DEC */
{ 1, 0, 0, 1, 0, 0, 1, D, D, D, D, D, 1, 1, 1, 1 }, /* PUSH */
{ 1, 0, 0, 1, 0, 0, 0, D, D, D, D, D, 1, 1, 1, 1 }, /* POP */
{ 1, 0, 1, 1, 1, A, A, R, R, R, R, R, A, A, A, A }, /* OUT */
{ 0, 0, 1, 0, 0, 1, D, D, D, D, D, D, D, D, D, D }  /* CLR */

/* More OPCodes coming soon... */

};
