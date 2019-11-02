/* (Sys)tem Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "sys.hpp"
#include "alu.hpp"
#include "gpr.hpp"
#include "sram.hpp"
#include "flash.hpp"
#include "eeprom.hpp"

using namespace std;

Sys::Sys(string asm_file, unsigned int heap_size, unsigned int stack_size) {

	/* Initializing System Components */

	this->alu = new Alu(asm_file);
	this->sram = new Sram(1024);
	this->eeprom = new Eeprom(1024);

	this->terminated = false;
}

void Sys::step(void) {

	/* alu->fetch(void) --> flash->load_instr --> execute */

	if(this->terminated == true)
		return;

	if(this->alu->fetch(this) < 0)
		this->terminated = true;
}

bool Sys::is_terminated(void) {

	return this->terminated;	
}

void Sys::write_gpr(int rx, int8_t data) {

	this->alu->write_reg(rx, data);	
}

int8_t Sys::read_gpr(int rx) {

	return (this->alu->read_reg(rx));
}

string Sys::gpr_to_str(int cursor) {

	return this->alu->get_gpr(cursor);
}

