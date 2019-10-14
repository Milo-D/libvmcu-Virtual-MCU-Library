/* AVR Flash Class */

#ifndef FLASH_HPP
#define FLASH_HPP

class Flash {

public:
	Flash(std::string hex_file);

	int inc_pc(void);				// incrementing PC
	int dec_pc(void);				// decrementing PC
	std::string load_instr(void);	// fetching next Instruction

private:
	int pc;							// Program Counter
	unsigned int size_used;			// Size of used Flash
	std::vector <std::string> app;	// Application Section
};

#endif