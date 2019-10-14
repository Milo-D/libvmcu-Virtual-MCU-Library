/* (G)eneral (P)urpose (R)egisters Header */

#ifndef GPR_HPP
#define GPR_HPP

// C Headers
#include <inttypes.h>

#define REG_NO 32

class Gpr {

public:
	Gpr(unsigned int amount, int8_t init_val);
	~Gpr();

	int8_t read(int8_t rx);
	void write(int rx, int8_t data);	

private:
	void clear_color(void);

	int8_t *reg;
	int reg_size;
	std::string *color;
};

#endif
