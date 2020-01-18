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
#include "system/eeprom.hpp"
#include "system/mcu.hpp"
#include "misc/stringmanip.hpp"
#include "cli/debugwindow.hpp"
#include "cli/style.hpp"

using namespace std;

/* --- Public --- */

Eeprom::Eeprom(void) {

    this->memory = (int8_t*) malloc(EEPROM_SIZE * sizeof(int8_t));
    memset(this->memory, 0x00, EEPROM_SIZE * sizeof(int8_t));

    this->cursor = (unsigned int) (EEPROM_SIZE / 2);
    this->color = make_tuple(0x0000, DEF);
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

    this->set_color(addr, G);
    this->cursor = addr;
}

int8_t Eeprom::read(int addr) {

    this->set_color(addr, R);
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

void Eeprom::to_win(DebugWindow *dwin) {

    stringstream stream;
    dwin->write(EEPROM_PANEL, "EEPROM:\n\n", DEF);

    for(int i = (this->cursor - 4); i <= (this->cursor + 4); i++) {

        int ism = DEF;

        if(i < 0 || i > EEPROM_SIZE) {

            dwin->write(EEPROM_PANEL, "\n", DEF);
            continue;
        }

        stream << "0x" << setfill('0') << setw(4);
        stream << hex << i << "      ";

        dwin->write(EEPROM_PANEL, stream.str(), DEF);
        stream.str(string());

        if(i == get <0> (this->color))
            ism = get <1> (this->color);

        stream << "0x" << setfill('0') << setw(2);
        stream << get_hex(this->memory[i]);
        
        dwin->write(EEPROM_PANEL, stream.str() + "\n", ism);
        stream.str(string());
    }

    this->clear_color();
}

/* --- Private --- */

void Eeprom::set_color(int cell, int color) {

    get <0> (this->color) = cell;
    get <1> (this->color) = color;
}

void Eeprom::clear_color(void) {

    get <0> (this->color) = 0;
    get <1> (this->color) = DEF;
}
