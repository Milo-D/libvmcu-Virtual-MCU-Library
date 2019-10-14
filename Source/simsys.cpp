/* Implementation of Simulated System */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <stdlib.h>

// Project Headers
#include "simsys.hpp"
#include "ehandling.hpp"
#include "style.hpp"
#include "simstack.hpp"

#define MAX_MEM 1024

using namespace std;

extern const string registers[REG_NO] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                                          "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", 
                                          "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
                                          "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31" };

static const string flags[FLAG_NO] = { "CF", "ZF", "NF", "VF", "SF", "HF", "TF", "IF" };

SimSys::SimSys(unsigned int heap_size, unsigned int stack_size) {

	if((this->heap = (int*) malloc(heap_size * sizeof(int))) == NULL)
		print_status("Fatal, could not allocate Heap Memory", true);

	this->stack = (SimStack*) new SimStack(stack_size);

	for(int i = 0; i < REG_NO; i++)
		this->reg[i] = 0;

	this->flag = 0x00;
}

SimSys::~SimSys(void) {

	free(this->heap);
	delete stack;
}

int SimSys::read_reg(string reg_string) {

	int reg_id = 0;

	for(int i = 0; i < REG_NO; i++) {

		if(reg_string == registers[i]) {

			reg_id = i;
			break;
		}
	}

	this->reg_col[reg_id] = RED;
	return this->reg[reg_id];
}

void SimSys::write_reg(string reg_string, int data) {

	int reg_id = 0;

	for(int i = 0; i < REG_NO; i++) {

		if(reg_string == registers[i]) {

			reg_id = i;
			break;
		}
	}

	this->reg_col[reg_id] = GREEN;
	this->reg[reg_id] = data;
}

int SimSys::read_flag(uint8_t flag_id) {

	this->flag_col[flag_id] = RED;
	return ((this->flag & (0x01 << flag_id)) >> flag_id);
}

void SimSys::set_flag(uint8_t flag_id, bool status) {

	this->flag_col[flag_id] = GREEN;

	if(status == true) {
	
		this->flag |= (0x01 << flag_id);
		return;
	}

	this->flag &= ~(0x01 << flag_id);
}

void SimSys::reset_col(int reg_id) {

	this->reg_col[reg_id] = "";
}

void SimSys::print_reg(int segment) {

	cout << HORI_SEP;
	cout << "Registers:\n\n";

	int seg_start = segment * 8;
	int seg_end = seg_start + 8;
	
	for(int i = seg_start; i < seg_end; i++) {

		if(i == seg_start + 4)
			cout << "\n";

		cout << reg_col[i];
		cout << registers[i] << DEFAULT;

		if(i <= 9)
			cout << " ";
		
		cout << ": ";
		cout << setfill(' ') << right << setw(5);

		if(i >= R26 && i <= R31)
			cout << hex << "0x" << this->reg[i];
		else
			cout << dec << this->reg[i];

		cout << setfill(' ') << left << setw(4);
		cout << "\t";
	}
	
	cout << dec << "\n";

	for(int i = 0; i < REG_NO; i++)
		this->reset_col(i);

	cout << HORI_SEP;
}

void SimSys::print_flag(void) {

	cout << "Flags:\n\n";

	for(int i = 0; i < FLAG_NO; i++) {

		if(i == 4)
			cout << "\n";

		cout << flag_col[i];
		cout << flags[i] << DEFAULT << " : ";

		cout << setfill(' ') << right << setw(4);
		cout << " " << ((this->flag & (0x01 << i)) >> i);
		cout << setfill(' ') << left << setw(4);
		cout << "\t";
	}

	cout << "\n";

	for(int i = 0; i < FLAG_NO; i++)
		this->flag_col[i] = "";
}

void SimSys::exit(void) {

	for(int i = 0; i < REG_NO; i++)
		this->reg[i] = 0;

	this->flag = 0x00;
	this->terminated = true;
}


