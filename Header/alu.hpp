/* ALU Header */

#ifndef ALU_HPP
#define ALU_HPP

#include <inttypes.h>

class Sys;
class Flash;
class Gpr;
class Sreg;

class Alu {

public:
    Alu(std::string asm_file);
    ~Alu();
		
    int fetch(Sys *sys);                        // fetching next instruction
	
    void write_gpr(int rx, int8_t data);        // writing to GPR
    int8_t read_gpr(int rx);                    // reading GPR
    std::string get_gpr(int cursor);            // get GPR output string

    void write_sreg(int flag, bool bit);        // writing to SREG
    bool read_sreg(int flag);                   // reading SREG
    std::string get_sreg(void);                 // get SREG output string

private:
    Flash *flash;
    Gpr *gpr;
    Sreg *sreg;
};

#endif
