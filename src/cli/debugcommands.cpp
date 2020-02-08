/* Debug Commands Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <inttypes.h>

// Project Headers
#include "cli/debugcommands.hpp"
#include "cli/debugwindow.hpp"
#include "system/sys.hpp"
#include "system/mcu.hpp"
#include "table/table.hpp"
#include "disassembler/decoder.hpp"
#include "printer/systemprinter.hpp"
#include "misc/stringmanip.hpp"
#include "misc/filemanip.hpp"
#include "misc/stdmsg.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

void jump_forward(DebugWindow *dwin, Sys *sys, Table *table, const int delay) {

    if(delay < 0) {

        dwin->write(OUTPUT_PANEL, INVALID_DELAY, DEF);
        return;
    }

    dwin->write(OUTPUT_PANEL, JUMP_START, DEF);

    do {

        if(table->is_break() == true)
            break;

        if(delay >= 10)
            system_to_win(dwin, sys, table);

        sys->step();

        sleep_for(milliseconds(delay));

    } while(sys->is_terminated() == false);

    dwin->write(OUTPUT_PANEL, BREAK_REACHED, G);
}

void set_breakpoint(DebugWindow *dwin, Table *table, const string & bp) {

    if(table->set_break(bp) < 0)
        return;

    dwin->write(OUTPUT_PANEL, break_set_success(bp), DEF);
}

void remove_breakpoint(DebugWindow *dwin, Table *table, const string & bp) {

    if(table->unset_break(bp) < 0)
        return;

    dwin->write(OUTPUT_PANEL, break_rem_success(bp), DEF);
}

void examine_data(DebugWindow *dwin, Sys *sys, const string & mem_cell) {

    int cell = hex_to_dec(mem_cell);

    if(cell < 0 || cell > RAM_END) {

        dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
        return;
    }

    int8_t data = sys->read_data(cell);
    dwin->write(OUTPUT_PANEL, val_of(mem_cell, get_hex(data)), DEF);
}

void examine_eeprom(DebugWindow *dwin, Sys *sys, const string & mem_cell) {

    int cell = hex_to_dec(mem_cell);

    if(cell < 0 || cell > EEPROM_SIZE - 1) {

        dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
        return;
    }

    int8_t data = sys->read_eeprom(cell);
    dwin->write(OUTPUT_PANEL, val_of(mem_cell, get_hex(data)), DEF);
}

void examine_eeprom_char(DebugWindow *dwin, Sys *sys, const string & mem_cell, const string & range) {

    int cell = hex_to_dec(mem_cell);
    int offs = to_dec(range);

    if(offs < 0 || (cell + offs) < 0 || (cell + offs) > EEPROM_SIZE - 1) {

        dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
        return;
    }

    string ascii = "";

    for(int i = cell; i < (cell + offs); i++) {

        uint8_t byte = (uint8_t) sys->read_eeprom(i);

        if(byte == '\0') {

            ascii += "\\0";
            continue;
        }

        ascii += byte;
    }

    dwin->write(OUTPUT_PANEL, val_of_eep(mem_cell, range, ascii), DEF);
}

void load_eep_hex(DebugWindow *dwin, Sys *sys, const string & file) {

    vector <struct data> d;

    if(file_exists(file) == false) {

        dwin->write(OUTPUT_PANEL, file_invalid(file), DEF);
        return;
    }

    decode_eep_hex(file).swap(d);

    if(d.size() == 0) {

        dwin->write(OUTPUT_PANEL, EEP_DECODE_ERR, DEF);
        return;
    }

    for(int i = 0; i < d.size(); i++)
        sys->write_eeprom(d[i].addr, (int8_t) d[i].value);

    dwin->write(OUTPUT_PANEL, eep_success(file), G);
}

void clear_output(DebugWindow *dwin) {

    dwin->clear_specific(OUTPUT_PANEL);
}