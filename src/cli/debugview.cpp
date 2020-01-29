/* Implementation of Debugger View */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <signal.h>

// Project Headers
#include "cli/debugview.hpp"
#include "cli/debugwindow.hpp"
#include "cli/debugcommands.hpp"
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

static DebugWindow *dwin;

static void sig_handler(int signal) {

    delete dwin;
    dwin = new DebugWindow;
}

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
            case 6: jump_forward(dwin, &sys, table); break;
            case 7: sys.scale_eeprom(+1); break;
            case 8: sys.scale_eeprom(-1); break;
            case 9: examine_data(dwin, &sys, cmd[1]); break;
            case 10: examine_eeprom(dwin, &sys, cmd[1]); break;
            case 11: table->set_tip(0); break;
            case 12: /* here comes help output*/break;
            case 13: table->set_break(cmd[1]); break;
            case 14: table->unset_break(cmd[1]); break;
            case 15: table->define(cmd[1], cmd[2]); break;
            case 16: table->next_page(+1); break;
            case 17: table->next_page(-1); break;

            default: /* ignoring invalid input */ break;
        } 
		
    } while(select != "q");

    SIG_OFF;
    delete dwin;
}


