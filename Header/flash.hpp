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
	int pc;									// Program Counter
	unsigned int size_used;					// Size of used Flash
	std::vector <int> app;					// Raw Application Section
	std::vector <int> key;					// Vector of Instruction Keys 
};

#endif
