/* Implementation of Table Class */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>

// Project Headers
#include "table.hpp"
#include "ehandling.hpp"
#include "style.hpp"
#include "uparse.hpp"

#define PR_DELAY 1

using namespace std;

namespace {

	int bp_lookup(Table *table, string point) {

		int line;
		
		if((line = to_dec(point)) < 0)	// check return of map
			return table->get_label_ln(point);

		return line;
	}

	string get_label(string line) {
	
		int pos;
	
		if((pos = line.find(":")) == string::npos)
			return "";
	
		return line.substr(0, pos);
	}

	bool is_exec(string line) {

		int pos;

		if(line == "")
			return false;

		if((pos = line.find(".")) == 0)
			return false;

		if((pos = line.find(";")) == 0)
			return false;

		if((pos = line.find(":")) == line.size() - 1)
			return false;

		return true;
	}
};

/* --- Public --- */

Table::Table(string asm_file) {

	ifstream read_file(asm_file, ios::in);
	string c_line = ""; int i = 0;

	if(file_exists(asm_file) == false)
		goto set_data;

	while(getline(read_file, c_line)) {

		string label = "";
	
		this->content.push_back(c_line);

		trim(&c_line);
		c_line = del_comment(c_line);

		if((label = get_label(c_line)) != "")
			this->add_label(label, i);
			
		this->exec.push_back( is_exec(c_line) );
		this->breaks.push_back(false);
		
		i += 1;
	}

set_data:

	this->tip = -1;
	this->table_size = this->content.size();
	this->src_file = asm_file;

	read_file.close();
}

int Table::set_break(string point) {

	int line = bp_lookup(this, point);

	if(line < 0 || line >= this->table_size) {

		print_event("Invalid Breakpoint.");
		return -1;
	}

	if(this->breaks[line] == true) {
	
		print_event("Breakpoint already exists.");
		return 0;
	}

	this->breaks[line] = true;

	return 0;
}

int Table::unset_break(string point) {

	int line = bp_lookup(this, point);
	
	if(line < 0 || line >= this->table_size) {

		print_event("Invalid Breakpoint.");
		return -1;
	}

	if(this->breaks[line] == false) {

		print_event("Breakpoint already unset.");
		return 0;
	}

	this->breaks[line] = false;

	return 0;
}

bool Table::is_break(int line) {

	return this->breaks[line];
}

int Table::get_label_ln(string id) {

	if(this->label.find(id) == this->label.end())
		return -1;

	return this->label[id];
}

void Table::set_tip(int instr_line) {

	if(instr_line >= this->table_size) {

		print_event("Invalid Instruction line.");
		return;
	}

	this->tip = instr_line;
}

int Table::get_tip(void) {

	return this->tip;
}

string Table::get_content(int line) {

	return this->content[line];
}

bool Table::executable(void) {

	return this->exec[this->tip];
}

void Table::step(void) {

	if(this->tip >= this->table_size - 1) {
	
		print_event("Reached End of File.");
		this->table_size = 0;
		
		return;
	}

	/* toDo: fix this */

	this->tip += 1;
}

void Table::jump_break(void) {

	/*
	* Jumping to the next breakpoint
	* and popping it from the list.
	**/

	/* Setting Instruction to the start */
	this->tip = 0;
	
	bool is_break = false;
	int last_instr = this->tip;

	int i = 0;

	while((is_break = this->breaks[i]) == false) {

		if(i >= this->table_size) {

			print_event("No Breakpoints set.");
			this->tip = last_instr;
			return;
		}

		this->step();
		i += 1;
	}

	this->breaks[i] = false; 	
}

void Table::refresh(void) {

	Table temp_table(this->src_file);

	this->table_size = temp_table.table_size;
	this->src_file = temp_table.src_file;
	this->content.swap(temp_table.content);
	this->label.swap(temp_table.label);
}

int Table::size(void) {

	return this->table_size;
}

string Table::src(void) {

	return this->src_file;
}

string Table::to_str(int start, int end) {

	stringstream stream;
	stream << SEPERATOR << "Instructions:\n\n";			

	if(this->table_size == 0) {

		stream << "[ No Source available ]\n";
		stream << SEPERATOR;

		return stream.str();
	}

	if(end > this->table_size)
		end = this->table_size;

	for(int i = start; i < end; i++) {

		if(i == this->tip)
			stream << BLUE;

		if(i >= this->table_size) {
		
			stream << "\n";
			continue;
		}

		if(this->breaks[i] == true)
			stream << to_string(i) << RED << " [b+] " << DEFAULT;
		else
			stream << to_string(i) << "      ";

		stream << this->content[i] << "\n";
		stream << DEFAULT;
	}

	stream << SEPERATOR;
	return stream.str();
}

/* --- Private --- */

void Table::add_label(string id, int line) {

	this->label[id] = line;
}

/* --- Non Member --- */

Table* create_table(vector <string> asm_file, int amount) {

	if(amount < 1)
		return NULL;

	Table *table = (Table*) malloc(amount * sizeof(Table));

	for(int i = 0; i < amount; i++)
		new (&table[i]) Table(asm_file[i]);

	return table;
}

void refresh_all(Table *table, int amount) {

	for(int i = 0; i < amount; i++) {

		if(&table[i] == NULL)
			continue;

		table[i].refresh();
	}
}


