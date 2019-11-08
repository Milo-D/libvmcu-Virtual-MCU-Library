/* SRAM Implemetation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "sram.hpp"
#include "mcu.hpp"

using namespace std;

Sram::Sram(void) {

    this->ram = (int8_t*) malloc(SRAM_SIZE * sizeof(int));
}

Sram::~Sram() {

    free(this->ram);
}
