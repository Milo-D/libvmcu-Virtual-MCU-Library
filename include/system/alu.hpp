/* ALU Header */

#ifndef ALU_HPP
#define ALU_HPP

#include <inttypes.h>

class DebugWindow;
class Sys;
class Flash;
class Gpr;
class Sreg;
class Table;

class Alu {

public:
    Alu(Table *table);                                  // ALU Constructor
    ~Alu(void);                                         // ALU Destructor

    int fetch(Sys *sys);                                // fetching next instruction
    int get_pc(void);                                   // returning program counter
    void set_pc(int addr);                              // setting program counter on 'addr'
	
    void write_gpr(int rx, int8_t data);                // writing to GPR
    int8_t read_gpr(int rx);                            // reading GPR
    void scale_gpr(int offs);                           // scaling GPR file
    void put_gpr(DebugWindow *dwin);                    // put GPR Data in 'dwin'

    void write_sreg(int flag, bool bit);                // writing to SREG
    bool read_sreg(int flag);                           // reading SREG
    void put_sreg(DebugWindow *dwin);                   // add SREG data to DebugWindow

    void put_table(DebugWindow *dwin, bool full);       // add Table data to DebugWindow

private:
    Flash *flash;                                       // FLASH Module
    Gpr *gpr;                                           // GPR Module
    Sreg *sreg;                                         // SREG Module
};

#endif
