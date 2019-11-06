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
#include "sys.hpp"

#define TIP_UNDEF -1

#define movec(cursor, offs, range) *cursor = ((*cursor) + offs) % range

using namespace std;

namespace {
	
    void step_forward(Sys *sys, Table *table) {

        if(table->executable() == true)
            sys->step();

        if(table->step() < 0)
            sys->kill();
	}
}

void debug(Table *table) {

    Sys sys( table->src() );
	
    if(table->size() > 0)
        table->jump_break();

    int cursor = 0;
    string last_select, select;
	
    do {

        debug_menu(&sys, table, cursor);
        getline(cin, select);

        if(table->size() <= 0)
            continue;
		
        if(select != "")
            last_select = select;		 
		
        if(last_select == "n")          // next instruction
            step_forward(&sys, table);

        if(last_select == "rn")         // next register site
            movec(&cursor, +1, 4);
		
    } while(select != "e");

    table->set_tip(TIP_UNDEF);
}


