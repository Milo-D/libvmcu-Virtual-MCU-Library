/* ALU Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "alu.hpp"
#include "sys.hpp"
#include "flash.hpp"
#include "gpr.hpp"
#include "sreg.hpp"
#include "instructions.hpp"

using namespace std;

Alu::Alu(Table *table) {

    this->flash = new Flash(table);
    this->gpr = new Gpr();
    this->sreg = new Sreg();
}

Alu::~Alu() {

    delete flash;
    delete gpr;
}

int Alu::fetch(Sys *sys) {

    int opcode = this->flash->load_instr();
    int key = this->flash->load_key();

    if(this->flash->table_is_exec() == false)
        goto skip; 

    if(opcode < 0)
        return -1;

    (*instructions[key])(sys, opcode);
    
    this->flash->pc_next();

skip:

    if(this->flash->table_step() < 0)
        sys->kill();

    return 0;
}

void Alu::write_gpr(int rx, int8_t data) {

    this->gpr->write(rx, data);
}

int8_t Alu::read_gpr(int rx) {

    return (this->gpr->read(rx));
}

string Alu::get_gpr(int cursor) {

    return this->gpr->to_str(cursor);
}

void Alu::write_sreg(int flag, bool bit) {

    this->sreg->write(flag, bit);
}

bool Alu::read_sreg(int flag) {

    return (this->sreg->read(flag));		
}

string Alu::get_sreg(void) {

    return this->sreg->to_str();
}

void Alu::table_set_tip(int instr_line) {

    this->flash->table_set_tip(instr_line);
}

bool Alu::table_has_break(void) {

    return this->flash->table_has_break();
}

bool Alu::table_is_break(void) {

    return this->flash->table_is_break();
}

int Alu::table_size(void) {

    return this->flash->table_size();
}

string Alu::table_to_str(void) {

    return this->flash->table_to_str();
}





