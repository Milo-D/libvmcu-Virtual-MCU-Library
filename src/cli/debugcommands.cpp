/* Debug Commands Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <inttypes.h>

// Project Headers
#include "cli/debugcommands.hpp"
#include "cli/debugwindow.hpp"
#include "cli/style.hpp"
#include "system/sys.hpp"
#include "system/mcu.hpp"
#include "table/table.hpp"
#include "misc/stringmanip.hpp"
#include "misc/stdmsg.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

void jump_forward(DebugWindow *dwin, Sys *sys, Table *table) {

    do {

        if(table->is_break() == true)
            break;

        sys->put_sys(dwin);
        sys->step();

        sleep_for(milliseconds(100));

    } while(sys->is_terminated() == false);

    dwin->write(OUTPUT_PANEL, BREAK_REACHED, G);
}

void examine_data(DebugWindow *dwin, Sys *sys, string mem_cell) {

    int cell = hex_to_dec(mem_cell);

    if(cell < 0 || cell > RAM_END) {

        dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
        return;
    }

    int8_t data = sys->read_data(cell);
    dwin->write(OUTPUT_PANEL, val_of(mem_cell, get_hex(data)), DEF);
}

void examine_eeprom(DebugWindow *dwin, Sys *sys, string mem_cell) {

    int cell = hex_to_dec(mem_cell);

    if(cell < 0 || cell > EEPROM_SIZE - 1) {

        dwin->write(OUTPUT_PANEL, MEM_CELL_ERR, DEF);
        return;
    }

    int8_t data = sys->read_eeprom(cell);
    dwin->write(OUTPUT_PANEL, val_of(mem_cell, get_hex(data)), DEF);
}