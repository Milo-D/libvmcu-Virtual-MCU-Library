/* Instruction Decoder Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "decoder.hpp"
#include "ehandling.hpp"
#include "uparse.hpp"
#include "flash.hpp"

#define D 2
#define R 3
#define K 4

using namespace std;

extern const int opcode[TOTAL_OP][BUS] = {

/* <----------------- 16 bit -----------------> */

{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },	/* NOP */
{ 0, 0, 0, 0, 0, 0, 0, 1, D, D, D, D, R, R, R, R },	/* MOVW */
{ 0, 0, 0, 0, 0, 0, 1, 0, D, D, D, D, R, R, R, R }, /* MULS */
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, D, D, D, 0, R, R, R }, /* MULSU */
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, D, D, D, 1, R, R, R },	/* FMUL */
{ 1, 1, 1, 0, K, K, K, K, D, D, D, D, K, K, K, K },	/* LDI */
{ 1, 1, 0, 0, K, K, K, K, K, K, K, K, K, K, K, K }	/* RJMP */

/* More OPCodes coming soon... */

};

namespace {

	int get_key_from_op(int data) {

		for(int i = 0; i < TOTAL_OP; i++) {

			int match = 0;

			for(int j = 0; j < BUS; j++) {

				int binary = ((0x01 << (BUS - j - 1)) & data) >> (BUS - j - 1);

				if(!(opcode[i][j] == 0 || opcode[i][j] == 1)) {

					match += 1;

					if(match == BUS)	// returning key
						return i;
					
					continue;
				}

				if(binary != opcode[i][j])
					break;
				else
					match += 1;

				if(match == BUS)	// returning key
					return i;
			}
		}

		return -1;	// no valid opcode found, returning -1
	}
};

void decode(Flash *flash, string hex_line) {

	hex_line = hex_line.substr(1, hex_line.size()); 

	if(hex_line[7] != '0')	// validate record type
		return;

	int byte_count = hex_to_dec(hex_line.substr(0, 2));

	if((byte_count & 0x01) == true)	 // lowest bit set (?)
		return;

	int total_instr = byte_count / 2;

	for(int i = 0; i < total_instr; i++) {

		/* Byte Order: 0C EF -> EF 0C -> 2,3 0,1 */

		int order[4] = { 2, 3, 0, 1 };
		string current = "";

		for(int j = 0; j < 4; j++)
			current += hex_line[8 + (i*4) + order[j]];		

		int instr = hex_to_dec(current); int key;

		if((key = get_key_from_op(instr)) < 0)
			print_status("Could not decode " + hex_line, true);

		flash->insert_instr(instr);
		flash->insert_key(key);
	}
}

