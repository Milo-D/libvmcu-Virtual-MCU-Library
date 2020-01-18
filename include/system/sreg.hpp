/* Status Register Class */

#ifndef SREG_HPP
#define SREG_HPP

#include <inttypes.h>

class DebugWindow;

class Sreg {

public:
    Sreg(void);                             // Constructor of SREG

    void write(int flag, bool bit);         // setting 'bit' on 'flag'
    bool read(int flag);                    // reading 'flag' from SREG
    void clear(void);                       // clearing SREG
    void to_win(DebugWindow *dwin);         // adding SREG Data to DebugWindow
	
private:
    int8_t status;                          // 8 bit Status Register
    int color[8];                           // SREG Color	

    void clear_color(void);                 // setting color to DEFAULT
};

#endif