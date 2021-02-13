/* RPNL Printer Implementation */

// C Headers
#include <inttypes.h>

// Project Headers (debugger)
#include "debugger/include/cli/debug/debugwindow.h"
#include "debugger/include/dbg/dbg.h"

// Project Headers (shared)
#include "shared/include/collections/sstream.h"

/* Forward Declaration of static Functions */

static void add_address_column(debugwindow_t *window, sstream_t *ss, const int32_t addr);
static void add_opcode_column(debugwindow_t *window, composed_line_t *ln, sstream_t *ss);

/* --- Extern --- */

void print_rpnl(debugwindow_t *window, dbg_t *dbg) {

    table_t *table = dbg->table;
    system_t *sys = dbg->sys;

    dwin_add(window, RPNL, "Disassembly:\n\n", D);

    const int cursor = dwin_get_page(window, RPNL);
    const int height = dwin_height(window, RPNL) - 4;

    const int pc = sys_get_pc(sys);
    const int start = (cursor * height);

    if(dbg->cdis->size == 0) {
        
        dwin_clear_panel(window, RPNL);

        dwin_add(window, RPNL, "Disassembly:\n\n", D);
        dwin_add(window, RPNL, "[ No Source available ]\n", D);
        
        return;
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = start; i < (start + height); i++) {
        
        if(i >= dbg->cdis->size) {

            dwin_add(window, RPNL, "\n", D);
            continue;
        }

        int32_t addr = dbg->cdis->line[i].addr;
        
        add_address_column(window, &ss, addr);
        add_opcode_column(window, &dbg->cdis->line[i], &ss);

        if(addr == pc) {
          
            dwin_add(window, RPNL, " [->] ", B);

        } else if(addr < 0) {

            dwin_add(window, RPNL, "      ", D);

        } else if(table_on_breakp(table, addr)) {

            dwin_add(window, RPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, RPNL, "      ", D);
        }

        dwin_highlight(window, RPNL, dbg->cdis->line[i].str);
        dwin_add(window, RPNL, "\n", D);
    }
}

/* --- Static --- */

static void add_address_column(debugwindow_t *window, sstream_t *ss, const int32_t addr) {
    
    if(addr >= 0) {

        sstream_put04x(ss, addr);

        dwin_add(window, RPNL, ss->str, D);
        sstream_flush(ss);

    } else {

        dwin_add(window, RPNL, "      ", D);
    }
}

static void add_opcode_column(debugwindow_t *window, composed_line_t *ln, sstream_t *ss) {
    
    if(ln->addr == CDIS_NO_ADDR) {
     
        dwin_add(window, RPNL, "               ", D);
        return;
    }

    const uint32_t opc = ln->opc;
    
    const uint16_t opcl = (opc & 0x0000ffff);
    const uint16_t swpl = (opcl >> 8) | (opcl << 8);
    
    const uint16_t opch = ((opc & 0xffff0000) >> 16);
    const uint16_t swph = (opch >> 8) | (opch << 8);
    
    if(swph == 0x0000)
        sstream_put(ss, "      .... ");
    else
        sstream_put(ss, "      %04x ", swph);
    
    dwin_add(window, RPNL, ss->str, G);
    sstream_flush(ss);
    
    sstream_put(ss, "%04x", swpl);
    
    dwin_add(window, RPNL, ss->str, Y);
    sstream_flush(ss);
}







