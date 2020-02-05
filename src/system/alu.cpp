/* ALU Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "system/alu.hpp"
#include "system/sys.hpp"
#include "system/flash.hpp"
#include "system/gpr.hpp"
#include "system/sreg.hpp"
#include "instructions/instructions.hpp"

using namespace std;

Alu::Alu(Table *table) {

    this->flash = new Flash(table);
    this->gpr = new Gpr();
    this->sreg = new Sreg();
}

Alu::~Alu(void) {

    delete this->flash;
    delete this->gpr;
    delete this->sreg;
}

int Alu::fetch(Sys *sys) {

    int opcode = this->flash->load_opcode();
    int key = this->flash->load_key();

    if(this->flash->table_is_sync() == true) {

        if(key < 0)
            return -1;

        (*instructions[key])(sys, opcode);
        this->flash->pc_next();
    }

    if(this->flash->table_step() < 0)
        sys->kill();

    return 0;
}

int Alu::get_pc(void) {

    return this->flash->pc_get();
}

void Alu::set_pc(int addr) {

    this->flash->pc_set(addr);
}

void Alu::write_gpr(const int rx, const int8_t data) {

    this->gpr->write(rx, data);
}

int8_t Alu::read_gpr(const int rx) {

    return (this->gpr->read(rx));
}

void Alu::gpr_coi(vector <int> & buffer) {

    this->gpr->get_coi(buffer);
}

void Alu::dump_gpr(vector <int8_t> & buffer) {

    this->gpr->dump(buffer);
}

void Alu::write_sreg(const int flag, const bool bit) {

    this->sreg->write(flag, bit);
}

bool Alu::read_sreg(const int flag) {

    return (this->sreg->read(flag));		
}

void Alu::sreg_coi(vector <int> & buffer) {

    this->sreg->get_coi(buffer);
}

uint8_t Alu::dump_sreg(void) {

    return this->sreg->dump();
}



