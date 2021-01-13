/* SPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/spnl_printer.h"
#include "printer/propcol.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "collections/array.h"
#include "collections/sstream.h"

#define GAP 13

void print_spnl(debugwindow_t *window, dbg_t *dbg) {

    dwin_add(window, SPNL, "Status-Register:\n\n", D);
    array_t *coi = array_ctor(SREG_SIZE, NULL, NULL);
    
    system_t *sys = dbg->sys;
    sys_sreg_coi(sys, coi);

    const uint8_t status = sys_dump_sreg(sys);

    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = 0; i < SREG_SIZE; i++) {

        if(i == SREG_SIZE / 2)
            dwin_add(window, SPNL, "\n", D);

        sstream_put(&ss, "%s:  ", flags[i]);

        dwin_add(window, SPNL, ss.str, D);
        sstream_flush(&ss);

        const MEMPROP prop = *((MEMPROP*) array_at(coi, i));
        const COLOR col = propcol(prop);

        const uint8_t bit = (status >> i) & 0x01;
        
        sstream_put(&ss, "0x0%d", bit);
        sstream_pad(&ss, GAP);
        
        dwin_add(window, SPNL, ss.str, col);
        sstream_flush(&ss);
    }

    dwin_add(window, SPNL, "\n", D);
    array_dtor(coi);
}
