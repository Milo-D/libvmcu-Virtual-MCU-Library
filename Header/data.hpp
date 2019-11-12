/* AVR Data Memory Header */

#ifndef DATA_HPP
#define DATA_HPP

// C Header
#include <inttypes.h>

class Data {

public:
    Data(void);                                     // DATA Constructor
    ~Data(void);                                    // DATA Destructor

    void push(int8_t value);                        // pushing 'value' on stack
    int8_t pop(void);                               // popping top of stack

    void write(int addr, int8_t value);             // writing to DATA memory at 'addr'
    int8_t read(int addr);                          // reading 'addr' from DATA

    void scale(int offs);                           // scaling DATA Memory
    std::string to_str(void);                       // returning a printable DATA string

private:
    int8_t *memory;                                 // 8-bit DATA Field
    unsigned int cursor;                            // DATA cursor
};

#endif