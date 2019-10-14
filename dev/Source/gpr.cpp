/* GPR Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "gpr.hpp"

using namespace std;

Gpr::Gpr(unsigned int amount, int8_t init_val) {

	this->reg = (int8_t*) malloc(amount * sizeof(int8_t));
	this->color = (string*) malloc(amount * sizeof(string));

	this->reg_size = amount;

	// toDo: free(...)
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

void Gpr::clear_color(void) {

	for(int i = 0; i < this->reg_size; i++)
		this->color[i] = DEFAULT;
}


