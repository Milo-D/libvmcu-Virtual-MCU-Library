/* SRAM Class Header */

#ifndef SRAM_HPP
#define SRAM_HPP

#include <inttypes.h>

class Sram {

public:
    Sram(void);                                 // Constructor of SRAM
    ~Sram();                                    // SRAM Destructor

private:
    int8_t *ram;
};

#endif
