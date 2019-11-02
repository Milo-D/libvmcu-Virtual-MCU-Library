/* EEPROM Header */

#ifndef EEPROM_HPP
#define EEPROM_HPP

class Eeprom {

public:
	Eeprom(unsigned int size);
	~Eeprom();

private:
	int *data;	
};

#endif
