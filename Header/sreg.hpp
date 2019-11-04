/* Status Register Class */

#ifndef SREG_HPP
#define SREG_HPP

#include <inttypes.h>

class Sreg {

public:
	Sreg(void);								// Constructor of SREG

	void write(int flag, bool bit);			// setting 'bit' on 'flag'
	bool read(int flag);					// reading 'flag' from SREG
	void clear(void);						// clearing SREG
	std::string to_str(void);				// returning a printable SREG String
	
private:
	int8_t status;							// 8 bit Status Register
	std::string color[8];					// SREG Color	

	void clear_color(void);					// setting color to DEFAULT
};

#endif