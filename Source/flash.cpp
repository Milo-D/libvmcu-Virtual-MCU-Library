/* AVR Flash Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

// Project Headers
#include "flash.hpp"
#include "ehandling.hpp"
#include "stringparse.hpp"
#include "mcu.hpp"
#include "decoder.hpp"
#include "table.hpp"

using namespace std;

namespace {

    void create(string asm_file) {

        string cmd = "avra -I /usr/share/avra/ " + asm_file;
        system(cmd.c_str());
    }

    void destroy(string prefix) {

        string type[4] = { ".cof", ".hex", ".eep.hex", ".obj" };

        for(int i = 0; i < 4; i++) {

            string cmd = "rm " + prefix + type[i];
            system(cmd.c_str());
        }
    }
};

Flash::Flash(Table *table) {

    string asm_file = table->src();
    create(asm_file);

    string hex_file = get_file_name(asm_file) + ".hex";
    ifstream read_file(hex_file, ios::in);

    if(read_file.good() == false)
        print_status("Could not create Hex File.", true);

    string line = "";

    while(getline(read_file, line))
        decode(this, line);

    destroy(get_file_name(asm_file));
		
    read_file.close();
					
    this->pc = 0;
    this->size = FLASH_SIZE;
    this->size_used = this->app.size();
    this->table = table;
}

void Flash::insert_instr(int instr) {

    this->app.push_back(instr);
}

void Flash::insert_key(int key) {

    this->key.push_back(key);
}

int Flash::load_instr(void) {

    return this->app[this->pc];
}

int Flash::load_key(void) {

    return this->key[this->pc];
}

void Flash::pc_next(void) {

    if(this->pc == this->size_used - 1)
        return;

    this->pc += 1;
}

int Flash::pc_get(void) {

    return this->pc;
}

void Flash::pc_set(int addr) {

    if(addr > this->size_used)
        return;

    this->pc = addr - 1;
    this->table->jump(addr);
}

int Flash::table_step(void) {

    return this->table->step();
}

void Flash::table_set_tip(int instr_line) {

    this->table->set_tip(instr_line);
}

bool Flash::table_has_break(void) {

    return this->table->has_break();
}

bool Flash::table_is_break(void) {

    return this->table->is_break();
}

bool Flash::table_is_exec(void) {

    return this->table->executable();
}

int Flash::table_size(void) {

    return this->table->size();
}

string Flash::get_table(void) {

    return this->table->center_to_str();
}

