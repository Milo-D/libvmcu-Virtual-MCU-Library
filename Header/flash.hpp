/* AVR Flash Class */

#ifndef FLASH_HPP
#define FLASH_HPP

// C++ Headers
#include <vector>

class Flash {

public:
	Flash(std::string asm_file);
	
	void insert_instr(int instr);			// adding instruction to Application Section
	void insert_key(int key);				// adding instruction key
	int load_instr(void);					// fetching next Instruction
	int load_key(void);						// fetching Instruction key
	void pc_next(void);						// incrementing program counter

private:
	int pc;									// program counter
	unsigned int size;						// total FLASH size
	unsigned int size_used;					// size of used FLASH
	std::vector <int> app;					// raw application section
	std::vector <int> key;					// vector of instruction keys 
};

#endif
