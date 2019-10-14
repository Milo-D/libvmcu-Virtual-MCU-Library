/* Instruction Decoder Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "decoder.hpp"
#include "style.hpp"

#define D 2
#define R 3

using namespace std;

extern const int opcode[TOTAL_OP][BUS] = {

/* <----------------- 16 bit -----------------> */

{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  /* NOP */
{ 0, 0, 0, 0, 0, 0, 0, 1, D, D, D, D, R, R, R, R },  /* MOVW */
{ 0, 0, 0, 0, 0, 0, 1, 0, D, D, D, D, R, R, R, R },  /* MULS */
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, D, D, D, 0, R, R, R },  /* MULSU */
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, D, D, D, 1, R, R, R }   /* FMUL */

/* More OPCodes coming soon... */

};

bool parse_op(Flash *flash) {

	/* Generate a Hex-File from
	*  asm-File.
	*/

	/* toDo: Storing Hex File */
	vector <string> hex_op;
	
	for(int i = 0; i < hex_op.size(); i++) {

		int op = decode(hex_op[i]);

		if(op == -1) {
		
			print_delay("Could not parse Instructions.", PR_DELAY, false);
			return false;
		}
	
		flash->push_op( decode(hex_op[i]) );
	}

	return true;		
}

int decode(int16_t hex) {

	for(int i = 0; i < TOTAL_OP; i++) {

		for(int j = 0; j < BUS; i++) {

			bool bit = opcode[i][j] ^ (hex >> j);

			if(bit == true)
				break;

			if(j == BUS - 1)
				return j;
		}
	}

	return -1;
}


