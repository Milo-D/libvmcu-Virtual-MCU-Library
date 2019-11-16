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

    /* FLASH Operations */

    int get_pc(void);
    void set_pc(int addr);

    /* DATA Operations */

    void push_stack(int8_t value);
    int8_t pop_stack(void);
    void write_data(int addr, int8_t value);
    int8_t read_data(int addr);
    void scale_data(int offs);
    std::string data_to_str(void);

    /* Table Operations */

    void table_set_tip(int instr_line);
    bool table_has_break(void);
    bool table_is_break(void);
    int table_size(void);
    std::string table_to_str(void);

private:
    Alu *alu;
    Data *data;
    Eeprom *eeprom;

    bool terminated;
};

#endif
