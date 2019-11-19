/* Implementation of AVR Data Memory */

// C++ Header
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <tuple>

// Project Header
#include "data.hpp"
#include "mcu.hpp"
#include "style.hpp"

#define sp(spl, sph) ((sph << 8) + spl)
#define spl(sp) (((0x01 << 8) - 1) & sp)
#define sph(sp) (((((0x01 << 8) - 1) << 8) & sp) >> 8)

using namespace std;

/* --- Public --- */

Data::Data(void) {

    this->memory = (int8_t*) malloc((RAM_END + 1) * sizeof(int8_t));
    memset(this->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));

    this->cursor = SRAM_START;
    this->color = make_tuple(0x0000, DEFAULT);
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

    this->set_color(sp + 1, GREEN);
    this->cursor = (sp + 1);
}

int8_t Data::pop(void) {

    int16_t sp = sp(this->memory[SPL], this->memory[SPH]);

    if(sp >= RAM_END)
        return 0xff;

    int8_t value = this->memory[++sp];

    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);

    this->set_color(sp, RED);
    this->cursor = sp;

    return value;
}

void Data::write(int addr, int8_t value) {

    this->memory[addr] = value;

    this->set_color(addr, GREEN);
    this->cursor = addr;
}

int8_t Data::read(int addr) {

    this->set_color(addr, RED);
    this->cursor = addr;

    return this->memory[addr];
}

void Data::scale(int offs) {

    if((this->cursor + offs) > RAM_END)
        return;

    if((this->cursor + offs) < 0x0000)
        return;

    this->cursor += offs;
}

vector <string> Data::to_vector(void) {

    vector <string> out;
    out.push_back("Data Memory:");

    int sp = sp(this->memory[SPL], this->memory[SPH]);

    for(int i = (this->cursor - 4); i <= (this->cursor + 4); i++) {

        stringstream stream;

        if(i < 0 || i > RAM_END) {

            out.push_back(SPACING);
            continue;
        }

        if(i == sp)
            stream << BLUE;

        stream << "0x" << setfill('0') << setw(4);
        stream << hex << i << "      " << DEFAULT;

        if(i == get <0> (this->color))
            stream << get <1> (this->color);

        stream << "0x" << setfill('0') << setw(2);
        stream << hex << (int) this->memory[i];
        stream << DEFAULT;

        out.push_back(stream.str());
    }

    this->clear_color();
    return out;
}

/* --- Private --- */

void Data::set_color(int cell, string color) {

    get <0> (this->color) = cell;
    get <1> (this->color) = color;
}

void Data::clear_color(void) {

    get <0> (this->color) = 0;
    get <1> (this->color) = DEFAULT;
}

