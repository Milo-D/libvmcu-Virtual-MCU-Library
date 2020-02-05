/* Table Header */

#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>

class Table {

public:
    Table(std::string hex_file);                                        // Table Constructor
	
    int step(void);                                                     // single stepping through Table
    int set_break(const std::string & point);                           // setting breakpoints
    int unset_break(const std::string & point);                         // removing breakpoints
    void define(const std::string & alias, const std::string & seq);    // defining 'seq' as 'alias'

    int get_tip(void);                                                  // gettting tip of table
    void set_tip(const int instr_line);                                 // moving tip to 'instr_line'

    void jump(const int exec_addr);                                     // jumping to executable address
    bool is_break(void);                                                // checks if current line is a breakpoint
    bool has_break(void);                                               // checks if table has breakpoints
    bool is_sync(const int hex_addr);                                   // returning true if table is synchronized

    int size(void);                                                     // get table size
    std::string src(void);                                              // get origin of source code

    void dump_content(std::vector <std::string> & buffer);              // dumping content into 'buffer'
    void dump_breakpoints(std::vector <bool> & buffer);                 // dumping breakpoints into 'buffer'

private:
    int tip;                                                            // (t)able (i)nstruction (p)ointer
    int table_size;                                                     // table size
    int break_counter;                                                  // breakpoint counter
    std::string source_file;							                // source of table
    std::vector <bool> breaks;                                          // breakpoint storage
    std::vector < std::tuple <std::string, int> > content;              // actual table
};

void create_table(std::vector <Table> & buffer, std::vector <std::string> hex_file);

#endif
