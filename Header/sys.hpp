/* System Class Header */

#ifndef SYS_HPP
#define SYS_HPP

#include <inttypes.h>

class Alu;
class Sram;
class Eeprom;

class Sys {

public:
	Sys(std::string asm_file, unsigned int heap_size, unsigned int stack_size);

	/* General System Functions */

	void step(void);
	bool is_terminated(void);

	/* GPR Operations */

	void write_gpr(int rx, int8_t data);
	int8_t read_gpr(int rx);
	std::string gpr_to_str(int cursor);

	/* SREG Operations */

	void write_sreg(int flag, bool bit);
	bool read_sreg(int flag);
	std::string sreg_to_str(void);

private:
	Alu *alu;
	Sram *sram;
	Eeprom *eeprom;

	bool terminated;
};

#endif
