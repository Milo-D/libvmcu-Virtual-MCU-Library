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
#include "misc/stringmanip.hpp"
#include "cli/debugwindow.hpp"
#include "cli/style.hpp"

#define sp(spl, sph) ((sph << 8) + spl)
#define spl(sp) (((0x01 << 8) - 1) & sp)
#define sph(sp) (((((0x01 << 8) - 1) << 8) & sp) >> 8)

using namespace std;

/* --- Public --- */

Data::Data(void) {

    this->memory = (int8_t*) malloc((RAM_END + 1) * sizeof(int8_t));
    memset(this->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));

    this->cursor = SRAM_START;
    this->color = make_tuple(0x0000, DEF);
}

Data::~Data(void) {

    free(this->memory);
}

void Data::push(int8_t value) {

    uint8_t spl = this->memory[SPL];
    uint8_t sph = this->memory[SPH];
    
    uint16_t sp = sp(spl, sph);

    if(sp <= SRAM_START)
        return;

    this->memory[sp--] = value;

    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);

    this->set_color(sp + 1, G);
    this->cursor = (sp + 1);
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

    this->set_color(sp, R);
    this->cursor = sp;

    return value;
}

void Data::write(int addr, int8_t value) {

    if(addr < 0 || addr > RAM_END)
        return;

    this->memory[addr] = value;

    this->set_color(addr, G);
    this->cursor = addr;
}

int8_t Data::read(int addr) {

    if(addr < 0 || addr > RAM_END)
        return 0xff;

    this->set_color(addr, R);
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

void Data::to_win(DebugWindow *dwin) {

    stringstream stream;
    dwin->write(DATA_PANEL, "Data Memory:\n\n", DEF);

    int16_t sp = sp(this->memory[SPL], this->memory[SPH]);

    for(int i = (this->cursor - 4); i <= (this->cursor + 4); i++) {

        int isp = DEF; int ism = DEF;

        if(i < 0 || i > RAM_END) {

            dwin->write(DATA_PANEL, "\n", DEF);
            continue;
        }

        if(i == sp)
            isp = B;

        if(i == get <0> (this->color))
            ism = get <1> (this->color);

        stream << "0x" << setfill('0') << setw(4);
        stream << hex << i << "      ";

        dwin->write(DATA_PANEL, stream.str(), isp);
        stream.str(string());

        stream << "0x" << setfill('0') << setw(2);
        stream << get_hex(this->memory[i]);

        dwin->write(DATA_PANEL, stream.str() + "\n", ism);
        stream.str(string());
    }

    this->clear_color();
}

/* --- Private --- */

void Data::set_color(int cell, int color) {

    get <0> (this->color) = cell;
    get <1> (this->color) = color;
}

void Data::clear_color(void) {

    get <0> (this->color) = 0x0000;
    get <1> (this->color) = DEF;
}

