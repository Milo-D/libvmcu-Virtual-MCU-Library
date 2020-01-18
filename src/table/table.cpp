/* Implementation of Table Class */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <iterator>
#include <algorithm>
#include <map>
#include <tuple>
#include <fstream>
#include <sstream>

// Project Headers
#include "table/table.hpp"
#include "misc/stringmanip.hpp"
#include "disassembler/disassembler.hpp"
#include "cli/debugwindow.hpp"
#include "cli/style.hpp"

#define PGS 31

using namespace std;

/* --- Public --- */

Table::Table(string hex_file) {

    create_source(hex_file).swap(this->content);

    for(int i = 0; i < this->content.size(); i++)
        this->breaks.push_back(false);

    this->table_size = this->content.size();
    this->page_size = this->table_size / PGS;

    if(this->table_size % PGS != 0)
        this->page_size += 1;

    this->tip = 0;
    this->page = 0;
    this->source_file = hex_file;
}

int Table::step(void) {

    if(this->tip >= this->table_size - 1)
        return -1;

    if(this->tip < 0)
        return -1;

    this->tip += 1;
    return 0;
}

int Table::set_break(string point) {

    int line = to_dec(point);

    if(line < 0 || line >= this->table_size)
        return -1;

    if(this->breaks[line] == true)
        return 0;

    this->breaks[line] = true;
    this->break_counter += 1;

    return 0;
}

int Table::unset_break(string point) {

    int line = to_dec(point);
	
    if(line < 0 || line >= this->table_size)
        return -1;

    if(this->breaks[line] == false)
        return 0;

    this->breaks[line] = false;
    this->break_counter -= 1;

    return 0;
}

void Table::define(const string &alias, const string &seq) {

    for(int i = 0; i < this->table_size; i++) {

        int pos;
        string line = get <0> (this->content[i]);

        if(get <1> (this->content[i]) < 0) {

            get <0> (this->content[i]) = replace_str(line, alias, seq);
            continue;
        }

        if((pos = line.find(";")) == string::npos)
            continue;

        string code = line.substr(0, pos);
        string comment = line.substr(pos, line.size());

        get <0> (this->content[i]) = replace_str(code, alias, seq) + comment;
    }
}

void Table::set_tip(int instr_line) {

    if(instr_line >= this->table_size)
        return;

    if(instr_line < 0)
        return;

    this->tip = instr_line;
}

void Table::jump(int exec_addr) {

    int i = 0;

    while(get <1> (this->content[i]) != exec_addr) {

        i += 1;

        if(i >= this->table_size) {

            this->set_tip(-1);
            return;
        }
    }

    if(get <1> (this->content[i - 2]) < 0) {

        this->set_tip(i - 2);
        return;
    }

    this->set_tip(i - 1);
}

bool Table::is_break(void) {

    return this->breaks[this->tip];
}

bool Table::has_break(void) {

    return (this->break_counter > 0);
}

bool Table::is_sync(int hex_addr) {

    int match = get <1> (this->content[this->tip]);

    return (match == hex_addr);
}

int Table::size(void) {

    return this->table_size;
}

void Table::next_page(int offs) {

    if(this->page + offs < 0)
        return;

    if(this->table_size == 0)
        return;

    this->page = ((this->page + offs) % this->page_size);
}

string Table::src(void) {

    return this->source_file;
}

void Table::to_win(DebugWindow *dwin, bool full) {  

    if(full == true) {

        this->full_to_win(dwin);
        return;
    }

    stringstream stream;
    dwin->write(CODE_PANEL, "Instructions:\n\n", DEF); 

    for(int i = (this->tip - 4); i <= (this->tip + 4); i++) {

        int isp = DEF; int isb = DEF;

        if(i < 0 || i > this->table_size - 1) {

            dwin->write(CODE_PANEL, "\n", DEF);
            continue;
        }

        if(i == this->tip)
            isp = B;

        if(this->breaks[i] == true)
            isb = R;

        stream << "0x" << setfill('0') << setw(4);
        stream << hex << i;

        dwin->write(CODE_PANEL, stream.str(), isp);

        if(this->breaks[i] == true)
            dwin->write(CODE_PANEL, " [b+] ", R);
        else
            dwin->write(CODE_PANEL, "      ", DEF);

        dwin->write(CODE_PANEL, get <0> (this->content[i]), isp);
        dwin->write(CODE_PANEL, "\n", DEF);

        stream.str(string());
    }
}

/* --- Private --- */

void Table::full_to_win(DebugWindow *dwin) {

    stringstream stream;
    dwin->write(SIDE_PANEL, "Source Code:\n\n", DEF);   

    if(this->table_size == 0) {

        dwin->write(SIDE_PANEL, "[ No Source available ]\n", DEF);
        return;
    }

    int page_start = (this->page * PGS);
    int page_end = ((this->page * PGS) + PGS);

    for(int i = page_start; i < page_end; i++) {

        int isp = DEF;
        stream << setw(3) << setfill(' ');

        if(i >= this->table_size) {

            stream << to_string(i) << "\n";
            dwin->write(SIDE_PANEL, stream.str(), DEF);
            stream.str(string());

            continue;
        }

        if(i == this->tip)
            isp = B;

        stream << to_string(i);

        dwin->write(SIDE_PANEL, stream.str(), isp);
        stream.str(string());

        if(this->breaks[i] == true)
            dwin->write(SIDE_PANEL, " [b+] ", R);
        else
            dwin->write(SIDE_PANEL, "      ", DEF);

        dwin->write(SIDE_PANEL, get <0> (this->content[i]) + "\n", isp);
    }
}

/* --- Non Member --- */

Table* create_table(vector <string> hex_file, int amount) {

    if(amount < 1)
        return NULL;

    Table *table = (Table*) malloc(amount * sizeof(Table));

    for(int i = 0; i < amount; i++)
        new (&table[i]) Table(hex_file[i]);

    return table;
}


