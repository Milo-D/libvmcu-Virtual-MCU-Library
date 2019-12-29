/* Implementation of Debugger View */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>

// Project Headers
#include "cli/debugview.hpp"
#include "misc/ehandling.hpp"
#include "parser/parser.hpp"
#include "cli/menus.hpp"
#include "cli/help.hpp"
#include "table/table.hpp"
#include "system/sys.hpp"

#define movec(cursor, offs, range) *cursor = ((*cursor) + offs) % range

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

namespace {

    void jump_forward(Sys *sys, Table *table, int cursor) {

        do {

            if(table->is_break() == true)
                break;

            debug_menu(sys, cursor);
            sys->step();

            sleep_for(milliseconds(100));

        } while(sys->is_terminated() == false);
    }
};

void debug(Table *table) {

    int cursor = 0;
    string last_select, select;

    Sys sys(table);
    Parser parser(DEBUG_CONTEXT);

    if(table->has_break() == true)
        jump_forward(&sys, table, cursor);
	
    do {

        debug_menu(&sys, cursor);
        getline(cin, select);

        if(table->size() <= 0)
            continue;
		
        if(select != "")
            last_select = select;

        switch(parser.parseln(last_select)) {

            case 0: sys.step(); break;
            case 1: movec(&cursor, +1, 4); break;
            case 2: sys.scale_data(+1); break;
            case 3: sys.scale_data(-1); break;
            case 4: jump_forward(&sys, table, cursor); break;
            case 5: sys.scale_eeprom(+1); break;
            case 6: sys.scale_eeprom(-1); break;
            case 7: table->set_tip(0); break;
            case 8: help(); break;

            default: /* Ignoring invalid Input */ break;
        } 
		
    } while(select != "e");
}


