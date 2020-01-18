/* Table Header */

#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>

class DebugWindow;

class Table {

public:
    Table(std::string hex_file);
	
    int step(void);                                                     // single stepping through Table
    int set_break(std::string point);                                   // setting breakpoints
    int unset_break(std::string point);                                 // removing breakpoints
    void define(const std::string &alias, const std::string &seq);      // defining 'seq' as 'alias'
    void set_tip(int instr_line);                                       // moving tip to 'instr_line'
    void jump(int exec_addr);                                           // jumping to executable address
    bool is_break(void);                                                // checks if current line is a breakpoint
    bool has_break(void);                                               // checks if table has breakpoints
    bool is_sync(int hex_addr);                                         // returning true if table is synchronized
    int size(void);                                                     // get table size
    void next_page(int offs);                                           // next/previous table page
    std::string src(void);                                              // get origin of source code
    void to_win(DebugWindow *dwin, bool full);                          // adding Table data to DebugWindow

private:
    int tip;                                                            // (t)able (i)nstruction (p)ointer
    int page;                                                           // table page
    int page_size;                                                      // total table pages
    int table_size;                                                     // table size
    int break_counter;                                                  // breakpoint counter
    std::string source_file;							                // source of table
    std::vector <bool> breaks;                                          // breakpoint storage
    std::vector < std::tuple <std::string, int> > content;              // actual table

    void full_to_win(DebugWindow *dwin);                                // adding full Table to Side-Panel
};

Table* create_table(std::vector <std::string> hex_file, int amount);

#endif
