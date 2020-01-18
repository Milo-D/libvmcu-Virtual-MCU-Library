/* GPR Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>

// Project Headers
#include "system/gpr.hpp"
#include "system/mcu.hpp"
#include "misc/stringmanip.hpp"
#include "cli/debugwindow.hpp"
#include "cli/style.hpp"

using namespace std;

/* --- Public --- */

Gpr::Gpr(void) {

    this->reg = (int8_t*) malloc(GPR_SIZE * sizeof(int8_t));
    memset(this->reg, 0x00, GPR_SIZE * sizeof(int8_t));

    for(int i = 0; i < GPR_SIZE; i++)
        this->color.push_back(DEF);

    this->size = GPR_SIZE;
    this->cursor = 0;
}

Gpr::~Gpr(void) {

    free(this->reg);
}

void Gpr::write(int rx, int8_t data) {

    this->reg[rx] = data;
    this->color[rx] = G;
}

int8_t Gpr::read(int rx) {

    this->color[rx] = R;
    return this->reg[rx];
}

void Gpr::scale(int offs) {

    if((this->cursor + offs) >= (GPR_SIZE / 8))
        return;

    if((this->cursor + offs) < 0)
        return;

    this->cursor += offs;
}

void Gpr::to_win(DebugWindow *dwin) {

	stringstream stream;	
    int start = (this->cursor * 8);

	dwin->write(GPR_PANEL, "Registers:\n\n", DEF);
	
    for(int i = 0; i < 8; i++) {

        if(i == 4)
            dwin->write(GPR_PANEL, "\n", DEF);

        dwin->write(GPR_PANEL, "R" + to_string(start + i), this->color[start + i]);
        dwin->write(GPR_PANEL, ": ", DEF);

        if(start + i < 10)
            dwin->write(GPR_PANEL, " ", DEF);

        stream << "0x" << right << setw(2) << setfill('0');
        stream << get_hex(this->reg[start + i]);

        stream << setfill(' ') << left << setw(4);
        stream << "      ";

        dwin->write(GPR_PANEL, stream.str(), DEF);
        stream.str(string());
    }

    dwin->write(GPR_PANEL, "\n", DEF);
    this->clear_color();
}

/* --- Private --- */

void Gpr::clear_color(void) {

    for(int i = 0; i < this->size; i++)
        this->color[i] = DEF;
}


