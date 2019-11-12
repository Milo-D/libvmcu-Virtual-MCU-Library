/* Implementation of AVR Data Memory */

// C++ Header
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>

// Project Header
#include "data.hpp"
#include "mcu.hpp"
#include "style.hpp"

#define sp(spl, sph) ((sph << 8) + spl)
#define spl(sp) (((0x01 << 8) - 1) & sp)
#define sph(sp) (((((0x01 << 8) - 1) << 8) & sp) >> 8)

using namespace std;

Data::Data(void) {

    this->memory = (int8_t*) malloc((RAM_END + 1) * sizeof(int8_t));
    memset(this->memory, 0x00, sizeof(this->memory));

    this->cursor = SRAM_START;
}

Data::~Data(void) {

    free(this->memory);
}

void Data::push(int8_t value) {

    int16_t sp = sp(this->memory[SPL], this->memory[SPH]);

    if(sp <= SRAM_START)
        return;

    this->memory[sp--] = value;

    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);

    this->cursor = sp;
}

int8_t Data::pop(void) {

    /* in progress */
    return 0;
}

void Data::write(int addr, int8_t value) {

    this->cursor = addr;
    this->memory[addr] = value;
}

int8_t Data::read(int addr) {

    this->cursor = addr;
    return this->memory[addr];
}

void Data::scale(int offs) {

    if((this->cursor + offs) == RAM_END)
        return;

    if((this->cursor + offs) < 0x0000)
        return;

    this->cursor += offs;
}

string Data::to_str(void) {

    stringstream stream;
    stream << "Data Memory:\n\n"; 

    for(int i = (this->cursor - 4); i <= (this->cursor + 4); i++) {

        if(i < 0 || i > RAM_END) {

            stream << "\n";
            continue;
        }

        stream << "0x" << setfill('0') << setw(4);
        stream << hex << i << "      ";

        stream << "0x" << setfill('0') << setw(2);
        stream << hex << (int) this->memory[i] << "\n";
    }

    stream << SEPERATOR;
    return stream.str();
}

