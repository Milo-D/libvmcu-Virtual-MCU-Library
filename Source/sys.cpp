/* (Sys)tem Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "sys.hpp"
#include "alu.hpp"
#include "gpr.hpp"
#include "sram.hpp"
#include "flash.hpp"
#include "eeprom.hpp"

using namespace std;

Sys::Sys(string asm_file) {

    this->alu = new Alu(asm_file);
    this->sram = new Sram();
    this->eeprom = new Eeprom();

    this->terminated = false;
}

void Sys::step(void) {

    if(this->terminated == true)
        return;

    if(this->alu->fetch(this) < 0)
        this->kill();
}

void Sys::kill(void) {

    this->terminated = true;
}

bool Sys::is_terminated(void) {

    return this->terminated;	
}

void Sys::write_gpr(int rx, int8_t data) {

    this->alu->write_gpr(rx, data);
}

int8_t Sys::read_gpr(int rx) {

    return this->alu->read_gpr(rx);
}

string Sys::gpr_to_str(int cursor) {

    return this->alu->get_gpr(cursor);
}

void Sys::write_sreg(int flag, bool bit) {

    this->alu->write_sreg(flag, bit);
}

bool Sys::read_sreg(int flag) {

    return this->alu->read_sreg(flag);
}

string Sys::sreg_to_str(void) {

    return this->alu->get_sreg();
}

