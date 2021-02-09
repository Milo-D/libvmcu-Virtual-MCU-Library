/* FPNL Printer Implementation */

// C Headers
#include <string.h>

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

static void add_address_column(debugwindow_t *window, sstream_t *ss, const int addr);
static void add_opcode_column(debugwindow_t *window, report_t *report, sstream_t *ss, const int i);

/* --- Extern --- */

void print_fpnl(debugwindow_t *window, dbg_t *dbg) {
    
    report_t *report = dbg->report;
    table_t *table = dbg->table;
    system_t *sys = dbg->sys;
    
    dwin_add(window, FPNL, "Flash:\n\n", D);

    const int pc = sys_get_pc(sys);
    plain_t *disassembly = report->disassembly;

    int k;

    for(k = 0; k < report->progsize; k++) {

        if(disassembly[k].addr == pc)
            break;
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = (k - 4); i <= (k + 4); i++) {

        if(i < 0 || i > report->progsize - 1) {

            dwin_add(window, FPNL, "\n", D);
            continue;
        }

        const int addr = disassembly[i].addr;

        add_address_column(window, &ss, addr);
        add_opcode_column(window, report, &ss, i);

        if(i == k) {

            dwin_add(window, FPNL, " [->] ", B);

        } else if(addr < 0) {

            dwin_add(window, FPNL, "      ", D);

        } else if(table_on_breakp(table, addr)) {

            dwin_add(window, FPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, FPNL, "      ", D);
        }

        dwin_highlight(window, FPNL, disassembly[i].mnem);
        dwin_add(window, FPNL, "\n", D);
    }
}

/* --- Static --- */

static void add_address_column(debugwindow_t *window, sstream_t *ss, const int addr) {
    
    if(addr >= 0) {

        sstream_put04x(ss, addr);

        dwin_add(window, FPNL, ss->str, D);
        sstream_flush(ss);

    } else {

        dwin_add(window, FPNL, "      ", D);
    }
}

static void add_opcode_column(debugwindow_t *window, report_t *report, sstream_t *ss, const int i) {
    
    if(report->disassembly[i].addr < 0) {
     
        dwin_add(window, FPNL, "               ", D);
        return;
    }

    const uint32_t opc = report->disassembly[i].opcode;
    
    const uint16_t opcl = (opc & 0x0000ffff);
    const uint16_t swpl = (opcl >> 8) | (opcl << 8);
    
    const uint16_t opch = ((opc & 0xffff0000) >> 16);
    const uint16_t swph = (opch >> 8) | (opch << 8);
    
    if(report->disassembly[i].dword == false)
        sstream_put(ss, "      .... ");
    else
        sstream_put(ss, "      %04x ", swph);
    
    dwin_add(window, FPNL, ss->str, G);
    sstream_flush(ss);
    
    sstream_put(ss, "%04x", swpl);
    
    dwin_add(window, FPNL, ss->str, Y);
    sstream_flush(ss);
}
