/* RPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/gpnl_printer.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "collections/sstream.h"

void print_rpnl(debugwindow_t *window, dbg_t *dbg) {
    
    report_t *report = dbg->report;
    table_t *table = dbg->table;
    system_t *sys = dbg->sys;

    dwin_add(window, RPNL, "Source Code:\n\n", D);

    const int cursor = dwin_get_page(window, RPNL);
    const int height = dwin_height(window, RPNL) - 4;

    const int pc = sys_get_pc(sys);
    const int start = (cursor * height);

    if(report->progsize == 0) {
        
        dwin_clear_panel(window, RPNL);

        dwin_add(window, RPNL, "Source Code:\n\n", D);
        dwin_add(window, RPNL, "[ No Source available ]\n", D);
        
        return;
    }

    plain_t *disassembly = report->disassembly;
    
    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = start; i < (start + height); i++) {
        
        if(i >= report->progsize) {

            dwin_add(window, RPNL, "\n", D);
            continue;
        }
        
        const int addr = disassembly[i].addr;

        if(addr >= 0) {

            sstream_put04x(&ss, addr);

            dwin_add(window, RPNL, ss.str, D);
            sstream_flush(&ss);

        } else {

            dwin_add(window, RPNL, "      ", D);
        }

        if(addr == pc) {
          
            dwin_add(window, RPNL, " [->] ", B);

        } else if(addr < 0) {

            dwin_add(window, RPNL, "      ", D);

        } else if(table_on_breakp(table, addr)) {

            dwin_add(window, RPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, RPNL, "      ", D);
        }

        dwin_highlight(window, RPNL, disassembly[i].mnem);
        dwin_add(window, RPNL, "\n", D);
    }
}
