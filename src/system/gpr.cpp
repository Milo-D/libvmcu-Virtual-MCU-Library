/* GPR Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>

// Project Headers
#include "system/gpr.hpp"
#include "system/mcu.hpp"
#include "printer/memprop.hpp"

using namespace std;

/* --- Public --- */

Gpr::Gpr(void) {

    this->reg = (int8_t*) malloc(GPR_SIZE * sizeof(int8_t));
    memset(this->reg, 0x00, GPR_SIZE * sizeof(int8_t));

    for(int i = 0; i < GPR_SIZE; i++)
        this->coi.push_back(NONE);

    this->size = GPR_SIZE;
}

Gpr::~Gpr(void) {

    free(this->reg);
}

void Gpr::write(const int rx, const int8_t data) {

    this->reg[rx] = data;
    this->coi[rx] = DEST;
}

int8_t Gpr::read(const int rx) {

    this->coi[rx] = SRC;
    return this->reg[rx];
}

void Gpr::get_coi(vector <int> & buffer) {

    for(int i = 0; i < this->size; i++)
        buffer.push_back(this->coi[i]);

    this->clear_coi();
}

void Gpr::dump(vector <int8_t> & buffer) {

    for(int i = 0; i < this->size; i++)
        buffer.push_back(this->reg[i]);
}

/* --- Private --- */

void Gpr::clear_coi(void) {

    for(int i = 0; i < this->size; i++)
        this->coi[i] = NONE;
}


