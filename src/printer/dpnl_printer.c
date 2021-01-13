/* DPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/dpnl_printer.h"
#include "printer/propcol.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "collections/tuple.h"
#include "collections/sstream.h"

#define GAP 6

void print_dpnl(debugwindow_t *window, dbg_t *dbg) {

    dwin_add(window, DPNL, "Data Memory:\n\n", D);
    tuple_t *coi = tuple_ctor(2, UINT16, INT);
    
    system_t *sys = dbg->sys;
    sys_data_coi(sys, coi);

    uint8_t *data = sys_dump_data(sys);
    int16_t cursor = dwin_get_page(window, DPNL);

    const uint16_t sp = ((data[SPH] << 8) + data[SPL]);

    if(*((MEMPROP*) tuple_get(coi, 1)) != NONE) {

        cursor = *((int16_t*) tuple_get(coi, 0));
        dwin_set_page(window, DPNL, cursor);
    }

    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = (cursor - 4); i <= (cursor + 4); i++) {

        int isp = D; COLOR ism = D;

        if(i < 0 || i > RAM_END) {

            dwin_add(window, DPNL, "\n", D);
            continue;
        }

        if(i == sp)
            isp = B;

        if(i == *((int16_t*) tuple_get(coi, 0)))
            ism = propcol(*((MEMPROP*) tuple_get(coi, 1)));

        sstream_put04x(&ss, i);
        sstream_pad(&ss, GAP);

        dwin_add(window, DPNL, ss.str, isp);
        sstream_flush(&ss);

        sstream_put02x(&ss, data[i]);
        
        dwin_add(window, DPNL, ss.str, ism);
        sstream_flush(&ss);

        if(i >= SFR_START && i <= SFR_END) {

            dwin_add(window, DPNL, "      @", Y);
            dwin_add(window, DPNL, sfreg[i - GPR_SIZE], D);
        }
        
        dwin_add(window, DPNL, "\n", D);
    }

    tuple_dtor(coi);
}
