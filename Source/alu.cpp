/* ALU Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "alu.hpp"
#include "flash.hpp"
#include "gpr.hpp"
#include "sreg.hpp"
#include "instructions.hpp"

using namespace std;

Alu::Alu(string asm_file) {

	this->flash = new Flash(asm_file);
	this->gpr = new Gpr();
	this->sreg = new Sreg();
}

Alu::~Alu() {

	delete flash;
	delete gpr;
}

int Alu::fetch(Sys *sys) {

	int opcode = this->flash->load_instr();
	int key = this->flash->load_key();
	
	this->flash->pc_next();

	if(opcode < 0)
		return -1;

	/* Using the Instruction Key to call the right function */

	(*instructions[key])(sys, opcode);

	return 0;
}

void Alu::write_gpr(int rx, int8_t data) {

	this->gpr->write(rx, data);
}

int8_t Alu::read_gpr(int rx) {

	return (this->gpr->read(rx));
}

string Alu::get_gpr(int cursor) {

	return this->gpr->to_str(cursor);
}

void Alu::write_sreg(int flag, bool bit) {

	this->sreg->write(flag, bit);
}

bool Alu::read_sreg(int flag) {

	return (this->sreg->read(flag));		
}

string Alu::get_sreg(void) {

	return this->sreg->to_str();
}





