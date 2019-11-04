/* Status Register Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Project Headers
#include "sreg.hpp"
#include "style.hpp"

#define SREG_SIZE 8

using namespace std;

const string flags[SREG_SIZE] = { "IF", "TF", "HF", "SF", "VF", "NF", "ZF", "CF" };

/* --- Public --- */

Sreg::Sreg(void) {

	this->status = 0x00;
}

void Sreg::write(int flag, bool bit) {

	this->color[flag] = GREEN;

	if(status == true) {

		this->status |= (0x01 << flag);
		return;
	}
	
	this->status &= ~(0x01 << flag);
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

