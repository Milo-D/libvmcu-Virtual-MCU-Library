/* AVR Data Memory Header */

#ifndef DATA_HPP
#define DATA_HPP

// C++ Header
#include <vector>
#include <inttypes.h>

class DebugWindow;

class Data {

public:
    Data(void);                                           // DATA Constructor
    ~Data(void);                                          // DATA Destructor

    void push(const int8_t value);                        // pushing 'value' on stack
    int8_t pop(void);                                     // popping top of stack

    void write(const int addr, const int8_t value);       // writing to DATA memory at 'addr'
    int8_t read(const int addr);                          // reading 'addr' from DATA

    void get_coi(std::tuple <int, int> & buffer);         // getting current coi of DATA
    void dump(std::vector <int8_t> & buffer);             // dumping DATA memory into 'buffer'

private:
    int8_t *memory;                                       // 8-bit DATA field
    std::tuple <int, int> coi;                            // DATA cell of interest (coi)

    void set_coi(const int cell, const int prop);         // setting properties 'prop' of 'cell'
    void clear_coi(void);                                 // clearing coi
};

#endif