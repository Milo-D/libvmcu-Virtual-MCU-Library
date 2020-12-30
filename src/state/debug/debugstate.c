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

void debug(dbg_t *dbg, const char *file) {

    const int size = dbg->report->progsize;

    window = dwin_ctor(size);
    parser_t *parser = parser_ctor(DEBUG);

    signal(SIGWINCH, sig_handler);
    char *sim_status = sim_start(file);

    dwin_write(window, OPNL, INIT_SUCCESS, D);
    dwin_write(window, OPNL, sim_status, D);
	
    char select[64] = "";
    char last_select[64] = "";

    do {

        system_to_win(window, dbg);
        dwin_read_prompt(window, select);

        if(size <= 0)
            continue;

        if(strcmp(select, "") != 0)
            strncpy(last_select, select, 64);

        list_t *com = ls_ctor(NULL, NULL);
        split(last_select, ' ', com);

        switch(parser_eval(parser, last_select)) {

            case 0:  command_n(window, dbg);                                break;
            case 1:  command_b(window, dbg);                                break;
            case 2:  command_rn(window);                                    break;
            case 3:  command_rp(window);                                    break;
            case 4:  command_dn(window);                                    break;
            case 5:  command_dp(window);                                    break;
            case 6:  command_jb(window, dbg, get_int(at(com, 1)));          break;
            case 7:  command_en(window);                                    break;
            case 8:  command_ep(window);                                    break;
            case 9:  command_xd(window, dbg, at(com, 1));                   break;
            case 10: command_xe(window, dbg, at(com, 1));                   break;
            case 11: command_xdc(window, dbg, at(com, 1), at(com, 2));      break;
            case 12: command_xec(window, dbg, at(com, 1), at(com, 2));      break;
            case 13: command_leep(window, dbg, at(com, 1));                 break;
            case 14: command_clear(window);                                 break;
            case 15: /* debug exit */                                       break;
            case 16: /* here comes help output*/                            break;
            case 17: command_break(window, dbg, at(com, 1));                break;
            case 18: command_unbreak(window, dbg, at(com, 1));              break;
            case 19: command_def(window);                                   break;
            case 20: command_pn(window);                                    break;
            case 21: command_pp(window);                                    break;
            case 22: command_cycles(window, dbg);                           break;
            case 23: command_clock(window, dbg);                            break;
            case 24: command_time(window, dbg);                             break;
            case 25: command_xdb(window, dbg, at(com, 1));                  break;
            case 26: command_jc(window, dbg, get_int(at(com, 1)));          break;
            case 27: command_cc(window, dbg, at(com, 1), at(com, 2));       break;

            default: /* ignoring invalid input */                           break;
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


