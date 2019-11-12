/* Instruction Decoder Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "decoder.hpp"
#include "opcode.hpp"
#include "ehandling.hpp"
#include "stringparse.hpp"
#include "flash.hpp"

using namespace std;

namespace {

    int get_key_from_op(int data) {

        for(int i = 0; i < SET_SIZE; i++) {

            int match = 0;

            for(int j = 0; j < WORD; j++) {

                int binary = ((0x01 << (WORD - j - 1)) & data) >> (WORD - j - 1);

                if(!(opcode[i][j] == 0 || opcode[i][j] == 1)) {

                    match += 1;

                    if(match == WORD)    // returning key
                        return i;
					
                    continue;
				}

                if(binary != opcode[i][j])
                    break;
                else
                    match += 1;

                if(match == WORD)    // returning key
                    return i;
            }
        }

        return -1;  // no valid opcode found, returning -1
    }
};

void decode(Flash *flash, string hex_line) {

    hex_line = hex_line.substr(1, hex_line.size()); 

    if(hex_line[7] != '0')  // validate record type
        return;

    int byte_count = hex_to_dec(hex_line.substr(0, 2));

    if((byte_count & 0x01) == true) // lowest bit set (?)
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

