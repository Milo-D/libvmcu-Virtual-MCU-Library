/* EEPROM Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "eeprom.hpp"

using namespace std;

Eeprom::Eeprom(unsigned int size) {

	this->data = (int*) malloc(size * sizeof(int));
}

Eeprom::~Eeprom() {

	free(this->data);
}
