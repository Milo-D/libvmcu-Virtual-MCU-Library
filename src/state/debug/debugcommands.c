/* Debug Commands Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Project Headers
#include "state/debug/debugcommands.h"
#include "state/debug/stdmsg.h"
#include "cli/debug/debugwindow.h"
#include "system/system.h"
#include "system/mcudef.h"
#include "disassembler/decoder.h"
#include "printer/systemprinter.h"
#include "misc/stringmanip.h"
#include "misc/bitmanip.h"
#include "misc/filemanip.h"
#include "misc/memmanip.h"
#include "collections/array.h"
#include "collections/list.h"

void jump_forward(debugwindow_t *window, system_t *sys, const int delay) {

    if(delay < 0) {

        dwin_write(window, OPNL, DELAY_ERR, D);
        return;
    }

    dwin_write(window, OPNL, JUMP_START, D);

    while(true) {

        if(sys_on_breakp(sys) == true)
            break;

        if(delay >= 10)
            system_to_win(window, sys);

        sys_step(sys);

        if(delay != 0)
            usleep(delay * 1000);
    }

    dwin_write(window, OPNL, BREAK_REACHED, G);
}

void jump_cycles(debugwindow_t *window, system_t *sys, const int n) {

    if(n < 0) {
        
        dwin_write(window, OPNL, CYCLE_ERR, D);
        return;
    }
    
    const uint64_t end = (sys->cycles + n);
    dwin_write(window, OPNL, CYCLE_JUMP_START, D);

    while(sys->cycles < end)
        sys_step(sys);

    dwin_write(window, OPNL, CYCLE_REACHED, G);
}

void set_breakpoint(debugwindow_t *window, system_t *sys, const char *bp) {

    if(sys_add_breakp(sys, bp) < 0) {

        char *msg = bp_set_failure(bp);
        dwin_write(window, OPNL, msg, D);

        free(msg);
        return;
    }

    char *msg = bp_set_success(bp);
    dwin_write(window, OPNL, msg, D);

    free(msg);
}

void remove_breakpoint(debugwindow_t *window, system_t *sys, const char *bp) {

    if(sys_del_breakp(sys, bp) < 0) {

        char *msg = bp_del_failure(bp);
        dwin_write(window, OPNL, msg, D);

        free(msg);
        return;
    }

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

    const int8_t *memory = sys_dump_eeprom(sys);
    
    char hex[3];
    to_hex(memory[cell], hex);

    char *msg = val_of(mem_cell, hex);

    dwin_write(window, OPNL, msg, D);
    dwin_set_curs(window, EPNL, cell);

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
    
    int8_t *memory = sys_dump_eeprom(sys);

    for(int i = cell; i < (cell + offs); i++)
        ascii[i - cell] = (uint8_t) memory[i];

    char *msg = val_of_eep(mem_cell, range, ascii);

    dwin_write(window, OPNL, msg, D);
    dwin_set_curs(window, EPNL, (cell + offs - 1));

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

    int8_t *memory = sys_dump_eeprom(sys);

    for(int i = 0; i < buffer->top; i++) {

        eep_t *eep = (eep_t*) array_at(buffer, i);
        memory[eep->addr] = (int8_t) eep->value;
    }

    char *msg = eep_success(file);
    dwin_write(window, OPNL, msg, G);

    array_dtor(buffer);
    free(msg);
}

void clear_output(debugwindow_t *window) {

    dwin_clear_panel(window, OPNL);
}

void show_cycles(debugwindow_t *window, system_t *sys) {

    char *cycles_str = get_cycles(sys->cycles);
    dwin_write(window, OPNL, cycles_str, D);

    free(cycles_str);
}

void show_clock(debugwindow_t *window, system_t *sys) {

    char *clock_str = get_clock(sys->clock);
    dwin_write(window, OPNL, clock_str, D);

    free(clock_str);
}

void show_time(debugwindow_t *window, system_t *sys) {

    const double c = sys->cycles;
    const double f = sys->clock;

    const double time = ((c / f) * 1000000);

    char *time_str = get_time(time);
    dwin_write(window, OPNL, time_str, D);

    free(time_str);
}

void examine_data_byte(debugwindow_t *window, system_t *sys, const char *mem_cell) {

    const int cell = htoi(mem_cell);

    if(cell < 0 || cell > RAM_END) {

        dwin_write(window, OPNL, MEM_CELL_ERR, D);
        return;
    }

    const uint8_t data = sys_read_data(sys, cell);

    char byte[10];

    byte[4] = ' ';
    byte[9] = '\0';

    for(int i = 0; i < 4; i++)
        byte[i] = bit(data, 7 - i) + 48;

    for(int i = 4; i < 8; i++)
        byte[i + 1] = bit(data, 7 - i) + 48;

    char *msg = bit_val_of(mem_cell, byte);
    dwin_write(window, OPNL, msg, D);

    free(msg);   
}

void create_comment(debugwindow_t *window, system_t *sys, const char *line, const char *comment) {
    
    const int lnno = get_int(line);
    const int len = strlen(comment);
    
    const char *mnem = sys_read_table(sys, lnno);
    const int pos = strpos(mnem, ";");

    if(len > 32) {
        
        dwin_write(window, OPNL, CC_TOO_LONG, D);
        return;
    }
    
    if(mnem == NULL || pos < 0) {

        dwin_write(window, OPNL, CC_FAILURE, D);
        return;
    }

    char *sub = substr(mnem, 0, pos + 1);
    char *new_mnem = strxcat(sub, comment);

    sys_write_table(sys, lnno, new_mnem);
    dwin_write(window, OPNL, CC_SUCCESS, D);

    free(new_mnem);
    free(sub);
}









