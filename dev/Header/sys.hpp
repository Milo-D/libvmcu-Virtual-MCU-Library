/* System Class Header */

#ifndef SYS_HPP
#define SYS_HPP

#include <inttypes.h>

/* Forward Declaration of System Components */
class Alu;
class Gpr;
class Sram;
class Flash;
class Eeprom;
/* Forward Declaration of System Components */

class Sys {

public:
	Sys(unsigned int heap_size, unsigned int stack_size);

	/* General System Functions */

	void step(void);

	/* Register Operations */
	
	int8_t read_reg(int8_t rx);
	void write_reg(int rx, int8_t data);

private:
	Alu *alu;
	Gpr *gpr;
	Sram *sram;
	Flash *flash;
	Eeprom *eeprom;
};

#endif
