/* System Class Header */

#ifndef SYS_HPP
#define SYS_HPP

#include <inttypes.h>

class DebugWindow;
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
    void put_sys(DebugWindow *dwin);
    void reboot(Table *table);
    void kill(void);
    bool is_terminated(void);

    /* GPR Operations */

    void write_gpr(int rx, int8_t data);
    int8_t read_gpr(int rx);
    void scale_gpr(int offs);
    void put_gpr(DebugWindow *dwin);

    /* SREG Operations */

    void write_sreg(int flag, bool bit);
    bool read_sreg(int flag);
    void put_sreg(DebugWindow *dwin);

    /* FLASH Operations */

    int get_pc(void);
    void set_pc(int addr);

    /* DATA Operations */

    void push_stack(int8_t value);
    int8_t pop_stack(void);
    void write_data(int addr, int8_t value);
    int8_t read_data(int addr);
    void scale_data(int offs);
    void put_data(DebugWindow *dwin);

    /* EEPROM Operations */

    void write_eeprom(int addr, int8_t value);
    int8_t read_eeprom(int addr);
    void scale_eeprom(int offs);
    void put_eeprom(DebugWindow *dwin);

    /* Table Operations */

    void put_table(DebugWindow *dwin, bool full);

private:
    Alu *alu;
    Data *data;
    Eeprom *eeprom;

    int steps;
    bool terminated;
};

#endif
