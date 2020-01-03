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
    ~Alu(void);                                 // ALU Destructor

    int fetch(Sys *sys);                        // fetching next instruction
    int get_pc(void);                           // returning program counter
    void set_pc(int addr);                      // setting program counter on 'addr'
	
    void write_gpr(int rx, int8_t data);        // writing to GPR
    int8_t read_gpr(int rx);                    // reading GPR
    void scale_gpr(int offs);                   // scaling GPR file
    std::string get_gpr(void);                  // get GPR output string

    void write_sreg(int flag, bool bit);        // writing to SREG
    bool read_sreg(int flag);                   // reading SREG
    std::string get_sreg(void);                 // get SREG output string

    std::string get_table(void);                // get TABLE output string

private:
    Flash *flash;                               // FLASH Module
    Gpr *gpr;                                   // GPR Module
    Sreg *sreg;                                 // SREG Module
};

#endif
