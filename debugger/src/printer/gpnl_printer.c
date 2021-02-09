/* GPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "debugger/include/printer/gpnl_printer.h"
#include "debugger/include/printer/propcol.h"
#include "debugger/include/cli/debug/debugwindow.h"
#include "debugger/include/dbg/dbg.h"

// Project Headers (shared)
#include "shared/include/collections/array.h"
#include "shared/include/collections/sstream.h"

#define GAP 13

void print_gpnl(debugwindow_t *window, dbg_t *dbg) {
    
    dwin_add(window, GPNL, "Registers:\n\n", D);
    array_t *coi = array_ctor(GPR_SIZE, NULL, NULL);

    system_t *sys = dbg->sys;
    sys_gpr_coi(sys, coi);

    uint8_t *regfile = sys_dump_gpr(sys);
    const int s = dwin_get_page(window, GPNL) * 8;

    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = 0; i < 8; i++) {

        if(i == 4)
            dwin_add(window, GPNL, "\n", D);

        const MEMPROP prop = *((MEMPROP*) array_at(coi, s + i));
        const COLOR col = propcol(prop);

        sstream_put(&ss, "R%d: ", (s + i));
        dwin_add(window, GPNL, ss.str, D);

        if((s + i) < 10)
            dwin_add(window, GPNL, " ", D);

        sstream_flush(&ss);
        
        sstream_put02x(&ss, regfile[s + i]);
        sstream_pad(&ss, GAP);
        
        dwin_add(window, GPNL, ss.str, col);

        sstream_flush(&ss);
    }

    dwin_add(window, GPNL, "\n", D);
    array_dtor(coi);
}
