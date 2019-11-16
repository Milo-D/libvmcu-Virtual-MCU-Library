/* AVR Flash Class */

#ifndef FLASH_HPP
#define FLASH_HPP

// C++ Headers
#include <vector>

class Table;

class Flash {

public:
    Flash(Table *table);
	
    void insert_instr(int instr);           // adding instruction to Application Section
    void insert_key(int key);               // adding instruction key
    int load_instr(void);                   // fetching next Instruction
    int load_key(void);                     // fetching Instruction key
    void pc_next(void);                     // incrementing program counter

    int table_step(void);                   // single stepping through table
    void table_set_tip(int instr_line);     // setting tip to 'instr_line'
    bool table_has_break(void);             // checking if table has breakpoints
    bool table_is_break(void);              // checking if current tip line is a breakpoint
    bool table_is_exec(void);               // checking if current line is an executable line
    int table_size(void);                   // returning size of table
    std::string table_to_str(void);         // get TABLE output string

private:
    int pc;                                 // program counter
    unsigned int size;                      // total FLASH size
    unsigned int size_used;                 // size of used FLASH
    std::vector <int> app;                  // raw application section
    std::vector <int> key;                  // vector of instruction keys 

    Table *table;                           // Origin Table
};

#endif
