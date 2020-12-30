/* Systemprinter Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "printer/systemprinter.h"
#include "printer/memprop.h"
#include "cli/debug/debugwindow.h"
#include "system/mcudef.h"
#include "dbg/dbg.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/array.h"
#include "collections/tuple.h"
#include "collections/queue.h"

/* Forward Declaration of static Systemprinter Functions */

static void print_gpr(debugwindow_t *window, dbg_t *dbg);
static void print_sreg(debugwindow_t *window, dbg_t *dbg);
static void print_data(debugwindow_t *window, dbg_t *dbg);
static void print_eeprom(debugwindow_t *window, dbg_t *dbg);
static void print_flash(debugwindow_t *window, dbg_t *dbg);
static void print_side(debugwindow_t *window, dbg_t *dbg);

static int color(const int mem_prop); 

/* Extern Systemprinter Functions */

extern void system_to_win(debugwindow_t *window, dbg_t *dbg) {

    print_gpr(window, dbg);
    print_sreg(window, dbg);
    print_data(window, dbg);
    print_eeprom(window, dbg);

    print_flash(window, dbg);
    print_side(window, dbg);

    dwin_update_all(window);
}

/* Static Systemprinter Functions */

static void print_gpr(debugwindow_t *window, dbg_t *dbg) {

    array_t *coi = array_ctor(GPR_SIZE, NULL, NULL);
    dwin_add(window, GPNL, "Registers:\n\n", D);
    
    system_t *sys = dbg->sys;
    sys_gpr_coi(sys, coi);

    int8_t *regfile = sys_dump_gpr(sys);

    const int s = dwin_get_page(window, GPNL) * 8;
    queue_t *stream = queue_ctor();

    for(int i = 0; i < 8; i++) {

        if(i == 4)
            dwin_add(window, GPNL, "\n", D);

        char *gprno = get_str(s + i);
        char *gprstr = strxcat("R", gprno);

        const int col = color(*((int*) array_at(coi, s + i)));

        dwin_add(window, GPNL, gprstr, D);
        dwin_add(window, GPNL, ": ", D);

        if((s + i) < 10)
            dwin_add(window, GPNL, " ", D);

        char vstr[3];
        to_hex(regfile[s + i], vstr);
        
        char *zero = strfill('0', strlen(vstr), 2);
        queue_put(stream, 3, "0x", zero, vstr);
        queue_put(stream, 1, "             ");

        char *out = queue_str(stream);
        dwin_add(window, GPNL, out, col);

        queue_flush(stream);
        nfree(4, gprno, gprstr, zero, out);
    }

    dwin_add(window, GPNL, "\n", D);

    queue_dtor(stream);
    array_dtor(coi);
}

static void print_sreg(debugwindow_t *window, dbg_t *dbg) {

    array_t *coi = array_ctor(SREG_SIZE, NULL, NULL);
    dwin_add(window, SPNL, "Status-Register:\n\n", D);
    
    system_t *sys = dbg->sys;
    sys_sreg_coi(sys, coi);

    const uint8_t status = sys_dump_sreg(sys);
    queue_t *stream = queue_ctor();

    for(int i = 0; i < SREG_SIZE; i++) {

        if(i == SREG_SIZE / 2)
            dwin_add(window, SPNL, "\n", D);

        const int col = color(*((int*) array_at(coi, i)));
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

static void print_data(debugwindow_t *window, dbg_t *dbg) {

    tuple_t *coi = tuple_ctor(2, UINT16, INT);
    dwin_add(window, DPNL, "Data Memory:\n\n", D);
    
    system_t *sys = dbg->sys;
    sys_data_coi(sys, coi);

    int8_t *data = sys_dump_data(sys);
    int16_t cursor = dwin_get_page(window, DPNL);

    if(*((int*) tuple_get(coi, 1)) != NONE) {

        cursor = *((int16_t*) tuple_get(coi, 0));
        dwin_set_page(window, DPNL, cursor);
    }

    const uint8_t spl = (uint8_t) data[SPL];
    const uint8_t sph = (uint8_t) data[SPH];

    const uint16_t sp = ((sph << 8) + spl);

    queue_t *stream = queue_ctor();

    for(int i = (cursor - 4); i <= (cursor + 4); i++) {

        int isp = D; int ism = D;

        if(i < 0 || i > RAM_END) {

            dwin_add(window, DPNL, "\n", D);
            continue;
        }

        if(i == sp)
            isp = B;

        if(i == *((int16_t*) tuple_get(coi, 0)))
            ism = color(*((int*) tuple_get(coi, 1)));

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

static void print_eeprom(debugwindow_t *window, dbg_t *dbg) {

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

static void print_flash(debugwindow_t *window, dbg_t *dbg) {

    report_t *report = dbg->report;
    table_t *table = dbg->table;
    system_t *sys = dbg->sys;
    
    dwin_add(window, FPNL, "Flash:\n\n", D);

    const int pc = sys_get_pc(sys);

    plain_t *disassembly = report->disassembly;
    queue_t *stream = queue_ctor();

    int k;

    for(k = 0; k < report->progsize; k++) {

        if(disassembly[k].addr == pc)
            break;
    }

    for(int i = (k - 4); i <= (k + 4); i++) {

        if(i < 0 || i > report->progsize - 1) {

            dwin_add(window, FPNL, "\n", D);
            continue;
        }
        
        const int address = disassembly[i].addr;

        if(address >= 0) {

            char *addr = itoh(address);

            char *fill = strfill('0', strlen(addr), 4);
            queue_put(stream, 3, "0x", fill, addr);

            char *out = queue_str(stream);
            dwin_add(window, FPNL, out, D);

            nfree(3, addr, fill, out);

        } else {

            dwin_add(window, FPNL, "      ", D);
        }

        if(i == k) {

            dwin_add(window, FPNL, " [->] ", B);

        } else if(address < 0) {

            dwin_add(window, FPNL, "      ", D);

        } else if(table_on_breakp(table, address)) {

            dwin_add(window, FPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, FPNL, "      ", D);
        }

        dwin_highlight(window, FPNL, disassembly[i].mnem);
        dwin_add(window, FPNL, "\n", D);

        queue_flush(stream);
    }

    queue_dtor(stream);
}

static void print_side(debugwindow_t *window, dbg_t *dbg) {

    report_t *report = dbg->report;
    table_t *table = dbg->table;
    system_t *sys = dbg->sys;

    dwin_add(window, RPNL, "Source Code:\n\n", D);

    const int cursor = dwin_get_page(window, RPNL);
    const int height = dwin_height(window, RPNL) - 4;

    const int pc = sys_get_pc(sys);
    const int start = (cursor * height);

    if(report->progsize == 0) {

        dwin_clear_panel(window, RPNL);

        dwin_add(window, RPNL, "Source Code:\n\n", D);
        dwin_add(window, RPNL, "[ No Source available ]\n", D); 

        return;
    }

    plain_t *disassembly = report->disassembly;
    queue_t *stream = queue_ctor();

    for(int i = start; i < (start + height); i++) {
        
        if(i >= report->progsize) {

            dwin_add(window, RPNL, "\n", D);
            continue;
        }
        
        const int address = disassembly[i].addr;

        if(address >= 0) {

            char *addr = itoh(address);
            char *fill = strfill('0', strlen(addr), 4);
            
            queue_put(stream, 3, "0x", fill, addr);

            char *out = queue_str(stream);
            dwin_add(window, RPNL, out, D);

            nfree(3, addr, fill, out);

        } else {

            dwin_add(window, RPNL, "      ", D);
        }

        if(address == pc) {
          
            dwin_add(window, RPNL, " [->] ", B);

        } else if(address < 0) {

            dwin_add(window, RPNL, "      ", D);

        } else if(table_on_breakp(table, address)) {

            dwin_add(window, RPNL, " [b+] ", R);
            
        } else {
            
            dwin_add(window, RPNL, "      ", D);
        }

        dwin_highlight(window, RPNL, disassembly[i].mnem);
        dwin_add(window, RPNL, "\n", D);

        queue_flush(stream);
    }

    queue_dtor(stream);
}

static int color(const int mem_prop) {

    int color;

    switch(mem_prop) {

        case SRC: color = R; break;
        case DEST: color = G; break;

        default: color = D; break;
    }

    return color;
}
