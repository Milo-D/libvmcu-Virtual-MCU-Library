/* SPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/spnl_printer.h"
#include "printer/propcol.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/array.h"
#include "collections/queue.h"

void print_spnl(debugwindow_t *window, dbg_t *dbg) {
    
    dwin_add(window, SPNL, "Status-Register:\n\n", D);
    array_t *coi = array_ctor(SREG_SIZE, NULL, NULL);
    
    system_t *sys = dbg->sys;
    sys_sreg_coi(sys, coi);

    const uint8_t status = sys_dump_sreg(sys);
    queue_t *stream = queue_ctor();

    for(int i = 0; i < SREG_SIZE; i++) {

        if(i == SREG_SIZE / 2)
            dwin_add(window, SPNL, "\n", D);

        const MEMPROP prop = *((MEMPROP*) array_at(coi, i));
        const COLOR col = propcol(prop);
        
        char *vstr = get_str((status >> i) & 0x01);

        dwin_add(window, SPNL, flags[i], D);
        dwin_add(window, SPNL, ":  ", D);

        char *fill = strfill(' ', 8, 21);
        queue_put(stream, 3, "0x0", vstr, fill);

        char *out = queue_str(stream);
        dwin_add(window, SPNL, out, col);

        queue_flush(stream);
        nfree(3, vstr, fill, out);
    }

    dwin_add(window, SPNL, "\n", D);

    queue_dtor(stream);
    array_dtor(coi);
}
