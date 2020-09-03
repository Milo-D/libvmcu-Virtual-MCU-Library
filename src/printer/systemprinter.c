/* Systemprinter Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "printer/systemprinter.h"
#include "printer/memprop.h"
#include "system/system.h"
#include "system/mcudef.h"
#include "cli/debugwindow.h"
#include "table/table.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/array.h"
#include "collections/tuple.h"
#include "collections/queue.h"

/* Forward Declaration of static Systemprinter Functions */

static void print_gpr(debugwindow_t *window, system_t *sys);
static void print_sreg(debugwindow_t *window, system_t *sys);
static void print_data(debugwindow_t *window, system_t *sys);
static void print_eeprom(debugwindow_t *window, system_t *sys);
static void print_table(debugwindow_t *window, table_t *table);
static void print_side_table(debugwindow_t *window, table_t *table);

static int color(const int mem_prop); 

/* Extern Systemprinter Functions */

extern void system_to_win(debugwindow_t *window, system_t *sys, table_t *table) {

    dwin_clr(window);

    print_gpr(window, sys);
    print_sreg(window, sys);
    print_data(window, sys);
    print_eeprom(window, sys);

    print_table(window, table);
    print_side_table(window, table);

    dwin_update_all(window);
}

/* Static Systemprinter Functions */

static void print_gpr(debugwindow_t *window, system_t *sys) {

    dwin_add(window, GPNL, "Registers:\n\n", D);

    array_t *buffer = array_ctor(GPR_SIZE, NULL, NULL);
    sys_dump_gpr(sys, buffer);

    array_t *coi = array_ctor(GPR_SIZE, NULL, NULL);
    sys_gpr_coi(sys, coi);

    const int s = dwin_curs_of(window, GPNL) * 8;
    queue_t *stream = queue_ctor();

    for(int i = 0; i < 8; i++) {

        if(i == 4)
            dwin_add(window, GPNL, "\n", D);

        char *gprno = get_str(s + i);
        char *gprstr = strxcat("R", gprno);

        const int col = color(*((int*) array_at(coi, s + i)));
        const int8_t val = *((int8_t*) array_at(buffer, s + i));

        dwin_add(window, GPNL, gprstr, D);
        dwin_add(window, GPNL, ": ", D);

        if((s + i) < 10)
            dwin_add(window, GPNL, " ", D);

        char vstr[3];
        to_hex(val, vstr);
        
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
    array_dtor(buffer);
    array_dtor(coi);
}

static void print_sreg(debugwindow_t *window, system_t *sys) {

    dwin_add(window, SPNL, "Status-Register:\n\n", D);

    const uint8_t status = sys_dump_sreg(sys);

    array_t *coi = array_ctor(SREG_SIZE, NULL, NULL);
    sys_sreg_coi(sys, coi);

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

static void print_data(debugwindow_t *window, system_t *sys) {

    dwin_add(window, DPNL, "Data Memory:\n\n", D);

    array_t *buffer = array_ctor((RAM_END + 1), NULL, NULL);
    sys_dump_data(sys, buffer);

    tuple_t *coi = tuple_ctor(2, UINT16, INT);
    sys_data_coi(sys, coi);

    int16_t cursor = dwin_curs_of(window, DPNL);

    if(*((int*) tuple_get(coi, 1)) != NONE) {

        cursor = *((int16_t*) tuple_get(coi, 0));
        dwin_set_curs(window, DPNL, cursor);
    }

    const uint8_t spl = *((uint8_t*) array_at(buffer, SPL));
    const uint8_t sph = *((uint8_t*) array_at(buffer, SPH));

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
        to_hex(*((int8_t*) array_at(buffer, i)), vstr);

        char *addr = itoh(i);

        char *left = strfill('0', strlen(addr), 4);
        queue_put(stream, 4, "0x", left, addr, "      ");

        char *addr_out = queue_str(stream);
        dwin_add(window, DPNL, addr_out, isp);

        queue_flush(stream);

        char *right = strfill('0', strlen(vstr), 2);
        queue_put(stream, 4, "0x", right, vstr, "\n");

        char *val_out = queue_str(stream);
        dwin_add(window, DPNL, val_out, ism);

        queue_flush(stream);
        nfree(5, addr, left, addr_out, right, val_out);
    }

    queue_dtor(stream);
    tuple_dtor(coi);
    array_dtor(buffer);
}

static void print_eeprom(debugwindow_t *window, system_t *sys) {

    dwin_add(window, EPNL, "EEPROM:\n\n", D);

    array_t *buffer = array_ctor(EEPROM_SIZE, NULL, NULL);
    sys_dump_eeprom(sys, buffer);

    tuple_t *coi = tuple_ctor(2, UINT16, INT);
    sys_eeprom_coi(sys, coi);

    int16_t cursor = dwin_curs_of(window, EPNL);

    if(*((int*) tuple_get(coi, 1)) != NONE) {

        cursor = *((int16_t*) tuple_get(coi, 0));
        dwin_set_curs(window, EPNL, cursor);
    }

    queue_t *stream = queue_ctor();

    for(int i = (cursor - 4); i <= (cursor + 4); i++) {

        int ism = D;

        if(i < 0 || i >= EEPROM_SIZE) {

            dwin_add(window, EPNL, "\n", D);
            continue;
        }

        if(i == *((int16_t*) tuple_get(coi, 0)))
            ism = color(*((int*) tuple_get(coi, 1)));

        char vstr[3];
        to_hex(*((int8_t*) array_at(buffer, i)), vstr);

        char *addr = itoh(i);

        char *left = strfill('0', strlen(addr), 4);
        queue_put(stream, 4, "0x", left, addr, "      ");

        char *addr_out = queue_str(stream);
        dwin_add(window, EPNL, addr_out, D);
        
        queue_flush(stream);

        char *right = strfill('0', strlen(vstr), 2);
        queue_put(stream, 4, "0x", right, vstr, "\n");
        
        char *val_out = queue_str(stream);
        dwin_add(window, EPNL, val_out, ism);
        
        queue_flush(stream);
        nfree(5, addr, left, addr_out, right, val_out);
    }

    queue_dtor(stream);
    tuple_dtor(coi);
    array_dtor(buffer);
}

static void print_table(debugwindow_t *window, table_t *table) {

    dwin_add(window, CPNL, "Instructions:\n\n", D);

    const int tip = table_get_tip(table);
    const int size = table->size;

    array_t *content = array_ctor(size, NULL, NULL);
    table_content(table, content);

    array_t *breakp = array_ctor(size, NULL, NULL);
    table_breakp(table, breakp);

    queue_t *stream = queue_ctor();

    for(int i = (tip - 4); i <= (tip + 4); i++) {

        if(i < 0 || i > size - 1) {

            dwin_add(window, CPNL, "\n", D);
            continue;
        }

        char *addr = itoh(i);

        char *fill = strfill('0', strlen(addr), 4);
        queue_put(stream, 3, "0x", fill, addr);

        char *out = queue_str(stream);
        dwin_add(window, CPNL, out, D);

        if(i == tip) {

            dwin_add(window, CPNL, " [->] ", B);

        } else if(*((bool*) array_at(breakp, i)) == true) {

            dwin_add(window, CPNL, " [b+] ", R);

        } else {

            dwin_add(window, CPNL, "      ", D);
        }

        char *line = (char*) array_at(content, i);
        dwin_highlight(window, CPNL, line);

        dwin_add(window, CPNL, "\n", D);

        queue_flush(stream);
        nfree(3, addr, fill, out);
    }

    queue_dtor(stream);
    array_dtor(content);
    array_dtor(breakp);
}

static void print_side_table(debugwindow_t *window, table_t *table) {

    dwin_add(window, RPNL, "Source Code:\n\n", D);

    const int tip = table_get_tip(table);
    const int size = table->size;

    array_t *content = array_ctor(size, NULL, NULL);
    table_content(table, content);

    array_t *breakp = array_ctor(size, NULL, NULL);
    table_breakp(table, breakp);

    const int cursor = dwin_curs_of(window, RPNL);
    const int height = dwin_height(window, RPNL) - 4;

    const int start = (cursor * height);

    if(size == 0) {

        dwin_add(window, RPNL, "[ No Source available ]\n", D);

        array_dtor(content);
        array_dtor(breakp);
        
        return;
    }

    queue_t *stream = queue_ctor();

    for(int i = start; i < (start + height); i++) {

        char *addr = get_str(i);
        char *fill = strfill(' ', strlen(addr), 3);

        queue_put(stream, 2, fill, addr);

        if(i >= size) {

            queue_put(stream, 1, "\n");

            char *out = queue_str(stream);
            dwin_add(window, RPNL, out, D);
            
            queue_flush(stream);
            nfree(3, addr, fill, out);

            continue;
        }

        char *out = queue_str(stream);
        dwin_add(window, RPNL, out, D);

        if(i == tip) {
          
            dwin_add(window, RPNL, " [->] ", B);

        } else if(*((bool*) array_at(breakp, i)) == true) {

            dwin_add(window, RPNL, " [b+] ", R);

        } else {

            dwin_add(window, RPNL, "      ", D);
        }

        char *line = (char*) array_at(content, i);
        dwin_highlight(window, RPNL, line);

        dwin_add(window, RPNL, "\n", D);

        queue_flush(stream);
        nfree(3, addr, fill, out);
    }

    queue_dtor(stream);
    array_dtor(content);
    array_dtor(breakp);
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
