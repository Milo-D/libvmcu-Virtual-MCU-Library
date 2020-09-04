/* Debug Commands Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Project Headers
#include "cli/debugcommands.h"
#include "cli/debugwindow.h"
#include "cli/stdmsg.h"
#include "system/system.h"
#include "system/mcudef.h"
#include "table/table.h"
#include "disassembler/decoder.h"
#include "printer/systemprinter.h"
#include "misc/stringmanip.h"
#include "misc/filemanip.h"
#include "misc/memmanip.h"
#include "collections/array.h"
#include "collections/list.h"

void jump_forward(debugwindow_t *window, system_t *sys, table_t *table, const int delay) {

    if(delay < 0) {

        dwin_write(window, OPNL, DELAY_ERR, D);
        return;
    }

    dwin_write(window, OPNL, JUMP_START, D);

    while(sys_is_term(sys) == false) {

        if(table_is_breakp(table) == true)
            break;

        if(delay >= 10)
            system_to_win(window, sys, table);

        sys_step(sys);

        if(delay != 0)
            usleep(delay * 1000);
    }

    dwin_write(window, OPNL, BREAK_REACHED, G);
}

void set_breakpoint(debugwindow_t *window, table_t *table, const char *bp) {

    if(table_add_breakp(table, bp) < 0)
        return;

    char *msg = bp_set_success(bp);
    dwin_write(window, OPNL, msg, D);

    free(msg);
}

void remove_breakpoint(debugwindow_t *window, table_t *table, const char *bp) {

    if(table_del_breakp(table, bp) < 0)
        return;

    char *msg = bp_del_success(bp);
    dwin_write(window, OPNL, msg, D);

    free(msg);
}

void examine_data(debugwindow_t *window, system_t *sys, const char *mem_cell) {

    const int cell = htoi(mem_cell);

    if(cell < 0 || cell > RAM_END) {

        dwin_write(window, OPNL, MEM_CELL_ERR, D);
        return;
    }

    const int8_t data = sys_read_data(sys, cell);

    char hex[3];
    to_hex(data, hex);

    char *msg = val_of(mem_cell, hex);
    dwin_write(window, OPNL, msg, D);

    free(msg);
}

void examine_eeprom(debugwindow_t *window, system_t *sys, const char *mem_cell) {

    const int cell = htoi(mem_cell);

    if(cell < 0 || cell >= EEPROM_SIZE) {

        dwin_write(window, OPNL, MEM_CELL_ERR, D);
        return;
    }

    const int8_t data = sys_read_eeprom(sys, cell);

    char hex[3];
    to_hex(data, hex);

    char *msg = val_of(mem_cell, hex);
    dwin_write(window, OPNL, val_of(mem_cell, hex), D);

    free(msg);
}

void examine_data_char(debugwindow_t *window, system_t *sys, const char *mem_cell, const char *range) {

    const int cell = htoi(mem_cell);
    const int offs = get_int(range);

    if((cell + offs) < 0 || (cell + offs) > RAM_END + 1) {

        dwin_write(window, OPNL, MEM_CELL_ERR, D);
        return;
    }

    if(offs < 1) {

        dwin_write(window, OPNL, MEM_RANGE_ERR, D);
        return;
    }

    char ascii[offs + 1];
    ascii[offs] = '\0';

    for(int i = cell; i < (cell + offs); i++) {

        const uint8_t byte = (uint8_t) sys_read_data(sys, i);
        ascii[i - cell] = byte;
    }

    char *msg = val_of_data(mem_cell, range, ascii);
    dwin_write(window, OPNL, msg, D);

    free(msg);
}

void examine_eeprom_char(debugwindow_t *window, system_t *sys, const char *mem_cell, const char *range) {

    const int cell = htoi(mem_cell);
    const int offs = get_int(range);

    if((cell + offs) < 0 || (cell + offs) >= EEPROM_SIZE + 1) {

        dwin_write(window, OPNL, MEM_CELL_ERR, D);
        return;
    }

    if(offs < 1) {

        dwin_write(window, OPNL, MEM_RANGE_ERR, D);
        return;
    }

    char ascii[offs + 1];
    ascii[offs] = '\0';

    for(int i = cell; i < (cell + offs); i++) {

        const uint8_t byte = (uint8_t) sys_read_eeprom(sys, i);
        ascii[i - cell] = byte;
    }

    char *msg = val_of_eep(mem_cell, range, ascii);
    dwin_write(window, OPNL, msg, D);

    free(msg);
}

void load_eep_hex(debugwindow_t *window, system_t *sys, const char *file) {

    array_t *buffer = array_ctor(1024, NULL, NULL);

    if(file_exists(file) == false) {

        char *msg = file_err(file);
        dwin_write(window, OPNL, msg, D);

        array_dtor(buffer);
        free(msg);

        return;
    }

    if(decode_eep(file, buffer) < 0) {

        dwin_write(window, OPNL, EEP_DECODE_ERR, D);
        array_dtor(buffer);

        return;
    }

    for(int i = 0; i < buffer->top; i++) {

        eep_t *eep = (eep_t*) array_at(buffer, i);
        sys_write_eeprom(sys, eep->addr, (int8_t) eep->value);
    }

    char *msg = eep_success(file);
    dwin_write(window, OPNL, msg, G);

    array_dtor(buffer);
    free(msg);
}

void clear_output(debugwindow_t *window) {

    dwin_clr_specific(window, OPNL);
}

void show_cycles(debugwindow_t *window, system_t *sys) {

    char cycles[64];

    sprintf(cycles, "(mdx) Cycles: %ld\n", sys->cycles);
    dwin_write(window, OPNL, cycles, D);
}

void show_clock(debugwindow_t *window, system_t *sys) {

    char *freq = nformat(sys->clock);

    dwin_add(window, OPNL, "(mdx) Clock: ", D);
    dwin_add(window, OPNL, freq, D);
    dwin_write(window, OPNL, " Hz\n", D);

    free(freq);
}