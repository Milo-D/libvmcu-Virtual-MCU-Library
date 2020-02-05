/* Systemprinter Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

// Project Headers
#include "printer/systemprinter.hpp"
#include "printer/memprop.hpp"
#include "system/sys.hpp"
#include "system/mcu.hpp"
#include "cli/debugwindow.hpp"
#include "table/table.hpp"
#include "misc/stringmanip.hpp"

using namespace std;

namespace {

    int color(const int mem_prop) {

        int color;

        switch(mem_prop) {

            case SRC: color = R; break;
            case DEST: color = G; break;

            default: color = DEF; break;
        }

        return color;
    }
};

static void print_gpr(DebugWindow *dwin, Sys *sys) {

    stringstream stream;
    dwin->add(GPR_PANEL, "Registers:\n\n", DEF);

    vector <int8_t> buffer;
    sys->dump_gpr(buffer);

    vector <int> cells_of_interest;
    sys->gpr_coi(cells_of_interest);

    const int s = (dwin->cursor_of(GPR_PANEL) * 8);

    for(int i = 0; i < 8; i++) {

        if(i == 4)
            dwin->add(GPR_PANEL, "\n", DEF);

        const int c = color(cells_of_interest[s + i]);

        dwin->add(GPR_PANEL, "R" + to_string(s + i), c);
        dwin->add(GPR_PANEL, ": ", DEF);

        if((s + i) < 10)
            dwin->add(GPR_PANEL, " ", DEF);

        stream << "0x" << right << setw(2) << setfill('0');
        stream << get_hex(buffer[s + i]);

        stream << setfill(' ') << left << setw(13);
        stream << "      ";

        dwin->add(GPR_PANEL, stream.str(), DEF);
        stream.str(string());
    }

    dwin->add(GPR_PANEL, "\n", DEF);
}

static void print_sreg(DebugWindow *dwin, Sys *sys) {

    stringstream stream;
    dwin->add(SREG_PANEL, "Status-Register:\n\n", DEF);

    vector <int> cells_of_interest;
    sys->sreg_coi(cells_of_interest);

    uint8_t status = sys->dump_sreg();

    for(int i = 0; i < SREG_SIZE; i++) {

        if(i == SREG_SIZE / 2)
            dwin->add(SREG_PANEL, "\n", DEF);

        const int c = color(cells_of_interest[i]);
        const int val = ((status >> i) & 0x01);

        dwin->add(SREG_PANEL, flags[i], c);

        stream << ": " << setfill(' ') << right << setw(2);
        stream << " 0x0" << hex << val << dec;
        stream << setfill(' ') << left << setw(13);
        stream << "        ";

        dwin->add(SREG_PANEL, stream.str(), DEF);
        stream.str(string());
    }

    dwin->add(SREG_PANEL, "\n", DEF);
}

static void print_data(DebugWindow *dwin, Sys *sys) {

    stringstream stream;
    dwin->add(DATA_PANEL, "Data Memory:\n\n", DEF);

    int cursor;

    vector <int8_t> buffer;
    sys->dump_data(buffer);

    tuple <int, int> cell_of_interest;
    sys->data_coi(cell_of_interest);

    if(get <1> (cell_of_interest) != NONE) {

        cursor = get <0> (cell_of_interest);
        dwin->set_cursor(DATA_PANEL, cursor);

    } else {

        cursor = dwin->cursor_of(DATA_PANEL);
    }

    const uint16_t sp = ((buffer[SPH] << 8) + buffer[SPL]);

    for(int i = (cursor - 4); i <= (cursor + 4); i++) {

        int isp = DEF; int ism = DEF;

        if(i < 0 || i > RAM_END) {

            dwin->add(DATA_PANEL, "\n", DEF);
            continue;
        }

        if(i == sp)
            isp = B;

        if(i == get <0> (cell_of_interest))
            ism = color(get <1> (cell_of_interest));

        stream << "0x" << setfill('0') << setw(4);
        stream << hex << i << "      ";

        dwin->add(DATA_PANEL, stream.str(), isp);
        stream.str(string());

        stream << "0x" << setfill('0') << setw(2);
        stream << get_hex(buffer[i]);

        dwin->add(DATA_PANEL, stream.str() + "\n", ism);
        stream.str(string());
    }
}

static void print_eeprom(DebugWindow *dwin, Sys *sys) {

    stringstream stream;
    dwin->add(EEPROM_PANEL, "EEPROM:\n\n", DEF);

    int cursor;

    vector <int8_t> buffer;
    sys->dump_eeprom(buffer);

    tuple <int, int> cell_of_interest;
    sys->eeprom_coi(cell_of_interest);

    if(get <1> (cell_of_interest) != NONE) {

        cursor = get <0> (cell_of_interest);
        dwin->set_cursor(EEPROM_PANEL, cursor);

    } else {

        cursor = dwin->cursor_of(EEPROM_PANEL);
    }

    for(int i = (cursor - 4); i <= (cursor + 4); i++) {

        int ism = DEF;

        if(i < 0 || i > EEPROM_SIZE) {

            dwin->add(EEPROM_PANEL, "\n", DEF);
            continue;
        }

        stream << "0x" << setfill('0') << setw(4);
        stream << hex << i << "      ";

        dwin->add(EEPROM_PANEL, stream.str(), DEF);
        stream.str(string());

        if(i == get <0> (cell_of_interest))
            ism = color(get <1> (cell_of_interest));

        stream << "0x" << setfill('0') << setw(2);
        stream << get_hex(buffer[i]);
        
        dwin->add(EEPROM_PANEL, stream.str() + "\n", ism);
        stream.str(string());
    }
}

static void print_table(DebugWindow *dwin, Table *table) {

    stringstream stream;
    dwin->add(CODE_PANEL, "Instructions:\n\n", DEF);

    vector <string> content;
    table->dump_content(content);

    vector <bool> breaks;
    table->dump_breakpoints(breaks);

    const int tip = table->get_tip();

    for(int i = (tip - 4); i <= (tip + 4); i++) {

        int isp = DEF; int isb = DEF;

        if(i < 0 || i > table->size() - 1) {

            dwin->add(CODE_PANEL, "\n", DEF);
            continue;
        }

        if(i == tip)
            isp = B;

        if(breaks[i] == true)
            isb = R;

        stream << "0x" << setfill('0') << setw(4);
        stream << hex << i;

        dwin->add(CODE_PANEL, stream.str(), isp);

        if(breaks[i] == true)
            dwin->add(CODE_PANEL, " [b+] ", R);
        else
            dwin->add(CODE_PANEL, "      ", DEF);

        dwin->add(CODE_PANEL, content[i], isp);
        dwin->add(CODE_PANEL, "\n", DEF);

        stream.str(string());
    }
}

static void print_side_table(DebugWindow *dwin, Table *table) {

    stringstream stream;
    dwin->add(SIDE_PANEL, "Source Code:\n\n", DEF);

    vector <string> content;
    table->dump_content(content);

    vector <bool> breaks;
    table->dump_breakpoints(breaks);

    if(table->size() == 0) {

        dwin->add(SIDE_PANEL, "[ No Source available ]\n", DEF);
        return;
    }

    const int cursor = dwin->cursor_of(SIDE_PANEL);
    const int height = (dwin->get_height(SIDE_PANEL) - 4);

    const int tip = table->get_tip();
    const int start = (cursor * height);

    for(int i = start; i < (start + height); i++) {

        int isp = DEF;
        stream << setw(3) << setfill(' ');

        if(i >= table->size()) {

            stream << to_string(i) << "\n";
            dwin->add(SIDE_PANEL, stream.str(), DEF);
            stream.str(string());

            continue;
        }

        if(i == tip)
            isp = B;

        stream << to_string(i);

        dwin->add(SIDE_PANEL, stream.str(), isp);
        stream.str(string());

        if(breaks[i] == true)
            dwin->add(SIDE_PANEL, " [b+] ", R);
        else
            dwin->add(SIDE_PANEL, "      ", DEF);

        dwin->add(SIDE_PANEL, content[i] + "\n", isp);
    }
}

extern void system_to_win(DebugWindow *dwin, Sys *sys, Table *table) {

    dwin->clear();

    print_gpr(dwin, sys);
    print_sreg(dwin, sys);
    print_data(dwin, sys);
    print_eeprom(dwin, sys);

    print_table(dwin, table);
    print_side_table(dwin, table);

    dwin->update_all();
}