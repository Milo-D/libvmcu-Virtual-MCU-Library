/* (Sys)tem Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "sys.hpp"
#include "alu.hpp"
#include "gpr.hpp"
#include "data.hpp"
#include "flash.hpp"
#include "eeprom.hpp"

using namespace std;

Sys::Sys(string asm_file) {

    this->alu = new Alu(asm_file);
    this->data = new Data();
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

void Sys::push_stack(int8_t value) {

    this->data->push(value);
}

int8_t Sys::pop_stack(void) {

    return this->data->pop();
}

void Sys::write_data(int addr, int8_t value) {

    this->data->write(addr, value);
}

int8_t Sys::read_data(int addr) {

    return this->data->read(addr);
}

string Sys::data_to_str(void) {

    return this->data->to_str();
}

