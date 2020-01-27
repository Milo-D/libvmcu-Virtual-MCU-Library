/* Implementation of Debugger View */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <signal.h>

// Project Headers
#include "cli/debugview.hpp"
#include "cli/debugwindow.hpp"
#include "cli/style.hpp"
#include "misc/stdmsg.hpp"
#include "misc/ehandling.hpp"
#include "misc/stringmanip.hpp"
#include "parser/parser.hpp"
#include "table/table.hpp"
#include "system/mcu.hpp"
#include "system/sys.hpp"

#define SIG_ON signal(SIGWINCH, sig_handler)
#define SIG_OFF signal(SIGWINCH, SIG_DFL)

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

static DebugWindow *dwin;

namespace {

    void jump_forward(Sys *sys, Table *table) {

        do {

            if(table->is_break() == true)
                break;

            sys->put_sys(dwin);
            sys->step();

            sleep_for(milliseconds(100));

        } while(sys->is_terminated() == false);
    }

    void examine_data(Sys *sys, string mem_cell) {

        int cell = hex_to_dec(mem_cell);

        if(cell < 0 || cell > RAM_END) {

            dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
            return;
        }

        int8_t data = sys->read_data(cell);
        dwin->write(OUTPUT_PANEL, val_of(mem_cell, get_hex(data)), DEF);
    }

    void sig_handler(int signal) {

        delete dwin;
        dwin = new DebugWindow;
    }
};

void debug(Table *table) {

    SIG_ON;
    dwin = new DebugWindow;

    Sys sys(table);
    Parser parser(DEBUG_CONTEXT);

    string last_select, select;

    dwin->write(OUTPUT_PANEL, INIT_SUCCESS, DEF);
    dwin->write(OUTPUT_PANEL, sim_start(table->src()), DEF);
	
    do {

        sys.put_sys(dwin);
        select = dwin->read_prompt();

        if(table->size() <= 0)
            continue;

        if(select != "")
            last_select = select;

        if(sys.is_terminated() == true)
            dwin->write(OUTPUT_PANEL, SYS_TERM, DEF);

        vector <string> cmd;
        split(last_select, cmd).swap(cmd); 

        switch(parser.parseln(last_select)) {

            case 0: sys.step(); break;
            case 1: sys.backstep(table); break;
            case 2: sys.scale_gpr(+1); break;
            case 3: sys.scale_gpr(-1); break;
            case 4: sys.scale_data(+1); break;
            case 5: sys.scale_data(-1); break;
            case 6: jump_forward(&sys, table); break;
            case 7: sys.scale_eeprom(+1); break;
            case 8: sys.scale_eeprom(-1); break;
            case 9: examine_data(&sys, cmd[1]); break;
            case 10: table->set_tip(0); break;
            case 11: /* help comes here */ break;
            case 12: table->set_break(cmd[1]); break;
            case 13: table->unset_break(cmd[1]); break;
            case 14: table->define(cmd[1], cmd[2]); break;
            case 15: table->next_page(+1); break;
            case 16: table->next_page(-1); break;

            default: /* ignoring invalid input */ break;
        } 
		
    } while(select != "q");

    SIG_OFF;
    delete dwin;
}


