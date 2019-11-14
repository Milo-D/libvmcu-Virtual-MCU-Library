/* Implementation of Debugger Functions */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>

// Project Headers
#include "debugger.hpp"
#include "ehandling.hpp"
#include "menus.hpp"
#include "table.hpp"
#include "sys.hpp"

#define TIP_UNDEF -1
#define TIP_DEF 0

#define movec(cursor, offs, range) *cursor = ((*cursor) + offs) % range

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

namespace {

    void step_forward(Sys *sys, Table *table) {

        if(table->executable() == true)
            sys->step();

        if(table->step() < 0)
            sys->kill();
	}

    void jump_forward(Sys *sys, Table *table) {

        int line;

        do {

            line = table->get_tip();

            if(table->is_break(line) == true) {

                table->unset_break(to_string(line));
                break;
            }

            step_forward(sys, table);

        } while(sys->is_terminated() == false);
    }
}

void debug(Table *table) {

    Sys sys( table->src() );

    int cursor = 0;
    string last_select, select;
	
    table->set_tip(TIP_DEF);

    if(table->has_break() == true)
        jump_forward(&sys, table);
	
    do {

        debug_menu(&sys, table, cursor);
        getline(cin, select);

        if(table->size() <= 0)
            continue;
		
        if(select != "")
            last_select = select;		 
		
        if(last_select == "n")
            step_forward(&sys, table);

        if(last_select == "jb")
            jump_forward(&sys, table);

        if(last_select == "rn")
            movec(&cursor, +1, 4);

        if(last_select == "dn")
            sys.scale_data(+1);

        if(last_select == "dp")
            sys.scale_data(-1);
		
    } while(select != "e");

    table->set_tip(TIP_UNDEF);
}


