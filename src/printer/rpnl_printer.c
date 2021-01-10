/* RPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/gpnl_printer.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/queue.h"

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
    queue_t *stream = queue_ctor();

    for(int i = start; i < (start + height); i++) {
        
        if(i >= report->progsize) {

            dwin_add(window, RPNL, "\n", D);
            continue;
        }
        
        const int address = disassembly[i].addr;

        if(address >= 0) {

            char *addr = itoh(address);
            char *fill = strfill('0', strlen(addr), 4);
            
            queue_put(stream, 3, "0x", fill, addr);

            char *out = queue_str(stream);
            dwin_add(window, RPNL, out, D);

            nfree(3, addr, fill, out);

        } else {

            dwin_add(window, RPNL, "      ", D);
        }

        if(address == pc) {
          
            dwin_add(window, RPNL, " [->] ", B);

        } else if(address < 0) {

            dwin_add(window, RPNL, "      ", D);

        } else if(table_on_breakp(table, address)) {

            dwin_add(window, RPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, RPNL, "      ", D);
        }

        dwin_highlight(window, RPNL, disassembly[i].mnem);
        dwin_add(window, RPNL, "\n", D);

        queue_flush(stream);
    }

    queue_dtor(stream);
}
