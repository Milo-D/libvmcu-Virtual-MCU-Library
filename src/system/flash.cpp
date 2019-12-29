/* AVR Flash Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <tuple>

// Project Headers
#include "system/flash.hpp"
#include "system/mcu.hpp"
#include "misc/ehandling.hpp"
#include "misc/stringmanip.hpp"
#include "disassembler/decoder.hpp"
#include "table/table.hpp"

using namespace std;

Flash::Flash(Table *table) {

    vector <struct plain> plain;
	decode_file( table->src() ).swap(plain);

    this->memory = (int16_t*) malloc(FLASH_SIZE * sizeof(int16_t));
    memset(this->memory, 0x0000, FLASH_SIZE * sizeof(int16_t));

    this->mem_usage = 0;

    for(int i = 0; i < plain.size(); i++) {

        this->memory[ plain[i].addr ] = plain[i].opcode;
        this->mem_usage += 1;

        tuple <int, int> key = make_tuple(plain[i].addr, plain[i].key);
        this->keys.push_back(key);
    }

    this->pc = 0;
    this->table = table;
}

Flash::~Flash(void) {

    free(this->memory);
}

int Flash::load_opcode(void) {

    return this->memory[this->pc];
}

int Flash::load_key(void) {

    for(int i = 0; i < this->keys.size(); i++) {

        if(get <0> (this->keys[i]) == this->pc)
            return get <1> (this->keys[i]);
    }

    return -1;
}

void Flash::pc_next(void) {

    if(this->pc == FLASH_SIZE - 1)
        return;

    this->pc += 1;
}

int Flash::pc_get(void) {

    return this->pc;
}

void Flash::pc_set(int addr) {

    if(addr >= FLASH_SIZE)
        return;

    if(addr < 0)
        return;

    this->pc = addr - 1;
    this->table->jump(addr);
}

int Flash::table_step(void) {

    return this->table->step();
}

bool Flash::table_is_sync(void) {

    return this->table->is_sync(this->pc);
}

string Flash::get_table(void) {

    return this->table->center_to_str();
}

