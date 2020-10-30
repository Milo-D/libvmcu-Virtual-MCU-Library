/* Implementation of DebugState */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// Project Headers
#include "state/debug/debugstate.h"
#include "state/debug/debugcommands.h"
#include "state/debug/stdmsg.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "system/system.h"
#include "parser/parser.h"
#include "printer/systemprinter.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/list.h"

#define at(l, i) ls_at(l, i)

static debugwindow_t *window = NULL;

/* Forward Declarations of static Debug Functions */

static void sig_handler(int signal);

/* --- Public --- */

void debug(system_t *sys, const char *file) {

    const int size = sys_table_size(sys);

    window = dwin_ctor(size);
    parser_t *parser = parser_ctor(DEBUG);

    signal(SIGWINCH, sig_handler);
    char *sim_status = sim_start(file);

    dwin_write(window, OPNL, INIT_SUCCESS, D);
    dwin_write(window, OPNL, sim_status, D);
	
    char select[64] = "";
    char last_select[64] = "";

    do {

        system_to_win(window, sys);
        dwin_read_prompt(window, select);

        if(size <= 0)
            continue;

        if(strcmp(select, "") != 0)
            strncpy(last_select, select, 64);

        list_t *com = ls_ctor(NULL, NULL);
        split(last_select, ' ', com);

        switch(parser_eval(parser, last_select)) {

            case 0:

                if(sys_step(sys) < 0)
                    dwin_write(window, OPNL, ILLEGAL_OPC, D);                                      

            break;

            case 1: sys_backstep(sys);                                            break;
            case 2: dwin_mv_curs(window, GPNL, +1);                               break;
            case 3: dwin_mv_curs(window, GPNL, -1);                               break;
            case 4: dwin_mv_curs(window, DPNL, +1);                               break;
            case 5: dwin_mv_curs(window, DPNL, -1);                               break;
            case 6: jump_forward(window, sys, get_int(at(com, 1)));               break;
            case 7: dwin_mv_curs(window, EPNL, +1);                               break;
            case 8: dwin_mv_curs(window, EPNL, -1);                               break;
            case 9: examine_data(window, sys, at(com, 1));                        break;
            case 10: examine_eeprom(window, sys, at(com, 1));                     break;
            case 11: examine_data_char(window, sys, at(com, 1), at(com, 2));      break;
            case 12: examine_eeprom_char(window, sys, at(com, 1), at(com, 2));    break;
            case 13: load_eep_hex(window, sys, at(com, 1));                       break;
            case 14: clear_output(window);                                        break;
            case 15: /* debug exit */                                             break;
            case 16: /* here comes help output*/                                  break;
            case 17: set_breakpoint(window, sys, at(com, 1));                     break;
            case 18: remove_breakpoint(window, sys, at(com, 1));                  break;
            case 19: dwin_write(window, OPNL, NOT_AVAIL, R);                      break;
            case 20: dwin_mv_curs(window, RPNL, +1);                              break;
            case 21: dwin_mv_curs(window, RPNL, -1);                              break;
            case 22: show_cycles(window, sys);                                    break;
            case 23: show_clock(window, sys);                                     break;
            case 24: show_time(window, sys);                                      break;
            case 25: examine_data_byte(window, sys, at(com, 1));                  break;

            default: /* ignoring invalid input */                                 break;
        }

        ls_dtor(com);
		
    } while(strcmp(select, "q") != 0);

    signal(SIGWINCH, SIG_DFL);

    free(sim_status);
    dwin_dtor(window);
    parser_dtor(parser);
}

/* --- Private --- */

static void sig_handler(int signal) {

    if(window == NULL)
        return;

    dwin_resize(window);
}


