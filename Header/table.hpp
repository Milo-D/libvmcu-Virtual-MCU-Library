/* Table Header */

#ifndef TABLE_HPP
#define TABLE_HPP

#include <map>
#include <vector>

class Table {

public:
	Table(std::string asm_file);
	
	int set_break(std::string point);
	int unset_break(std::string point);
	void add_label(std::string id, int line);
	int get_label_ln(std::string id);
	void set_instr(int instr_line);
	int get_instr(void);
	void step(void);
	void jump_break(void);
	void refresh(void);
	void print_short(int row);
	void print(void);

	int size;
	std::string src_file;
	std::vector <std::string> line;
	std::vector <std::string> debug_line;
	std::vector <bool> breaks;

private:
	int instr;
	std::map <std::string, int> label;
};

Table* create_table(std::vector <std::string> asm_file, int amount);
void refresh_all(Table *table, int amount);

#endif
