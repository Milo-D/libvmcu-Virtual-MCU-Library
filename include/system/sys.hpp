/* System Class Header */

#ifndef SYS_HPP
#define SYS_HPP

// C++ Headers
#include <vector>
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
    void reboot(Table *table);
    void kill(void);
    bool is_terminated(void);

    /* GPR Operations */

    void write_gpr(const int rx, const int8_t data);
    int8_t read_gpr(const int rx);
    void gpr_coi(std::vector <int> & buffer);
    void dump_gpr(std::vector <int8_t> & buffer);

    /* SREG Operations */

    void write_sreg(const int flag, const bool bit);
    bool read_sreg(const int flag);
    void sreg_coi(std::vector <int> & buffer);
    uint8_t dump_sreg(void);

    /* FLASH Operations */

    int get_pc(void);
    void set_pc(const int addr);

    /* DATA Operations */

    void push_stack(const int8_t value);
    int8_t pop_stack(void);
    void write_data(const int addr, const int8_t value);
    int8_t read_data(const int addr);
    void data_coi(std::tuple <int, int> & buffer);
    void dump_data(std::vector <int8_t> & buffer);

    /* EEPROM Operations */

    void write_eeprom(const int addr, const int8_t value);
    int8_t read_eeprom(const int addr);
    void eeprom_coi(std::tuple <int, int> & buffer);
    void dump_eeprom(std::vector <int8_t> & buffer);

private:
    Alu *alu;
    Data *data;
    Eeprom *eeprom;

    int steps;
    bool terminated;
};

#endif
