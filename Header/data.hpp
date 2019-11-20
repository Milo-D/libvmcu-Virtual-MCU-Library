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
    std::vector <std::string> to_vector(void);      // returning a vector of DATA

private:
    int8_t *memory;                                 // 8-bit DATA Field
    
    int cursor;                                     // DATA cursor
    std::tuple <int, std::string> color;            // color tuple (memory cell, color)

    void set_color(int cell, std::string color);    // setting color of memory cell
    void clear_color(void);                         // clearing color of memory cells
};

#endif