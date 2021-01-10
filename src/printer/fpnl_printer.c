/* FPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/fpnl_printer.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/queue.h"

void print_fpnl(debugwindow_t *window, dbg_t *dbg) {
    
    report_t *report = dbg->report;
    table_t *table = dbg->table;
    system_t *sys = dbg->sys;
    
    dwin_add(window, FPNL, "Flash:\n\n", D);

    const int pc = sys_get_pc(sys);

    plain_t *disassembly = report->disassembly;
    queue_t *stream = queue_ctor();

    int k;

    for(k = 0; k < report->progsize; k++) {

        if(disassembly[k].addr == pc)
            break;
    }

    for(int i = (k - 4); i <= (k + 4); i++) {

        if(i < 0 || i > report->progsize - 1) {

            dwin_add(window, FPNL, "\n", D);
            continue;
        }
        
        const int address = disassembly[i].addr;

        if(address >= 0) {

            char *addr = itoh(address);

            char *fill = strfill('0', strlen(addr), 4);
            queue_put(stream, 3, "0x", fill, addr);

            char *out = queue_str(stream);
            dwin_add(window, FPNL, out, D);

            nfree(3, addr, fill, out);

        } else {

            dwin_add(window, FPNL, "      ", D);
        }

        if(i == k) {

            dwin_add(window, FPNL, " [->] ", B);

        } else if(address < 0) {

            dwin_add(window, FPNL, "      ", D);

        } else if(table_on_breakp(table, address)) {

            dwin_add(window, FPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, FPNL, "      ", D);
        }

        dwin_highlight(window, FPNL, disassembly[i].mnem);
        dwin_add(window, FPNL, "\n", D);

        queue_flush(stream);
    }

    queue_dtor(stream);
}
