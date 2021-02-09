/* RPNL Printer Implementation */

// C Headers
#include <string.h>

// Project Headers (debugger)
#include "debugger/include/printer/gpnl_printer.h"
#include "debugger/include/cli/debug/debugwindow.h"
#include "debugger/include/dbg/dbg.h"

// Project Headers (shared)
#include "shared/include/collections/sstream.h"

/* Forward Declaration of static Functions */

static void add_address_column(debugwindow_t *window, sstream_t *ss, const int addr);
static void add_opcode_column(debugwindow_t *window, report_t *report, sstream_t *ss, const int i);

/* --- Extern --- */

void print_rpnl(debugwindow_t *window, dbg_t *dbg) {
    
    report_t *report = dbg->report;
    table_t *table = dbg->table;
    system_t *sys = dbg->sys;

    dwin_add(window, RPNL, "Disassembly:\n\n", D);

    const int cursor = dwin_get_page(window, RPNL);
    const int height = dwin_height(window, RPNL) - 4;

    const int pc = sys_get_pc(sys);
    const int start = (cursor * height);

    if(report->progsize == 0) {
        
        dwin_clear_panel(window, RPNL);

        dwin_add(window, RPNL, "Disassembly:\n\n", D);
        dwin_add(window, RPNL, "[ No Source available ]\n", D);
        
        return;
    }

    plain_t *disassembly = report->disassembly;
    
    sstream_t ss;
    sstream_ctor(&ss);

    for(int i = start; i < (start + height); i++) {
        
        if(i >= report->progsize) {

            dwin_add(window, RPNL, "\n", D);
            continue;
        }

        const int addr = disassembly[i].addr;
        
        add_address_column(window, &ss, addr);
        add_opcode_column(window, report, &ss, i);

        if(addr == pc) {
          
            dwin_add(window, RPNL, " [->] ", B);

        } else if(addr < 0) {

            dwin_add(window, RPNL, "      ", D);

        } else if(table_on_breakp(table, addr)) {

            dwin_add(window, RPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, RPNL, "      ", D);
        }

        dwin_highlight(window, RPNL, disassembly[i].mnem);
        dwin_add(window, RPNL, "\n", D);
    }
}

/* --- Static --- */

static void add_address_column(debugwindow_t *window, sstream_t *ss, const int addr) {
    
    if(addr >= 0) {

        sstream_put04x(ss, addr);

        dwin_add(window, RPNL, ss->str, D);
        sstream_flush(ss);

    } else {

        dwin_add(window, RPNL, "      ", D);
    }
}

static void add_opcode_column(debugwindow_t *window, report_t *report, sstream_t *ss, const int i) {
    
    if(report->disassembly[i].addr < 0) {
     
        dwin_add(window, RPNL, "               ", D);
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
    
    dwin_add(window, RPNL, ss->str, G);
    sstream_flush(ss);
    
    sstream_put(ss, "%04x", swpl);
    
    dwin_add(window, RPNL, ss->str, Y);
    sstream_flush(ss);
}







