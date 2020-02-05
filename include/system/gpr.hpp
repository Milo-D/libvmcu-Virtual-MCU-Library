/* (G)eneral (P)urpose (R)egisters Header */

#ifndef GPR_HPP
#define GPR_HPP

// C++
#include <vector>

// C Headers
#include <inttypes.h>

class DebugWindow;

class Gpr {

public:
    Gpr(void);                                              // GPR constructor
    ~Gpr(void);                                             // GPR destructor

    int8_t read(const int rx);                              // reading value from GPR 'rx'
    void write(const int rx, const int8_t data);            // writing 'data' to GPR 'rx'

    void get_coi(std::vector <int> & buffer);               // get current cells of interest
    void dump(std::vector <int8_t> & buffer);               // dumping GPR content into 'buffer'

private:
    int8_t *reg;                                            // GPR file
    int size;                                               // total size of GPR file (size * 8-bit)
    std::vector <int> coi;                                  // cell of interests

    void clear_coi(void);                                   // clearing coi
};

#endif
