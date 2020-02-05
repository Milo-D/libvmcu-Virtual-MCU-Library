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
#include "printer/memprop.hpp"

using namespace std;

/* --- Public --- */

Sreg::Sreg(void) {

    for(int i = 0; i < SREG_SIZE; i++)
        this->coi.push_back(NONE);

    this->status = 0x00;
}

void Sreg::write(const int flag, const bool bit) {

    if(flag < 0 || flag >= SREG_SIZE)
        return;

    if(bit == true) {

        this->status |= (0x01 << flag);
        this->coi[flag] = DEST;
        
        return;
    }
	
    this->status &= ~(0x01 << flag);
    this->coi[flag] = DEST;
}

bool Sreg::read(const int flag) {

    if(flag < 0 || flag >= SREG_SIZE)
        return false;

    this->coi[flag] = SRC;
    return ((this->status >> flag) & 0x01); 
}

void Sreg::clear(void) {

    this->status = 0x00;
}

void Sreg::get_coi(vector <int> & buffer) {

    for(int i = 0; i < SREG_SIZE; i++)
        buffer.push_back(this->coi[i]);

    this->clear_coi();
}

uint8_t Sreg::dump(void) {

    return this->status;
}

/* --- Private --- */

void Sreg::clear_coi(void) {

    for(int i = 0; i < SREG_SIZE; i++)
        this->coi[i] = NONE;
}

