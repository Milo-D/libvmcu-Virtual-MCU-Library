/* FPNL Printer Implementation */

// C Headers
#include <inttypes.h>

// Project Headers (debugger)
#include "debugger/include/printer/fpnl_printer.h"
#include "debugger/include/cli/debug/debugwindow.h"
#include "debugger/include/dbg/dbg.h"

// Project Headers (shared)
#include "shared/include/collections/sstream.h"

/*
 * This file contains duplication of code, because of
 * the fact that, both, FPNL and RPNL are working on the
 * same task: printing the disassembly. While RPNL shows
 * the whole disassembly, FPNL shows disassembly based on PC's
 * position.
 * 
 * Duplicated functions will be removed as soon as possible.
 * 
 * */

/* Forward Declaration of static Functions */

static void add_address_column(debugwindow_t *window, sstream_t *ss, const int32_t addr);
static void add_opcode_column(debugwindow_t *window, composed_line_t *ln, sstream_t *ss);

/* --- Extern --- */

void print_fpnl(debugwindow_t *window, dbg_t *dbg) {

    table_t *table = dbg->table;
    system_t *sys = dbg->sys;
    
    dwin_add(window, FPNL, "Flash:\n\n", D);
    const int pc = sys_get_pc(sys); int k;

    for(k = 0; k < dbg->cdis->size; k++) {

        if(dbg->cdis->line[k].addr == pc)
            break;
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = (k - 4); i <= (k + 4); i++) {

        if(i < 0 || i > dbg->cdis->size - 1) {

            dwin_add(window, FPNL, "\n", D);
            continue;
        }

        int32_t addr = dbg->cdis->line[i].addr;

        add_address_column(window, &ss, addr);
        add_opcode_column(window, &dbg->cdis->line[i], &ss);

        if(i == k) {

            dwin_add(window, FPNL, " [->] ", B);

        } else if(addr < 0) {

            dwin_add(window, FPNL, "      ", D);

        } else if(table_on_breakp(table, addr)) {

            dwin_add(window, FPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, FPNL, "      ", D);
        }

        dwin_highlight(window, FPNL, dbg->cdis->line[i].str);
        dwin_add(window, FPNL, "\n", D);
    }
}

/* --- Static --- */

static void add_address_column(debugwindow_t *window, sstream_t *ss, const int32_t addr) {
    
    if(addr >= 0) {

        sstream_put04x(ss, addr);

        dwin_add(window, FPNL, ss->str, D);
        sstream_flush(ss);

    } else {

        dwin_add(window, FPNL, "      ", D);
    }
}

static void add_opcode_column(debugwindow_t *window, composed_line_t *ln, sstream_t *ss) {

    if(ln->addr == CDIS_NO_ADDR) {

        dwin_add(window, FPNL, "               ", D);
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

    dwin_add(window, FPNL, ss->str, G);
    sstream_flush(ss);

    sstream_put(ss, "%04x", swpl);

    dwin_add(window, FPNL, ss->str, Y);
    sstream_flush(ss);
}
