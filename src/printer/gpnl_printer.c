/* GPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/gpnl_printer.h"
#include "printer/propcol.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/array.h"
#include "collections/queue.h"

void print_gpnl(debugwindow_t *window, dbg_t *dbg) {
    
    dwin_add(window, GPNL, "Registers:\n\n", D);
    array_t *coi = array_ctor(GPR_SIZE, NULL, NULL);
    
    system_t *sys = dbg->sys;
    sys_gpr_coi(sys, coi);

    int8_t *regfile = sys_dump_gpr(sys);

    const int s = dwin_get_page(window, GPNL) * 8;
    queue_t *stream = queue_ctor();

    for(int i = 0; i < 8; i++) {

        if(i == 4)
            dwin_add(window, GPNL, "\n", D);

        char *gprno = get_str(s + i);
        char *gprstr = strxcat("R", gprno);

        const MEMPROP prop = *((MEMPROP*) array_at(coi, s + i));
        const COLOR col = propcol(prop);

        dwin_add(window, GPNL, gprstr, D);
        dwin_add(window, GPNL, ": ", D);

        if((s + i) < 10)
            dwin_add(window, GPNL, " ", D);

        char vstr[3];
        to_hex(regfile[s + i], vstr);
        
        char *zero = strfill('0', strlen(vstr), 2);
        queue_put(stream, 3, "0x", zero, vstr);
        queue_put(stream, 1, "             ");

        char *out = queue_str(stream);
        dwin_add(window, GPNL, out, col);

        queue_flush(stream);
        nfree(4, gprno, gprstr, zero, out);
    }

    dwin_add(window, GPNL, "\n", D);

    queue_dtor(stream);
    array_dtor(coi);
}
