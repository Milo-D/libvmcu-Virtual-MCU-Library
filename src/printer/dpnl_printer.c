/* DPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/dpnl_printer.h"
#include "printer/propcol.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/tuple.h"
#include "collections/queue.h"

void print_dpnl(debugwindow_t *window, dbg_t *dbg) {

    dwin_add(window, DPNL, "Data Memory:\n\n", D);
    tuple_t *coi = tuple_ctor(2, UINT16, INT);
    
    system_t *sys = dbg->sys;
    sys_data_coi(sys, coi);

    int8_t *data = sys_dump_data(sys);
    int16_t cursor = dwin_get_page(window, DPNL);

    if(*((MEMPROP*) tuple_get(coi, 1)) != NONE) {

        cursor = *((int16_t*) tuple_get(coi, 0));
        dwin_set_page(window, DPNL, cursor);
    }

    const uint8_t spl = (uint8_t) data[SPL];
    const uint8_t sph = (uint8_t) data[SPH];

    const uint16_t sp = ((sph << 8) + spl);

    queue_t *stream = queue_ctor();

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

        char vstr[3];
        to_hex(data[i], vstr);

        char *addr = itoh(i);

        char *left = strfill('0', strlen(addr), 4);
        queue_put(stream, 4, "0x", left, addr, "      ");

        char *addr_out = queue_str(stream);
        dwin_add(window, DPNL, addr_out, isp);

        queue_flush(stream);

        char *right = strfill('0', strlen(vstr), 2);
        queue_put(stream, 3, "0x", right, vstr);

        char *val_out = queue_str(stream);
        dwin_add(window, DPNL, val_out, ism);

        if(i >= SFR_START && i <= SFR_END) {

            dwin_add(window, DPNL, "      @", Y);
            dwin_add(window, DPNL, sfreg[i - GPR_SIZE], D);
        }
        
        dwin_add(window, DPNL, "\n", D);

        queue_flush(stream);
        nfree(5, addr, left, addr_out, right, val_out);
    }

    queue_dtor(stream);
    tuple_dtor(coi);
}
