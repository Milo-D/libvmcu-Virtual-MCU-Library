/* ALU Header */

#ifndef ALU_HPP
#define ALU_HPP

#include <inttypes.h>

class Sys;
class Flash;
class Gpr;
class Sreg;
class Table;

class Alu {

public:
    Alu(Table *table);                          // ALU Constructor
    ~Alu();                                     // ALU Destructor

    int fetch(Sys *sys);                        // fetching next instruction
    int get_pc(void);                           // returning program counter
    void set_pc(int addr);                      // setting program counter on 'addr'
	
    void write_gpr(int rx, int8_t data);        // writing to GPR
    int8_t read_gpr(int rx);                    // reading GPR
    std::string get_gpr(int cursor);            // get GPR output string

    void write_sreg(int flag, bool bit);        // writing to SREG
    bool read_sreg(int flag);                   // reading SREG
    std::string get_sreg(void);                 // get SREG output string

    void table_set_tip(int instr_line);         // setting tip to 'instr_line'
    bool table_has_break(void);                 // checking if table has breakpoints
    bool table_is_break(void);                  // checking if current tip line is a breakpoint
    int table_size(void);                       // returning size of table
    std::string get_table(void);                // get TABLE output string

private:
    Flash *flash;
    Gpr *gpr;
    Sreg *sreg;
};

#endif
