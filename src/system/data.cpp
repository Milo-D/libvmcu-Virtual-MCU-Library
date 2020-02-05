/* Implementation of AVR Data Memory */

// C++ Header
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>
#include <tuple>

// Project Header
#include "system/data.hpp"
#include "system/mcu.hpp"
#include "printer/memprop.hpp"
#include "misc/stringmanip.hpp"

#define sp(spl, sph) ((sph << 8) + spl)
#define spl(sp) (((0x01 << 8) - 1) & sp)
#define sph(sp) (((((0x01 << 8) - 1) << 8) & sp) >> 8)

using namespace std;

/* --- Public --- */

Data::Data(void) {

    this->memory = (int8_t*) malloc((RAM_END + 1) * sizeof(int8_t));
    memset(this->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));

    this->coi = make_tuple(0x0000, NONE);
}

Data::~Data(void) {

    free(this->memory);
}

void Data::push(const int8_t value) {

    uint8_t spl = this->memory[SPL];
    uint8_t sph = this->memory[SPH];
    
    uint16_t sp = sp(spl, sph);

    if(sp <= SRAM_START)
        return;

    this->memory[sp--] = value;
    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);

    this->set_coi(sp + 1, DEST);
}

int8_t Data::pop(void) {

    uint8_t spl = this->memory[SPL];
    uint8_t sph = this->memory[SPH];
    
    uint16_t sp = sp(spl, sph);

    if(sp >= RAM_END)
        return 0xff;

    int8_t value = this->memory[++sp];

    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);

    this->set_coi(sp, SRC);
    return value;
}

void Data::write(const int addr, const int8_t value) {

    if(addr < 0 || addr > RAM_END)
        return;

    this->memory[addr] = value;
    this->set_coi(addr, DEST);
}

int8_t Data::read(const int addr) {

    if(addr < 0 || addr > RAM_END)
        return 0xff;

    this->set_coi(addr, SRC);
    return this->memory[addr];
}

void Data::get_coi(tuple <int, int> & buffer) {

    get <0> (buffer) = get <0> (this->coi);
    get <1> (buffer) = get <1> (this->coi);

    this->clear_coi();
}

void Data::dump(vector <int8_t> & buffer) {

    for(int i = 0; i < (RAM_END + 1); i++)
        buffer.push_back(this->memory[i]);
}

/* --- Private --- */

void Data::set_coi(const int cell, const int prop) {

    get <0> (this->coi) = cell;
    get <1> (this->coi) = prop;
}

void Data::clear_coi(void) {

    get <0> (this->coi) = 0x0000;
    get <1> (this->coi) = NONE;
}

