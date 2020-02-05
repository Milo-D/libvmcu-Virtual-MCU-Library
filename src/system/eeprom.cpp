/* EEPROM Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <tuple>

// Project Headers
#include "system/eeprom.hpp"
#include "system/mcu.hpp"
#include "printer/memprop.hpp"

using namespace std;

/* --- Public --- */

Eeprom::Eeprom(void) {

    this->memory = (int8_t*) malloc(EEPROM_SIZE * sizeof(int8_t));
    memset(this->memory, 0x00, EEPROM_SIZE * sizeof(int8_t));

    this->coi = make_tuple(0x0000, NONE);
}

Eeprom::~Eeprom(void) {

    free(this->memory);
}

void Eeprom::write(const int addr, const int8_t value) {

    if(addr < 0x0000)
        return;

    if(addr >= EEPROM_SIZE)
        return;

    this->memory[addr] = value;
    this->set_coi(addr, DEST);
}

int8_t Eeprom::read(const int addr) {

    if(addr < 0x0000)
        return 0xff;

    if(addr >= EEPROM_SIZE)
        return 0xff;

    this->set_coi(addr, SRC);
    return this->memory[addr];
}

void Eeprom::get_coi(tuple <int, int> & buffer) {

    get <0> (buffer) = get <0> (this->coi);
    get <1> (buffer) = get <1> (this->coi);

    this->clear_coi();
}

void Eeprom::dump(vector <int8_t> & buffer) {

    for(int i = 0; i < EEPROM_SIZE; i++)
        buffer.push_back(this->memory[i]);
}

/* --- Private --- */

void Eeprom::set_coi(const int cell, const int prop) {

    get <0> (this->coi) = cell;
    get <1> (this->coi) = prop;
}

void Eeprom::clear_coi(void) {

    get <0> (this->coi) = 0;
    get <1> (this->coi) = NONE;
}
