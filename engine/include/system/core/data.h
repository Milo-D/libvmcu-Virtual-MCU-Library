/* Data Memory Header */

#ifndef VMCU_DATA_H
#define VMCU_DATA_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_io vmcu_io_t;

typedef struct vmcu_data {
   
    vmcu_io_t *io;
    int8_t *memory;
    
} vmcu_data_t;

extern vmcu_data_t* vmcu_data_ctor(void);
extern void vmcu_data_dtor(vmcu_data_t *this);

/* General vmcu_data Operations */

extern void vmcu_data_push(vmcu_data_t *this, const int8_t value);
extern int8_t vmcu_data_pop(const vmcu_data_t *this);
extern void vmcu_data_write(vmcu_data_t *this, const uint16_t addr, const int8_t value);
extern int8_t vmcu_data_read(const vmcu_data_t *this, const uint16_t addr);
extern int8_t* vmcu_data_dump(const vmcu_data_t *this);
extern void vmcu_data_reboot(const vmcu_data_t *this);

/* IO/SFR Operations */

extern void vmcu_data_update_io(const vmcu_data_t *this, const uint32_t cpu_clk, const uint64_t dc);
extern int vmcu_data_check_irq(const vmcu_data_t *this);

extern void vmcu_data_set_sfr(vmcu_data_t *this, const uint16_t addr, const int bit);
extern void vmcu_data_clear_sfr(vmcu_data_t *this, const uint16_t addr, const int bit);

/* EEPROM Operations */

extern int8_t* vmcu_data_dump_eeprom(const vmcu_data_t *this);

#endif
