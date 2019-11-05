/* EEPROM Header */

#ifndef EEPROM_HPP
#define EEPROM_HPP

class Eeprom {

public:
	Eeprom(void);
	~Eeprom();

private:
	int *data;
	unsigned int size;
};

#endif
