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
#include "table.hpp"
#include "ehandling.hpp"
#include "stringmanip.hpp"
#include "disassembler.hpp"
#include "style.hpp"

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

    if(this->tip == this->table_size - 1)
        return -1;

    this->tip += 1;
    return 0;
}

int Table::set_break(string point) {

    int line = to_dec(point);

    if(line < 0 || line >= this->table_size) {

        print_event("Invalid Breakpoint.");
        return -1;
    }

    if(this->breaks[line] == true) {
	
        print_event("Breakpoint already exists.");
        return 0;
    }

    this->breaks[line] = true;
    this->break_counter += 1;

    return 0;
}

int Table::unset_break(string point) {

    int line = to_dec(point);
	
    if(line < 0 || line >= this->table_size) {

        print_event("Invalid Breakpoint.");
        return -1;
    }

    if(this->breaks[line] == false) {

        print_event("Breakpoint already unset.");
        return 0;
    }

    this->breaks[line] = false;
    this->break_counter -= 1;

    return 0;
}

void Table::set_tip(int instr_line) {

    if(instr_line >= this->table_size) {

        print_event("Invalid Instruction line.");
        return;
    }

    this->tip = instr_line;
}

void Table::jump(int exec_addr) {

    int i = 0;

    while(get <1> (this->content[i]) != exec_addr)
        i += 1;

    if(get <1> (this->content[i - 2]) < 0) {

        this->tip = (i - 2);
        return;
    }

    this->tip = (i - 1);
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

    this->page = ((this->page + offs) % this->page_size);
}

string Table::src(void) {

    return this->source_file;
}

string Table::to_str(void) {

    stringstream stream;
    stream << SEPERATOR << "Source Code:\n\n";			

    if(this->table_size == 0) {

        stream << "[ No Source available ]\n";
        stream << SEPERATOR;

        return stream.str();
    }

    int page_start = (this->page * PGS);
    int page_end = ((this->page * PGS) + PGS);

    for(int i = page_start; i < page_end; i++) {

        stream << setw(3) << setfill(' ');

        if(i >= this->table_size) {

            stream << to_string(i) << "\n";
            continue;
        }

        if(this->breaks[i] == true)
            stream << to_string(i) << RED << " [b+] " << DEFAULT;
        else
            stream << to_string(i) << "      ";

        stream << get <0> (this->content[i]) << "\n";
        stream << DEFAULT;
    }

    stream << SEPERATOR;
    return stream.str();
}

string Table::center_to_str(void) {

    stringstream stream;
    stream << SEPERATOR << "Instructions:\n\n"; 

    for(int i = (this->tip - 4); i <= (this->tip + 4); i++) {

        if(i < 0 || i > this->table_size - 1) {

            stream << "\n";
            continue;
        }

        if(i == this->tip)
            stream << BLUE;

        stream << "0x" << setfill('0') << setw(4);

        if(this->breaks[i] == true)
            stream << hex << i << RED << " [b+] " << DEFAULT;
        else
            stream << hex << i << "      ";

        stream << get <0> (this->content[i]) << "\n";
        stream << DEFAULT;
    }

    stream << SEPERATOR;
    return stream.str();
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


