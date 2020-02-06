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
#include "system/mcu.hpp"
#include "system/sys.hpp"
#include "table/table.hpp"
#include "parser/parser.hpp"
#include "printer/systemprinter.hpp"
#include "misc/stdmsg.hpp"
#include "misc/ehandling.hpp"
#include "misc/stringmanip.hpp"

#define SIG_ON signal(SIGWINCH, sig_handler)
#define SIG_OFF signal(SIGWINCH, SIG_DFL)

using namespace std;

static int table_size;                              // needed during interrupt
static DebugWindow *dwin;                           // needed during interrupt

static void sig_handler(int signal) {

    delete dwin;
    dwin = new DebugWindow(table_size);
}

void debug(Table *table) {

    SIG_ON;

    table_size = table->size();
    dwin = new DebugWindow(table_size);

    Sys sys(table);
    Parser parser(DEBUG_CONTEXT);

    string last_select, select;

    dwin->write(OUTPUT_PANEL, INIT_SUCCESS, DEF);
    dwin->write(OUTPUT_PANEL, sim_start(table->src()), DEF);
	
    do {

        system_to_win(dwin, &sys, table);
        select = dwin->read_prompt();

        if(table_size <= 0)
            continue;

        if(select != "")
            last_select = select;

        if(sys.is_terminated() == true)
            dwin->write(OUTPUT_PANEL, SYS_TERM, DEF);

        vector <string> cmd;
        split(last_select, cmd).swap(cmd);

        switch(parser.parseln(last_select)) {

            case 0: sys.step();                                         break;
            case 1: sys.backstep(table);                                break;
            case 2: dwin->move_cursor(GPR_PANEL, +1);                   break;
            case 3: dwin->move_cursor(GPR_PANEL, -1);                   break;
            case 4: dwin->move_cursor(DATA_PANEL, +1);                  break;
            case 5: dwin->move_cursor(DATA_PANEL, -1);                  break;
            case 6: jump_forward(dwin, &sys, table, to_dec(cmd[1]));    break;
            case 7: dwin->move_cursor(EEPROM_PANEL, +1);                break;
            case 8: dwin->move_cursor(EEPROM_PANEL, -1);                break;
            case 9: examine_data(dwin, &sys, cmd[1]);                   break;
            case 10: examine_eeprom(dwin, &sys, cmd[1]);                break;
            case 11: examine_eeprom_char(dwin, &sys, cmd[1], cmd[2]);   break;
            case 12: load_eep_hex(dwin, &sys, cmd[1]);                  break;
            case 13: clear_output(dwin);                                break;
            case 14: table->set_tip(0);                                 break;
            case 15: /* here comes help output*/                        break;
            case 16: set_breakpoint(dwin, table, cmd[1]);               break;
            case 17: remove_breakpoint(dwin, table, cmd[1]);            break;
            case 18: table->define(cmd[1], cmd[2]);                     break;
            case 19: dwin->move_cursor(SIDE_PANEL, +1);                 break;
            case 20: dwin->move_cursor(SIDE_PANEL, -1);                 break;

            default: /* ignoring invalid input */                       break;
        } 
		
    } while(select != "q");

    SIG_OFF;
    delete dwin;
}


