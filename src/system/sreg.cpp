/* Status Register Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>  
#include <sstream>

// Project Headers
#include "system/sreg.hpp"
#include "system/mcu.hpp"
#include "cli/debugwindow.hpp"
#include "cli/style.hpp"

using namespace std;

/* --- Public --- */

Sreg::Sreg(void) {

    this->status = 0x00;
    memset(this->color, DEF, SREG_SIZE * sizeof(int));
}

void Sreg::write(int flag, bool bit) {

    if(bit == true) {

        this->status |= (0x01 << flag);
        this->color[flag] = G;
        
        return;
    }
	
    this->status &= ~(0x01 << flag);
    this->color[flag] = G;
}

bool Sreg::read(int flag) {

    this->color[flag] = R;
    return ((this->status >> flag) & 0x01); 
}

void Sreg::clear(void) {

    this->status = 0x00;
}

void Sreg::to_win(DebugWindow *dwin) {

    stringstream stream;
    dwin->write(SREG_PANEL, "Status-Register:\n\n", DEF);

    for(int i = 0; i < SREG_SIZE; i++) {

        if(i == SREG_SIZE / 2)
            dwin->write(SREG_PANEL, "\n", DEF);

        dwin->write(SREG_PANEL, flags[i], this->color[i]);

        stream << ": " << setfill(' ') << right << setw(2);
        stream << " 0x0" << hex << this->read(i) << dec;
        stream << setfill(' ') << left << setw(13);
        stream << "        ";

        dwin->write(SREG_PANEL, stream.str(), DEF);
        stream.str(string());
    }

    dwin->write(SREG_PANEL, "\n", DEF);
    this->clear_color();
}

/* --- Private --- */

void Sreg::clear_color(void) {

    for(int i = 0; i < SREG_SIZE; i++)
        this->color[i] = DEF;
}

