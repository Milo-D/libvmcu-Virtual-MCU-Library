/* EPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/epnl_printer.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/queue.h"

void print_epnl(debugwindow_t *window, dbg_t *dbg) {
    
    system_t *sys = dbg->sys;
    dwin_add(window, EPNL, "EEPROM:\n\n", D);

    int8_t *eeprom = sys_dump_eeprom(sys);
    int16_t cursor = dwin_get_page(window, EPNL);

    queue_t *stream = queue_ctor();

    for(int i = (cursor - 4); i <= (cursor + 4); i++) {

        if(i < 0 || i >= EEPROM_SIZE) {

            dwin_add(window, EPNL, "\n", D);
            continue;
        }

        char vstr[3];
        to_hex(eeprom[i], vstr);

        char *addr = itoh(i);

        char *left = strfill('0', strlen(addr), 4);
        queue_put(stream, 4, "0x", left, addr, "      ");

        char *addr_out = queue_str(stream);
        dwin_add(window, EPNL, addr_out, D);
        
        queue_flush(stream);

        char *right = strfill('0', strlen(vstr), 2);
        queue_put(stream, 4, "0x", right, vstr, "\n");
        
        char *val_out = queue_str(stream);
        dwin_add(window, EPNL, val_out, D);
        
        queue_flush(stream);
        nfree(5, addr, left, addr_out, right, val_out);
    }

    queue_dtor(stream);
}
