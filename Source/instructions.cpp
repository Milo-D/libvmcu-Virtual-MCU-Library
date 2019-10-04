/* Assembly Instruction Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "instructions.hpp"
#include "ehandling.hpp"
#include "style.hpp"
#include "uparse.hpp"
#include "asmparser.hpp"
#include "table.hpp"
#include "simsys.hpp"
#include "attributes.hpp"
#include "syscall.hpp"

#define SRC 0
#define DEST 1
#define PR_DELAY 2

using namespace std;

void skip(SimSys *sys, Table *table, struct attributes *attr) {

	/* Label Skipping */

	return;
}

void mov(SimSys *sys, Table *table, struct attributes *attr) {

	int source;
	
	if((source = to_dec(attr->src)) != -1) {

		sys->write_reg(attr->dest, source);
		
	} else {
		
		source = sys->read_reg(attr->src);
		sys->write_reg(attr->dest, source);
	}
}

void push(SimSys *sys, Table *table, struct attributes *attr) {

	int source;

	if((source = to_dec(attr->src)) != -1) {

		if(sys->push_stack(source) < 0)
			print_delay("Segmentation Fault.", PR_DELAY, false);

		return;
	}
	
	source = sys->read_reg(attr->src);

	if(sys->push_stack(source) < 0)
		print_delay("Segmentation Fault.", PR_DELAY, false);
}

void pop(SimSys *sys, Table *table, struct attributes *attr) {

	int source;

	if(sys->is_popable() == false) {
	
		print_delay("Could not pop Stack. Stack is empty", PR_DELAY, false);
		return;
	}
	
	source = sys->pop_stack();
	sys->write_reg(attr->dest, source);
}

void jmp(SimSys *sys, Table *table, struct attributes *attr) {

	int label_ln = table->get_label_ln(attr->label);

	if(label_ln < 0) {

		print_delay("Could not find label.", PR_DELAY, false);
		return;
	}

	table->set_instr(label_ln);
}

void nop(SimSys *sys, Table *table, struct attributes *attr) {

	return;
}

void jle(SimSys *sys, Table *table, struct attributes *attr) {

	/* Work in Progress */
}

void sub(SimSys *sys, Table *table, struct attributes *attr) {

	/* Work in Progress */
}

void cmp(SimSys *sys, Table *table, struct attributes *attr) {

	/* toDo: cover all flags */

	int destination, source, diff;

	if((destination = to_dec(attr->dest)) == -1)
		destination = sys->read_reg(attr->dest);			

	if((source = to_dec(attr->src)) == -1)
		source = sys->read_reg(attr->src);

	diff = destination - source;

	if(diff == 0) {

		sys->set_flag(ZF, true);
		sys->set_flag(CF, false);
		return;
	}

	if(diff > 0) {

		sys->set_flag(ZF, false);
		sys->set_flag(CF, false);
		return;
	}

	sys->set_flag(ZF, false);
	sys->set_flag(CF, true);
}

void je(SimSys *sys, Table *table, struct attributes *attr) {

	if(sys->read_flag(ZF) == false)
		return;

	sys->set_flag(ZF, false);
	jmp(sys, table, attr);
}

void interrupt(SimSys *sys, Table *table, struct attributes *attr) {

	switch(attr->int_id) {

		case 0x80: call_kernel(sys); break;
		
		default: /* currently only supporting 0x80 */ break;
	}
}

void (*instructions[INSTR_MAX]) (SimSys *sys, Table *table, struct attributes *attr) =
{ skip, mov, push, pop, jmp, nop, jle, sub, cmp, je, interrupt };




