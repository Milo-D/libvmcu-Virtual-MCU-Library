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

// Project Headers
#include "table.hpp"
#include "ehandling.hpp"
#include "style.hpp"
#include "uparse.hpp"
#include "asmparser.hpp"

#define PR_DELAY 1

using namespace std;

namespace {

	string del_comment(string line) {

		int pos;

		if((pos = line.find(";")) == string::npos)
			return line;

		return line.substr(0, pos);		
	}

	void trim(string *x) {

		x->erase(remove(x->begin(), x->end(), ' '), x->end());
		x->erase(remove(x->begin(), x->end(), '\t'), x->end());
	}

	int bp_lookup(Table *table, string point) {

		int line;
		
		if((line = to_dec(point)) < 0)	// check return of map
			return table->get_label_ln(point);

		return line;
	}
};

Table::Table(string asm_file) {

	ifstream read_file(asm_file, ios::in);
	string c_line = ""; int i = 0;

	if(file_exists(asm_file) == false)
		goto set_data;

	while(getline(read_file, c_line)) {

		string label = "";
	
		this->line.push_back(c_line);

		trim(&c_line);
		c_line = del_comment(c_line);

		if((label = get_label(c_line)) != "")
			this->add_label(label, i);

		this->debug_line.push_back(c_line);
		i += 1;
	}

set_data:

	this->instr = -1;
	this->size = this->line.size();
	this->src_file = asm_file;

	for(int i = 0; i < this->size; i++)
		this->breaks.push_back(false);

	read_file.close();
}

int Table::set_break(string point) {

	int line = bp_lookup(this, point);

	if(line < 0 || line >= this->size) {

		print_delay("Invalid Breakpoint.", PR_DELAY, false);
		return -1;
	}

	if(this->breaks[line] == true) {
	
		print_delay("Breakpoint already exists.", PR_DELAY, false);
		return 0;
	}

	this->breaks[line] = true;

	return 0;
}

int Table::unset_break(string point) {

	int line = bp_lookup(this, point);
	
	if(line < 0 || line >= this->size) {

		print_delay("Invalid Breakpoint.", PR_DELAY, false);
		return -1;
	}

	if(this->breaks[line] == false) {

		print_delay("Breakpoint already unset", PR_DELAY, false);
		return 0;
	}

	this->breaks[line] = false;

	return 0;
}

void Table::add_label(string id, int line) {

	this->label[id] = line;
}

int Table::get_label_ln(string id) {

	if(this->label.find(id) == this->label.end())
		return -1;

	return this->label[id];
}

void Table::set_instr(int instr_line) {

	if(instr_line >= this->size) {

		print_delay("Invalid Instruction line.", PR_DELAY, false);
		return;
	}

	this->instr = instr_line;
}

int Table::get_instr(void) {

	return this->instr;
}

void Table::step(void) {

	if(this->instr >= this->size - 1) {
	
		print_delay("Reached End of File.", PR_DELAY, false);
		this->size = 0;
		
		return;
	}

	/* toDo: fix this */

	this->instr += 1;
}

void Table::jump_break(void) {

	/*
	* Jumping to the next breakpoint
	* and popping it from the list.
	**/

	/* Setting Instruction to the start */
	this->instr = 0;
	
	bool is_break = false;
	int last_instr = this->instr;

	int i = 0;

	while((is_break = this->breaks[i]) == false) {

		if(i >= this->size) {

			print_delay("No Breakpoints", PR_DELAY, false);
			this->instr = last_instr;
			return;
		}

		this->step();
		i += 1;
	}

	this->breaks[i] = false; 	
}

void Table::refresh(void) {

	Table temp_table(this->src_file);

	this->size = temp_table.size;
	this->src_file = temp_table.src_file;
	this->line.swap(temp_table.line);
	this->label.swap(temp_table.label);
	this->debug_line.swap(temp_table.debug_line);

	/*
	* Do not swap breakpoints
	* this->breaks.swap(temp_table.breaks);
	**/
}

void Table::print_short(int row) {

	cout << HORI_SEP;
	cout << "Instructions:\n\n";

	if(this->size == 0) {
			
		cout << "[ No Source available ]\n";
		goto end;
	}

	for(int i = this->instr; i < this->instr + row; i++) {

		if(i == this->instr)
			cout << BLUE;

		if(i >= this->size) {
		
			cout << "\n";
			continue;
		}

		if(this->breaks[i] == true)
			cout << i << RED << " [b+] " << DEFAULT;
		else
			cout << i << "      ";

		cout << this->line[i] << endl;
		cout << DEFAULT;
	}
	
end:
	cout << HORI_SEP;
}

void Table::print(void) {

	cout << HORI_SEP;

	if(this->size == 0) {
				
		cout << "[ No Source available ]\n";
		goto end;
	}

	for(int i = 0; i < this->size; i++) {

		if(i == this->instr)
			cout << BLUE;

		if(this->breaks[i] == true)
			cout << i << RED << " [b+] " << DEFAULT;
		else
			cout << i << "      ";
			
		cout << this->line[i] << endl;
		cout << DEFAULT;
	}

end:
	cout << HORI_SEP;
}

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


