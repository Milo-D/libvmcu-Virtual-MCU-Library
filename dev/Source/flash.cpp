/* AVR Flash Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "flash.hpp"
#include "decoder.hpp"

using namespace std;

Flash::Flash(string hex_file) {

	// toDo: decode

	decode(hex_file, this->app);

	this->pc = 0;
	this->size_used = this->app.size();
}

int Flash::inc_pc(void) {

	if(this->pc >= this->size_used)
		return -1;

	this->pc += 1;

	return 0;
}

int Flash::dec_pc(void) {

	if(this->pc <= 0)
		return -1;

	this->pc -= 1;
}

string Flash::load_instr(void) {

	return this->app[this->pc];
}

