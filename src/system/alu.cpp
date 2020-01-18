/* ALU Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "cli/debugwindow.hpp"
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

void Alu::write_gpr(int rx, int8_t data) {

    this->gpr->write(rx, data);
}

int8_t Alu::read_gpr(int rx) {

    return (this->gpr->read(rx));
}

void Alu::scale_gpr(int offs) {

    this->gpr->scale(offs);
}

void Alu::put_gpr(DebugWindow *dwin) {

    this->gpr->to_win(dwin);
}

void Alu::write_sreg(int flag, bool bit) {

    this->sreg->write(flag, bit);
}

bool Alu::read_sreg(int flag) {

    return (this->sreg->read(flag));		
}

void Alu::put_sreg(DebugWindow *dwin) {

    this->sreg->to_win(dwin);
}

void Alu::put_table(DebugWindow *dwin, bool full) {

    this->flash->put_table(dwin, full);
}





