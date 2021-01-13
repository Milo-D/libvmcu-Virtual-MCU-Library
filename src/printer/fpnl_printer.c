/* FPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/fpnl_printer.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "collections/sstream.h"

void print_fpnl(debugwindow_t *window, dbg_t *dbg) {
    
    report_t *report = dbg->report;
    table_t *table = dbg->table;
    system_t *sys = dbg->sys;
    
    dwin_add(window, FPNL, "Flash:\n\n", D);

    const int pc = sys_get_pc(sys);
    plain_t *disassembly = report->disassembly;

    int k;

    for(k = 0; k < report->progsize; k++) {

        if(disassembly[k].addr == pc)
            break;
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = (k - 4); i <= (k + 4); i++) {

        if(i < 0 || i > report->progsize - 1) {

            dwin_add(window, FPNL, "\n", D);
            continue;
        }

        const int addr = disassembly[i].addr;

        if(addr >= 0) {

            sstream_put04x(&ss, addr);
            
            dwin_add(window, FPNL, ss.str, D);
            sstream_flush(&ss);

        } else {

            dwin_add(window, FPNL, "      ", D);
        }

        if(i == k) {

            dwin_add(window, FPNL, " [->] ", B);

        } else if(addr < 0) {

            dwin_add(window, FPNL, "      ", D);

        } else if(table_on_breakp(table, addr)) {

            dwin_add(window, FPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, FPNL, "      ", D);
        }

        dwin_highlight(window, FPNL, disassembly[i].mnem);
        dwin_add(window, FPNL, "\n", D);
    }
}
