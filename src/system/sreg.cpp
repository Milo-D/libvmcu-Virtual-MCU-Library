/* Status Register Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Project Headers
#include "system/sreg.hpp"
#include "system/mcu.hpp"
#include "cli/style.hpp"

using namespace std;

/* --- Public --- */

Sreg::Sreg(void) {

    this->status = 0x00;
}

void Sreg::write(int flag, bool bit) {

    if(bit == true) {

        this->status |= (0x01 << flag);
        this->color[flag] = GREEN;
        
        return;
    }
	
    this->status &= ~(0x01 << flag);
    this->color[flag] = GREEN;
}

bool Sreg::read(int flag) {

    this->color[flag] = RED;
    return ((this->status >> flag) & 0x01); 
}

void Sreg::clear(void) {

    this->status = 0x00;
}

string Sreg::to_str(void) {

    stringstream stream;

    stream << SEPERATOR;
    stream << "SREG:\n\n";

    for(int i = 0; i < SREG_SIZE; i++) {

        if(i == SREG_SIZE / 2)
            stream << "\n";

        stream << this->color[i];
        stream << flags[i] << DEFAULT << ": ";

        stream << setfill(' ') << right << setw(2);
        stream << " " << this->read(i);
        stream << setfill(' ') << left << setw(4);
        stream << "        ";	
    }

    stream << "\n";
    this->clear_color();

    return stream.str();
}

/* --- Private --- */

void Sreg::clear_color(void) {

    for(int i = 0; i < SREG_SIZE; i++)
        this->color[i] = DEFAULT;
}

