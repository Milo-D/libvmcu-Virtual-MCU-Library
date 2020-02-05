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

using namespace std;

/* --- Public --- */

Table::Table(string hex_file) {

    create_source(hex_file).swap(this->content);

    for(int i = 0; i < this->content.size(); i++)
        this->breaks.push_back(false);

    this->tip = 0;
    this->source_file = hex_file;
    this->table_size = this->content.size();
}

int Table::step(void) {

    if(this->tip >= this->table_size - 1)
        return -1;

    if(this->tip < 0)
        return -1;

    this->tip += 1;
    return 0;
}

int Table::set_break(const string & point) {

    const int line = to_dec(point);

    if(line < 0 || line >= this->table_size)
        return -1;

    if(this->breaks[line] == true)
        return -1;

    this->breaks[line] = true;
    this->break_counter += 1;

    return 0;
}

int Table::unset_break(const string & point) {

    const int line = to_dec(point);
	
    if(line < 0 || line >= this->table_size)
        return -1;

    if(this->breaks[line] == false)
        return -1;

    this->breaks[line] = false;
    this->break_counter -= 1;

    return 0;
}

void Table::define(const string & alias, const string & seq) {

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

int Table::get_tip(void) {

    return this->tip;
}

void Table::set_tip(const int instr_line) {

    if(instr_line >= this->table_size)
        return;

    if(instr_line < 0)
        return;

    this->tip = instr_line;
}

void Table::jump(const int exec_addr) {

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

bool Table::is_sync(const int hex_addr) {

    const int match = get <1> (this->content[this->tip]);
    return (match == hex_addr);
}

int Table::size(void) {

    return this->table_size;
}

string Table::src(void) {

    return this->source_file;
}

void Table::dump_content(vector <string> & buffer) {

    for(int i = 0; i < this->table_size; i++)
        buffer.push_back(get <0> (this->content[i]));
}

void Table::dump_breakpoints(vector <bool> & buffer) {

    for(int i = 0; i < this->table_size; i++)
        buffer.push_back(this->breaks[i]);
}

/* --- Non Member --- */

void create_table(vector <Table> & buffer, vector <string> hex_file) {

    for(int i = 0; i < hex_file.size(); i++)
        buffer.push_back(Table(hex_file[i]));
}


