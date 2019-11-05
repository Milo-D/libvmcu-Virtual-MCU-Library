/* GPR Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Project Headers
#include "gpr.hpp"
#include "mcu.hpp"
#include "style.hpp"

using namespace std;

/* --- Public --- */

Gpr::Gpr(void) {

    this->reg = (int8_t*) malloc(GPR_SIZE * sizeof(int8_t));

    for(int i = 0; i < GPR_SIZE; i++) {

        this->reg[i] = 0;
        this->color.push_back(DEFAULT);
    }

    this->size = GPR_SIZE;
}

Gpr::~Gpr() {

    free(this->reg);
}

void Gpr::write(int rx, int8_t data) {

    this->reg[rx] = data;
    this->color[rx] = GREEN;
}

int8_t Gpr::read(int rx) {

    this->color[rx] = RED;
    return this->reg[rx];
}

string Gpr::to_str(int cursor) {

    stringstream stream;
	
    stream << SEPERATOR;
    stream << "Registers:\n\n";
	
    int start = (cursor * 8);
	
    for(int i = 0; i < 8; i++) {

        if(i == 4)
            stream << "\n";

        stream << this->color[start + i];
        stream << "R" << to_string(start + i) << DEFAULT << ": ";

        if(start + i < 10)
            stream << " ";

        stream << "0x" << right << setw(2) << setfill('0');
        stream << hex << (int) this->reg[start + i];

        stream << setfill(' ') << left << setw(4);
        stream << "      ";	
    }

    stream << dec << "\n";
    this->clear_color();
	
    return stream.str();
}

/* --- Private --- */

void Gpr::clear_color(void) {

    for(int i = 0; i < this->size; i++)
        this->color[i] = DEFAULT;
}


