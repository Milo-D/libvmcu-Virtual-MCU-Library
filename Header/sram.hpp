/* SRAM Class Header */

#ifndef SRAM_HPP
#define SRAM_HPP

class Sram {

public:
	Sram(unsigned int size);
	~Sram();

private:
	int *ram;
};

#endif
