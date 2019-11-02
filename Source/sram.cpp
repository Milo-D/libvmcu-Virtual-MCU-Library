/* SRAM Implemetation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "sram.hpp"

using namespace std;

Sram::Sram(unsigned int size) {

	this->ram = (int*) malloc(size * sizeof(int));
}

Sram::~Sram() {

	free(this->ram);
}
