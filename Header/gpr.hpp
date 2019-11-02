/* (G)eneral (P)urpose (R)egisters Header */

#ifndef GPR_HPP
#define GPR_HPP

// C Headers
#include <inttypes.h>

#define REG_NO 32

class Gpr {

public:
	Gpr(unsigned int amount, int8_t init_val);	// gpr constructor
	~Gpr();										// default gpr destructor

	int8_t read(int rx);						// reading value from gpr 'rx'
	void write(int rx, int8_t data);			// writing 'data' to gpr 'rx'
	std::string to_str(int cursor);				// returning printable gpr string

private:
	void clear_color(void);						// clearing gpr output colors

	int8_t *reg;								// gpr file
	std::string color[32];						// associated gpr colors
	
	unsigned int size;							// total size of gpr file (size * 8-bit)
};

#endif
