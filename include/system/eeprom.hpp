/* EEPROM Header */

#ifndef EEPROM_HPP
#define EEPROM_HPP

#include <inttypes.h>

class DebugWindow;

class Eeprom {

public:
    Eeprom(void);                                           // EEPROM Constructor
    ~Eeprom(void);                                          // EEPROM Destructor

    void write(const int addr, const int8_t value);         // writing to EEPROM
    int8_t read(const int addr);                            // reading to EEPROM

    void get_coi(std::tuple <int, int> & buffer);           // getting current coi
    void dump(std::vector <int8_t> & buffer);               // dumping EEPROM memory into 'buffer'

private:
    int8_t *memory;                                         // EEPROM memory block
    std::tuple <int, int> coi;                              // EEPROM cell of interest

    void set_coi(const int cell, const int prop);           // setting color of cell
    void clear_coi(void);                                   // clear all cell colors
};

#endif
