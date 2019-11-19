/* EEPROM Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <tuple>
#include <sstream>

// Project Headers
#include "eeprom.hpp"
#include "mcu.hpp"
#include "style.hpp"

using namespace std;

/* --- Public --- */

Eeprom::Eeprom(void) {

    this->memory = (int8_t*) malloc(EEPROM_SIZE * sizeof(int8_t));
    memset(this->memory, 0x00, EEPROM_SIZE * sizeof(int8_t));

    this->cursor = (unsigned int) (EEPROM_SIZE / 2);
    this->color = make_tuple(0x0000, DEFAULT);
}

Eeprom::~Eeprom(void) {

    free(this->memory);
}

void Eeprom::write(int addr, int8_t value) {

    if(addr < 0x0000)
        return;

    if(addr >= EEPROM_SIZE)
        return;

    this->memory[addr] = value;

    this->set_color(addr, GREEN);
    this->cursor = addr;
}

int8_t Eeprom::read(int addr) {

    this->set_color(addr, RED);
    this->cursor = addr;

    return this->memory[addr];
}

void Eeprom::scale(int offs) {

    if((this->cursor + offs) > EEPROM_SIZE)
        return;

    if((this->cursor + offs) < 0x0000)
        return;

    this->cursor += offs;
}

vector <string> Eeprom::to_vector(void) {

    vector <string> out;
    out.push_back("EEPROM:");

    for(int i = (this->cursor - 4); i <= (this->cursor + 4); i++) {

        stringstream stream;

        if(i < 0 || i > EEPROM_SIZE) {

            out.push_back("");
            continue;
        }
        
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

void Eeprom::set_color(int cell, string color) {

    get <0> (this->color) = cell;
    get <1> (this->color) = color;
}

void Eeprom::clear_color(void) {

    get <0> (this->color) = 0;
    get <1> (this->color) = DEFAULT;
}
