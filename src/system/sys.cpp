/* (Sys)tem Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <tuple>

// Project Headers
#include "system/sys.hpp"
#include "system/alu.hpp"
#include "system/gpr.hpp"
#include "system/data.hpp"
#include "system/flash.hpp"
#include "system/eeprom.hpp"
#include "table/table.hpp"

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

void Sys::write_gpr(const int rx, const int8_t data) {

    this->alu->write_gpr(rx, data);
}

int8_t Sys::read_gpr(const int rx) {

    return this->alu->read_gpr(rx);
}

void Sys::gpr_coi(vector <int> & buffer) {

    this->alu->gpr_coi(buffer);
}

void Sys::dump_gpr(vector <int8_t> & buffer) {

    this->alu->dump_gpr(buffer);
}

void Sys::write_sreg(const int flag, const bool bit) {

    this->alu->write_sreg(flag, bit);
}

bool Sys::read_sreg(const int flag) {

    return this->alu->read_sreg(flag);
}

void Sys::sreg_coi(vector <int> & buffer) {

    this->alu->sreg_coi(buffer);
}

uint8_t Sys::dump_sreg(void) {

    return this->alu->dump_sreg();
}

int Sys::get_pc(void) {

    return this->alu->get_pc();
}

void Sys::set_pc(const int addr) {

    this->alu->set_pc(addr);
}

void Sys::push_stack(const int8_t value) {

    this->data->push(value);
}

int8_t Sys::pop_stack(void) {

    return this->data->pop();
}

void Sys::write_data(const int addr, const int8_t value) {

    this->data->write(addr, value);
}

int8_t Sys::read_data(const int addr) {

    return this->data->read(addr);
}

void Sys::data_coi(tuple <int, int> & buffer) {

    this->data->get_coi(buffer);
}

void Sys::dump_data(vector <int8_t> & buffer) {

    this->data->dump(buffer);
}

void Sys::write_eeprom(const int addr, const int8_t value) {

    this->eeprom->write(addr, value);
}

int8_t Sys::read_eeprom(const int addr) {

    return this->eeprom->read(addr);
}

void Sys::eeprom_coi(tuple <int, int> & buffer) {

    this->eeprom->get_coi(buffer);
}

void Sys::dump_eeprom(vector <int8_t> & buffer) {

    this->eeprom->dump(buffer);
}

