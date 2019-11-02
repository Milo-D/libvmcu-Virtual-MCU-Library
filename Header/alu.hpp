/* ALU Header */

#ifndef ALU_HPP
#define ALU_HPP

#include <inttypes.h>

class Sys;
class Flash;
class Gpr;

class Alu {

public:
	Alu(std::string asm_file);
	~Alu();
		
	int fetch(Sys *sys);						// fetching next instruction
	
	void write_reg(int rx, int8_t data);		// writing to register
	int8_t read_reg(int rx);					// reading register
	std::string get_gpr(int cursor);			// get gpr output string

private:
	Flash *flash;
	Gpr *gpr;
};

#endif
