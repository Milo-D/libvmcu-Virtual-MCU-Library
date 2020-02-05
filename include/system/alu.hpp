/* ALU Header */

#ifndef ALU_HPP
#define ALU_HPP

// C++ Headers
#include <vector>
#include <inttypes.h>

class Sys;
class Flash;
class Gpr;
class Sreg;
class Table;

class Alu {

public:
    Alu(Table *table);                                      // ALU Constructor
    ~Alu(void);                                             // ALU Destructor

    int fetch(Sys *sys);                                    // fetching next instruction
    int get_pc(void);                                       // returning program counter
    void set_pc(int addr);                                  // setting program counter on 'addr'
	
    void write_gpr(const int rx, const int8_t data);        // writing to GPR
    int8_t read_gpr(const int rx);                          // reading GPR
    void gpr_coi(std::vector <int> & buffer);               // getting current coi of GPR
    void dump_gpr(std::vector <int8_t> & buffer);           // dumping GPR content into 'buffer'

    void write_sreg(const int flag, const bool bit);        // writing to SREG
    bool read_sreg(const int flag);                         // reading SREG
    void sreg_coi(std::vector <int> & buffer);              // getting current coi of SREG
    uint8_t dump_sreg(void);                                // dump SREG content

private:
    Flash *flash;                                           // FLASH Module
    Gpr *gpr;                                               // GPR Module
    Sreg *sreg;                                             // SREG Module
};

#endif
