/* EEPROM Header */

#ifndef EEPROM_HPP
#define EEPROM_HPP

#include <inttypes.h>

class Eeprom {

public:
    Eeprom(void);                                           // EEPROM Constructor
    ~Eeprom(void);                                          // EEPROM Destructor

    void write(int addr, int8_t value);                     // writing to EEPROM
    int8_t read(int addr);                                  // reading to EEPROM

    void scale(int offs);                                   // scaling EEPROM memory
    std::vector <std::string> to_vector(void);              // returning a vector of EEPROM

private:
    int8_t *memory;                                         // EEPROM memory block

    int cursor;                                             // EEPROM cursor
    std::tuple <int, std::string> color;                    // memory cell color

    void set_color(int cell, std::string color);            // setting color of cell
    void clear_color(void);                                 // clear all cell colors
};

#endif
