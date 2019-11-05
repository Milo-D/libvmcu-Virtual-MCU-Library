/* EEPROM Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "eeprom.hpp"
#include "mcu.hpp"

using namespace std;

Eeprom::Eeprom(void) {

	this->data = (int*) malloc(EEPROM_SIZE * sizeof(int));
	this->size = EEPROM_SIZE;
}

Eeprom::~Eeprom() {

	free(this->data);
}
