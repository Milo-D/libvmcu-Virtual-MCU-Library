/* Status Register Class */

#ifndef SREG_HPP
#define SREG_HPP

// C++ Headers
#include <vector>
#include <inttypes.h>

class DebugWindow;

class Sreg {

public:
    Sreg(void);                                         // Constructor of SREG

    void write(const int flag, const bool bit);         // setting 'bit' on 'flag'
    bool read(const int flag);                          // reading 'flag' from SREG
    void clear(void);                                   // clearing SREG

    void get_coi(std::vector <int> & buffer);           // getting coi
    uint8_t dump(void);                                 // dumping Status Register
	
private:
    uint8_t status;                                     // 8 bit Status Register
    std::vector <int> coi;                              // cells of interest

    void clear_coi(void);                               // clearing focus
};

#endif