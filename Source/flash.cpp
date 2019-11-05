/* AVR Flash Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

// Project Headers
#include "flash.hpp"
#include "mcu.hpp"
#include "ehandling.hpp"
#include "uparse.hpp"
#include "decoder.hpp"

using namespace std;

Flash::Flash(string asm_file) {

	string cmd = "avra -I /usr/share/avra/ " + asm_file;
	system(cmd.c_str());

	string hex_file = get_file_name(asm_file) + ".hex";
		
	ifstream read_file(hex_file, ios::in);

	if(read_file.good() == false)
		print_status("Could not create Hex File.", true);

	string line = "";

	while(getline(read_file, line))
		decode(this, line);

	// toDo: cleanup files
		
	read_file.close();
					
	this->pc = 0;
	this->size = FLASH_SIZE;
	this->size_used = this->app.size();
}

void Flash::insert_instr(int instr) {

	this->app.push_back(instr);
}

void Flash::insert_key(int key) {

	this->key.push_back(key);
}

int Flash::load_instr(void) {

	return this->app[this->pc];
}

int Flash::load_key(void) {

	return this->key[this->pc];
}

void Flash::pc_next(void) {

	if(this->pc == this->size_used - 1)
		return;

	this->pc += 1;
}

