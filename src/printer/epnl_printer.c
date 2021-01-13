/* EPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers
#include "printer/epnl_printer.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "collections/sstream.h"

#define GAP 6

void print_epnl(debugwindow_t *window, dbg_t *dbg) {
    
    system_t *sys = dbg->sys;
    dwin_add(window, EPNL, "EEPROM:\n\n", D);

    uint8_t *eeprom = sys_dump_eeprom(sys);
    int16_t cursor = dwin_get_page(window, EPNL);

    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = (cursor - 4); i <= (cursor + 4); i++) {

        if(i < 0 || i >= EEPROM_SIZE) {

            dwin_add(window, EPNL, "\n", D);
            continue;
        }

        sstream_put04x(&ss, i);
        sstream_pad(&ss, GAP);

        dwin_add(window, EPNL, ss.str, D);
        sstream_flush(&ss);

        sstream_put(&ss, "0x%02x\n", eeprom[i]);
        
        dwin_add(window, EPNL, ss.str, D);
        sstream_flush(&ss);
    }
}
