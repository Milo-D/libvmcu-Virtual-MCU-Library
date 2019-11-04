/* GPR Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Project Headers
#include "gpr.hpp"
#include "style.hpp"

using namespace std;

/* --- Public --- */

Gpr::Gpr(unsigned int amount, int8_t init_val) {

	this->reg = (int8_t*) malloc(amount * sizeof(int8_t));

	for(int i = 0; i < amount; i++) {

		this->reg[i] = init_val;
		this->color[i] = DEFAULT;
	}

	this->size = amount;
}

Gpr::~Gpr() {

	free(this->reg);
	free(this->color);
}

void Gpr::write(int rx, int8_t data) {

	this->clear_color();

	this->reg[rx] = data;
	this->color[rx] = GREEN;
}

int8_t Gpr::read(int rx) {

	this->clear_color();
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


