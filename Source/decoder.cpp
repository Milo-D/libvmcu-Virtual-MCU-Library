/* Instruction Decoder Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

// Project Headers
#include "decoder.hpp"
#include "opcode.hpp"
#include "ehandling.hpp"
#include "stringmanip.hpp"

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

    vector <struct plain> decode_line(string hex_line) {

        vector <struct plain> decrypt;
        hex_line = hex_line.substr(1, hex_line.size()); 

        if(hex_line[7] != '0')
            return decrypt;

        int byte_count = hex_to_dec(hex_line.substr(0, 2));

        int s_addr = hex_to_dec(hex_line.substr(2, 4)) / 2;
        int total_instr = byte_count / 2;

        for(int i = 0; i < total_instr; i++) {

            struct plain p;

            int order[4] = { 2, 3, 0, 1 };
            string current = "";

            for(int j = 0; j < 4; j++) {

                int index = (8 + (i * 4) + order[j]);

                if(index >= hex_line.size())
                    print_status("Invalid Byte Count.", true);

                current += hex_line[index];     
            }

            int instr = hex_to_dec(current); int found;

            if((found = get_key_from_op(instr)) < 0)
                print_status("Could not decode " + hex_line, true);

            p.opcode = instr;
            p.key = found;
            p.addr = (s_addr + i);

            decrypt.push_back(p);
        }

        return decrypt;
    }
};

vector <struct plain> decode_file(string hex_file) {

    vector <struct plain> dump;
    ifstream read_file(hex_file, ios::in);

    if(read_file.good() == false)
        print_status("Could not open Hex File.", true);

    string line = "";

    while(getline(read_file, line)) {

        vector <struct plain> p;
        decode_line(line).swap(p);

        dump.insert(dump.end(), p.begin(), p.end());    
    }

    read_file.close();
    return dump;
}

