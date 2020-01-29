/* Instruction Decoder Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

// Project Headers
#include "disassembler/decoder.hpp"
#include "disassembler/opcode.hpp"
#include "misc/ehandling.hpp"
#include "misc/stringmanip.hpp"

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

    vector <struct plain> decode_hex_line(string hex_line) {

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

            if(instr < 0)
                print_status("Illegal symbols in Hex File.", true);

            if((found = get_key_from_op(instr)) < 0)
                print_status("Could not decode " + hex_line, true);

            p.opcode = instr;
            p.key = found;
            p.addr = (s_addr + i);

            decrypt.push_back(p);
        }

        return decrypt;
    }

    vector <struct data> decode_eep_line(string hex_line) {

        vector <struct data> decrypt;
        hex_line = hex_line.substr(1, hex_line.size());

        if(hex_line[7] != '0')
            return decrypt;

        int byte_count = hex_to_dec(hex_line.substr(0, 2));
        int s_addr = hex_to_dec(hex_line.substr(2, 4)) / 2;

        for(int i = 0; i < byte_count; i++) {

            struct data d;
            string current = "";

            for(int j = 0; j < 2; j++) {

                if((8 + (i * 2) + j) >= hex_line.size()) {

                    decrypt.clear();
                    return decrypt;
                }

                current += hex_line[8 + (i * 2) + j];
            }

            int eep_data = hex_to_dec(current);

            if(eep_data < 0) {

                decrypt.clear();
                return decrypt;
            }

            d.value = eep_data;
            d.addr = (s_addr + i);

            decrypt.push_back(d);
        }

        return decrypt;
    }

    bool validate_hex(string line) {

        if(line.size() < 11)
            return false;

        if(line.find(":") != 0)
            return false;

        return true;
    }
};

vector <struct plain> decode_hex(string hex_file) {

    vector <struct plain> dump;
    ifstream read_file(hex_file, ios::in);

    if(read_file.good() == false)
        print_status("Could not open Hex File.", true);

    string line = "";

    while(getline(read_file, line)) {

        vector <struct plain> p;

        if(validate_hex(line) == false)
            print_status("Wrong Hex Format.", true);

        decode_hex_line(line).swap(p);
        dump.insert(dump.end(), p.begin(), p.end());    
    }

    read_file.close();
    return dump;
}

vector <struct data> decode_eep_hex(string hex_file) {

    vector <struct data> dump;
    ifstream read_file(hex_file, ios::in);

    if(read_file.good() == false) {

        dump.clear();
        return dump;
    }

    string line = "";

    while(getline(read_file, line)) {

        vector <struct data> d;

        if(validate_hex(line) == false) {

            dump.clear();
            return dump;
        }

        decode_eep_line(line).swap(d);
        dump.insert(dump.end(), d.begin(), d.end());
    }

    read_file.close();
    return dump;
}

