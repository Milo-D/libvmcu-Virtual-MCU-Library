/* Implementation of Debugger Functions */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

// Project Headers
#include "debugger.hpp"
#include "ehandling.hpp"
#include "menus.hpp"
#include "table.hpp"
#include "simsys.hpp"
#include "asmparser.hpp"
#include "attributes.hpp"
#include "instructions.hpp"

#define HEAP 1024
#define STACK 1024
#define PR_DELAY 1
#define ROW 8

#define mover(seg, offs, range) *seg = ((*seg) + offs) % range

using namespace std;

namespace {

	void next(SimSys *sys, Table *table) {

		struct attributes attr;
		int current = table->get_instr();

		if(sys->terminated == true)
			return;

		if(parse_ln(table, &attr, current) == false) {
		
			print_delay("Instruction invalid.", PR_DELAY, false);
			return;
		}

		(*instructions[attr.instr])(sys, table, &attr);

		/* Double Checking in case of sys_exit */
		if(sys->terminated == true)
			return;
		
		table->step();					
	}
};

void debug(Table *table) {

	SimSys sys(HEAP, STACK);

	/* toDo: interpret while jumping */
	if(table->size > 0)
		table->jump_break();

	int reg_seg = 0;
	string last_select, select;
	
	do {

		table->refresh();
		debug_menu(&sys, table, reg_seg, ROW);

		cout << ">>> ";
		getline(cin, select);

		if(table->size <= 0)
			continue;
		
		if(select != "")
			last_select = select;		 
		
		if(last_select == "n")
			next(&sys, table);

		if(last_select == "rn")
			mover(&reg_seg, +1, 4);
		
	} while(select != "e");

	table->set_instr(-1);
	// system.~SimSys();
}


