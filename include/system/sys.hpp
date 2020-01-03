/* System Class Header */

#ifndef SYS_HPP
#define SYS_HPP

#include <inttypes.h>

class Alu;
class Data;
class Eeprom;
class Table;

class Sys {

public:
    Sys(Table *table);
    ~Sys(void);

    /* General System Functions */

    void step(void);
    void backstep(Table *table);
    void kill(void);
    bool is_terminated(void);

    /* GPR Operations */

    void write_gpr(int rx, int8_t data);
    int8_t read_gpr(int rx);
    void scale_gpr(int offs);
    std::string gpr_to_str(void);

    /* SREG Operations */

    void write_sreg(int flag, bool bit);
    bool read_sreg(int flag);
    std::string sreg_to_str(void);

    /* FLASH Operations */

    int get_pc(void);
    void set_pc(int addr);

    /* DATA Operations */

    void push_stack(int8_t value);
    int8_t pop_stack(void);
    void write_data(int addr, int8_t value);
    int8_t read_data(int addr);
    void scale_data(int offs);

    /* EEPROM Operations */

    void write_eeprom(int addr, int8_t value);
    int8_t read_eeprom(int addr);
    void scale_eeprom(int offs);

    /* DATA and EEPROM Operations */

    std::string memory_to_str(void);

    /* Table Operations */

    std::string table_to_str(void);

private:
    Alu *alu;
    Data *data;
    Eeprom *eeprom;

    int steps;
    bool terminated;
};

#endif
