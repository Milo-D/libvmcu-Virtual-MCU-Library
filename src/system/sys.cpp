/* (Sys)tem Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// Project Headers
#include "system/sys.hpp"
#include "system/alu.hpp"
#include "system/gpr.hpp"
#include "system/data.hpp"
#include "system/flash.hpp"
#include "system/eeprom.hpp"
#include "table/table.hpp"
#include "misc/stringmanip.hpp"

using namespace std;

Sys::Sys(Table *table) {

    this->alu = new Alu(table);
    this->data = new Data();
    this->eeprom = new Eeprom();

    this->steps = 0;
    this->terminated = false;
}

Sys::~Sys(void) {

    delete this->alu;
    delete this->data;
    delete this->eeprom;
}

void Sys::step(void) {

    if(this->terminated == true)
        return;

    if(this->alu->fetch(this) < 0)
        this->kill();

    this->steps += 1;
}

void Sys::backstep(Table *table) {

    if(this->terminated == true)
        return;

    int counter = (this->steps - 1);
    
    this->reboot(table);

    while(counter-- > 0)
        this->step();
}

void Sys::reboot(Table *table) {

    delete this->alu;
    delete this->data;
    delete this->eeprom;

    this->alu = new Alu(table);
    this->data = new Data();
    this->eeprom = new Eeprom();

    this->steps = 0;
    table->set_tip(0);
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

void Sys::scale_gpr(int offs) {

    this->alu->scale_gpr(offs);
}

string Sys::gpr_to_str(void) {

    return this->alu->get_gpr();
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

int Sys::get_pc(void) {

    return this->alu->get_pc();
}

void Sys::set_pc(int addr) {

    this->alu->set_pc(addr);
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

void Sys::scale_data(int offs) {

    this->data->scale(offs);
}

void Sys::write_eeprom(int addr, int8_t value) {

    this->eeprom->write(addr, value);
}

int8_t Sys::read_eeprom(int addr) {

    return this->eeprom->read(addr);
}

void Sys::scale_eeprom(int offs) {

    this->eeprom->scale(offs);
}

string Sys::memory_to_str(void) {

    return mix_memory(data->to_vector(), eeprom->to_vector());
}

string Sys::table_to_str(void) {

    return this->alu->get_table();
}

