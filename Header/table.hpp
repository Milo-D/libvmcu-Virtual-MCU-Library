/* Table Header */

#ifndef TABLE_HPP
#define TABLE_HPP

#include <map>
#include <vector>

class Table {

public:
    Table(std::string asm_file);
	
    int set_break(std::string point);           // setting breakpoints
    int unset_break(std::string point);         // removing breakpoints
    bool is_break(int line);                    // checks if 'line' is a breakpoint
    bool has_break(void);                       // checks if table has breakpoints
    int get_label_ln(std::string id);           // translating label to line
    void set_tip(int instr_line);               // moving tip to 'instr_line'
    int get_tip(void);                          // getting current tip
    std::string get_content(int line);          // get content at index 'line'
    bool executable(void);                      // returns 'true' if line is executable
    int step(void);                             // single stepping through Table
    int size(void);                             // get table size
    std::string src(void);                      // get origin of source code
    std::string to_str(void);                   // returning a Table String
    std::string center_to_str(void);            // returning a centered Table String

private:
    int tip;                                    // (t)able (i)nstruction (p)ointer
    int table_size;
    int break_counter;
    std::string src_file;								
    std::vector <std::string> content;
    std::vector <bool> breaks;
    std::vector <bool> exec;
    std::map <std::string, int> label;

    void add_label(std::string id, int line);
};

Table* create_table(std::vector <std::string> asm_file, int amount);

#endif
