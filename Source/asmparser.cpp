/* Implementation of asmparser Header */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>

// Project Headers
#include "asmparser.hpp"
#include "ehandling.hpp"
#include "uparse.hpp"
#include "table.hpp"
#include "simsys.hpp"
#include "attributes.hpp"

#define PR_DELAY 1

using namespace std;

namespace {

	bool is_label(struct attributes *attr, string line) {

		if(line.find(":") == string::npos)
			return false;

		attr->instr = SKIP;
		return true;
	}

	bool is_mov(struct attributes *attr, string line) {

		bool tar_ok, data_ok;
		string instr = line.substr(0, 3);

		int pos;

		if(instr != "mov")
			return false;

		if((pos = line.find(",")) == string::npos) {

			print_delay("Invalid mov Instruction.", PR_DELAY, false);
			return false;
		}

		string tar = line.substr(3, 3);

		if(line.size() < pos + 1)
			return false;
		
		string data = line.substr(pos + 1, line.size());

		for(int i = 0; i < REG_NO; i++) {			

			if(tar == registers[i])
				tar_ok = true;

			if(data == registers[i])
				data_ok = true;
		}

		if(to_dec(data) != -1)
			data_ok = true;

		attr->instr = MOV;
		attr->dest = tar;
		attr->src = data;

		return (tar_ok & data_ok);						
	}

	bool is_push(struct attributes *attr, string line) {

		string instr = line.substr(0, 4);

		if(line.size() < 5)
			return false;

		if(instr != "push")
			return false;

		string data = line.substr(4, line.size());

		for(int i = 0; i < REG_NO; i++) {

			if(data == registers[i]) {

				attr->instr = PUSH;
				attr->src = data;
				
				return true;
			}
		}

		print_delay("Invalid push Instruction.", PR_DELAY, false);
		
		return false;
	}

	bool is_pop(struct attributes *attr, string line) {

		string instr = line.substr(0, 3);

		if(line.size() < 4)
			return false;

		if(instr != "pop")
			return false;

		string tar = line.substr(3, 3);

		for(int i = 0; i < REG_NO; i++) {

			if(tar == registers[i]) {

				attr->instr = POP;
				attr->dest = tar;
				
				return true;
			}
		}

		print_delay("Invalid pop Instruction.", PR_DELAY, false);

		return false;
	}

	bool is_jmp(Table *table, struct attributes *attr, string line) {

		string instr = line.substr(0, 3);

		if(line.size() < 4)
			return false;

		if(instr != "jmp")
			return false;

		string label = line.substr(3, line.size());

		if(table->get_label_ln(label) == -1) {

			print_delay("Invalid Label.", PR_DELAY, false);
			return false;
		}

		attr->instr = JMP;
		attr->label = label;
		
		return true;
	}
	
	bool is_nop(struct attributes *attr, string line) {

		attr->instr = NOP;
		return (line == "nop");
	}
	
	bool is_sub(struct attributes *attr, string line) {

		/* toDo: fix this */

		attr->instr = SUB;
		return (line.find("sub") == 0);
	}

	bool is_cmp(struct attributes *attr, string line) {

		bool tar_ok, data_ok;
		string instr = line.substr(0, 3);
		
		int pos;
		
		if(instr != "cmp")
			return false;
		
		if((pos = line.find(",")) == string::npos) {
		
			print_delay("Invalid cmp Instruction.", PR_DELAY, false);
			return false;
		}
		
		string tar = line.substr(3, 3);
		
		if(line.size() < pos + 1)
			return false;
				
		string data = line.substr(pos + 1, line.size());
		
		for(int i = 0; i < REG_NO; i++) {			
		
			if(tar == registers[i])
				tar_ok = true;
		
			if(data == registers[i])
				data_ok = true;
		}
		
		if(to_dec(data) != -1)
			data_ok = true;
		
		attr->instr = CMP;
		attr->dest = tar;
		attr->src = data;
		
		return (tar_ok & data_ok);
	}

	bool is_je(Table *table, struct attributes *attr, string line) {

		string instr = line.substr(0, 2);

		if(line.size() < 3)
			return false;

		if(instr != "je")
			return false;

		string label = line.substr(2, line.size());

		if(table->get_label_ln(label) == -1) {

			print_delay("Invalid Label.", PR_DELAY, false);
			return false;
		}

		attr->instr = JE;
		attr->label = label;
		
		return true;
	}

	bool is_int(struct attributes *attr, string line) {

		string instr = line.substr(0, 3);

		if(line.size() < 4)
			return false;

		if(instr != "int")
			return false;

		int id;
		string int_id = line.substr(3, line.size());

		if((id = hex_to_dec(int_id)) == -1) {

			print_delay("Unknown Interrupt.", PR_DELAY, false);
			return false;
		}

		attr->instr = INT;
		attr->int_id = id;
			
		return true;
	}
	
};

bool parse_ln(Table *table, struct attributes *attr, int line) {

	int ret = false;
	
	string debug_ln = table->debug_line[line];

	if(debug_ln == "") {

		attr->instr = SKIP;
		return true;
	}

	if(is_label(attr, debug_ln) == true)
		return true;	

	if(is_mov(attr, debug_ln) == true)
		return true;

	if(is_push(attr, debug_ln) == true)
		return true;

	if(is_pop(attr, debug_ln) == true)
		return true;

	if(is_jmp(table, attr, debug_ln) == true)
		return true;
		
	if(is_nop(attr, debug_ln) == true)
		return true;
		
	if(is_sub(attr, debug_ln) == true)
		return true;

	if(is_cmp(attr, debug_ln) == true)
		return true;

	if(is_je(table, attr, debug_ln) == true)
		return true;

	if(is_int(attr, debug_ln) == true)
		return true;

	/* Invalid Instruction */
	return false;
}

string get_label(string line) {

	int pos;

	if((pos = line.find(":")) == string::npos)
		return "";

	return line.substr(0, pos);
}












