/* AVR Flash Class */

#ifndef FLASH_HPP
#define FLASH_HPP

// C++ Headers
#include <vector>

// C Headers
#include <inttypes.h>

class Table;

class Flash {

public:
    Flash(Table *table);                            // FLASH Constructor
    ~Flash(void);                                   // FLASH Destructor

    int load_opcode(void);                          // loading next opcode
    int load_key(void);                             // loading next key
    void pc_next(void);                             // incrementing program counter
    int pc_get(void);                               // getting program counter
    void pc_set(int addr);                          // setting program counter on 'addr'

    int table_step(void);                           // single stepping through table
    void table_set_tip(int instr_line);             // setting tip to 'instr_line'
    bool table_has_break(void);                     // checking if table has breakpoints
    bool table_is_break(void);                      // checking if current tip line is a breakpoint
    bool table_is_sync(void);                       // returning true if table is synchronized
    int table_size(void);                           // returning size of table
    std::string get_table(void);                    // get TABLE output string

private:
    int16_t *memory;                                // FLASH Memory Block
    std::vector < std::tuple <int, int> > keys;     // Instruction Keys

    int pc;                                         // program counter
    unsigned int mem_usage;                         // size of used FLASH
    
    Table *table;                                   // Origin Table
};

#endif
