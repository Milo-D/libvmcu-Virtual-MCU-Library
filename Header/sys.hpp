/* System Class Header */

#ifndef SYS_HPP
#define SYS_HPP

#include <inttypes.h>

class Alu;
class Data;
class Eeprom;

class Sys {

public:
    Sys(std::string asm_file);

    /* General System Functions */

    void step(void);
    void kill(void);
    bool is_terminated(void);

    /* GPR Operations */

    void write_gpr(int rx, int8_t data);
    int8_t read_gpr(int rx);
    std::string gpr_to_str(int cursor);

    /* SREG Operations */

    void write_sreg(int flag, bool bit);
    bool read_sreg(int flag);
    std::string sreg_to_str(void);

    /* DATA Operations */

    void push_stack(int8_t value);
    int8_t pop_stack(void);
    void write_data(int addr, int8_t value);
    int8_t read_data(int addr);
    void scale_data(int offs);
    std::string data_to_str(void);

private:
    Alu *alu;
    Data *data;
    Eeprom *eeprom;

    bool terminated;
};

#endif
