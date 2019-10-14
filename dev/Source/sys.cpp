/* System Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "alu.hpp"
#include "gpr.hpp"
#include "sram.hpp"
#include "flash.hpp"
#include "eeprom.hpp"

Sys::Sys() {

	/* Initializing System Components */

	alu = (Alu*) malloc(sizeof(Alu));
	gpr = (Gpr*) malloc(sizeof(Gpr));
	sram = (Sram*) malloc(sizeof(Sram));
	eeprom = (Eeprom*) malloc(sizeof(Eeprom));

	// toDo: Call Constructor and pass System Data
}

Sys::~Sys() {

	/* Releasing System Components */

	free(alu);
	free(gpr);
	free(sram);
	free(eeprom);
}

void Sys::write_reg(int rx, int8_t data) {

	this->gpr->write(rx, data);	
}

int8_t Sys::read_reg(int rx) {

	return (this->gpr->read(rx));
}

void Sys::step(void) {

	/* alu->fetch(void) --> flash->load_instr --> execute */
	this->alu->fetch();
	
	if(this->flash->inc_pc() < 0)
		system_event("Reached File limits.", true);

	/* system_event notifies about
	*  system related Events like
	*  internal Exceptions or
	*  Warnings. If boolean equals
	*  true, the simulated AVR System
	*  terminates.
	**/
}

