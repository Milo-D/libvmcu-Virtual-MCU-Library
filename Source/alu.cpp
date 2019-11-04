/* ALU Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "alu.hpp"
#include "flash.hpp"
#include "gpr.hpp"
#include "instructions.hpp"

#include "ehandling.hpp"

using namespace std;

Alu::Alu(string asm_file) {

	this->flash = new Flash(asm_file);
	this->gpr = new Gpr(32, 0);
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

	/* Using Instruction Key to call the right function */

	(*instructions[key])(sys, opcode);

	return 0;
}

void Alu::write_reg(int rx, int8_t data) {

	this->gpr->write(rx, data);
}

int8_t Alu::read_reg(int rx) {

	return (this->gpr->read(rx));
}

string Alu::get_gpr(int cursor) {

	return this->gpr->to_str(cursor);
}


