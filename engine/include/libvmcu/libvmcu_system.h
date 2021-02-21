/* libvmcu - System Library Interface */

#ifndef LIBVMCU_SYSTEM_INTERFACE_H
#define LIBVMCU_SYSTEM_INTERFACE_H

/*
 * libvmcu - Virtual Microcontroller Library
 *
 * Copyright (C) 2021 David Milosevic - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPLv2 license.
 *
 * You should have received a copy of the GPLv2 license with
 * this file. If not, please contact me.
 *
 * Email: David.Milosevic@web.de
 *
 * */

/* <------------------------------------------- Structures --------------------------------------------> */

typedef struct vmcu_gpr   vmcu_gpr_t;         ///< general purpose register (forward declared) (hidden)
typedef struct vmcu_sreg  vmcu_sreg_t;        ///< status register (forward declared) (hidden)
typedef struct vmcu_flash vmcu_flash_t;       ///< FLASH memory (forward declared) (hidden)
typedef struct vmcu_data  vmcu_data_t;        ///< data space memory (forward declared) (hidden)

typedef struct vmcu_progmem {                 ///< program memory single

    int opcode;                               ///< 16-bit opcode (todo: change type to uint16)
    int addr;                                 ///< address of opcode (todo: change type to uint16)
    int key;                                  ///< numerical instruction identifier

    bool exec;                                ///< instruction executable ?
    bool dword;                               ///< 32-bit instruction ?

} vmcu_progmem_t;

typedef struct vmcu_system {                  ///< virtual microcontroller

    vmcu_gpr_t *gpr;                          ///< general purpose registers
    vmcu_sreg_t *sreg;                        ///< status register

    vmcu_flash_t *flash;                      ///< FLASH memory
    vmcu_data_t *data;                        ///< data space memory

    uint64_t cycles;                          ///< cycle counter
    uint32_t clock;                           ///< cpu clock

    int steps;                                ///< step counter

} vmcu_system_t;

/* <------------------------------------------- Functions ---------------------------------------------> */

/*
 * vmcu_system_ctor - initialize virtual system
 * @report:   analyzer report (see libvmcu_analyzer.h)
 * */
extern vmcu_system_t* vmcu_system_ctor(const vmcu_report_t *report);

/*
 * vmcu_system_dtor - destroy virtual system
 * @this:   pointer to virtual system
 * */
extern void vmcu_system_dtor(vmcu_system_t *this);

/*
 * vmcu_system_step - single step
 * @this:   pointer to virtual system
 * */
extern int vmcu_system_step(vmcu_system_t *this);

/*
 * vmcu_system_backstep - single step back
 * @this:   pointer to virtual system
 * */
extern void vmcu_system_backstep(vmcu_system_t *this);

/*
 * vmcu_system_reboot - reboot virtual system
 * @this:   pointer to virtual system
 * */
extern void vmcu_system_reboot(vmcu_system_t *this);

/*
 * vmcu_system_write_gpr - write byte to register
 * @this:   pointer to virtual system
 * @rx:     register Rx { 0, 1, ..., 31 }
 * @data:   byte to write
 * */
extern void vmcu_system_write_gpr(vmcu_system_t *this, const int rx, const int8_t data);

/*
 * vmcu_system_read_gpr - read byte from register
 * @this:   pointer to virtual system
 * @rx:     register Rx { 0, 1, ..., 31 }
 * */
extern int8_t vmcu_system_read_gpr(const vmcu_system_t *this, const int rx);

/*
 * vmcu_system_dump_gpr - dump register contents
 * @this:   pointer to virtual system
 * */
extern int8_t* vmcu_system_dump_gpr(const vmcu_system_t *this);

/*
 * vmcu_system_write_sreg - write bit to status register
 * @this:   pointer to virtual system
 * @flag:   flag position { 0, 1, ..., 7 }
 * @bit:    bit { 0, 1 }
 * */
extern void vmcu_system_write_sreg(vmcu_system_t *this, const int flag, const bool bit);

/*
 * vmcu_system_read_sreg - read bit from status register
 * @this:   pointer to virtual system
 * @flag:   flag position { 0, 1, ..., 7 }
 * */
extern bool vmcu_system_read_sreg(const vmcu_system_t *this, const int flag);

/*
 * vmcu_system_dump_sreg - dump status register contents
 * @this:   pointer to virtual system
 * */
extern uint8_t vmcu_system_dump_sreg(const vmcu_system_t *this);

/*
 * vmcu_system_read_progmem - read program memory from FLASH
 * @this:   pointer to virtual system
 * @addr:   FLASH address { 0, 1, ..., FLASH_SIZE - 1 }
 * */
extern vmcu_progmem_t* vmcu_system_read_progmem(const vmcu_system_t *this, const int addr);

/*
 * vmcu_system_read_flash - read opcode from FLASH
 * @this:   pointer to virtual system
 * @addr:   FLASH address { 0, 1, ..., FLASH_SIZE - 1 }
 * */
extern uint16_t vmcu_system_read_flash(const vmcu_system_t *this, const int addr);

/*
 * vmcu_system_move_pc - move program counter (relative)
 * @this:   pointer to virtual system
 * @inc:    relative offset
 * */
extern void vmcu_system_move_pc(const vmcu_system_t *this, const int inc);

/*
 * vmcu_system_set_pc - set program counter (absolute)
 * @this:   pointer to virtual system
 * @addr:   FLASH address { 0, 1, ..., FLASH_SIZE - 1 }
 * */
extern void vmcu_system_set_pc(vmcu_system_t *this, const int addr);

/*
 * vmcu_system_get_pc - get current program counter
 * @this:   pointer to virtual system
 * */
extern int vmcu_system_get_pc(const vmcu_system_t *this);

/*
 * vmcu_system_push_stack - push byte onto stack
 * @this:   pointer to virtual system
 * @value:  byte { 0, 1, ..., 255 }
 * */
extern void vmcu_system_push_stack(vmcu_system_t *this, const int8_t value);

/*
 * vmcu_system_pop_stack - pop byte from stack
 * @this:   pointer to virtual system
 * */
extern int8_t vmcu_system_pop_stack(const vmcu_system_t *this);

/*
 * vmcu_system_write_data - write byte to data space
 * @this:   pointer to virtual system
 * @addr:   data space address { 0, 1, ..., RAM_END - 1 }
 * @value:  byte to write { 0, 1, ..., 255 }
 * */
extern void vmcu_system_write_data(vmcu_system_t *this, const uint16_t addr, const int8_t value);

/*
 * vmcu_system_read_data - read byte from data space
 * @this:   pointer to virtual system
 * @addr:   data space address { 0, 1, ..., RAM_END - 1 }
 * */
extern int8_t vmcu_system_read_data(const vmcu_system_t *this, const uint16_t addr);

/*
 * vmcu_system_dump_data - dump data space contents
 * @this:   pointer to virtual system
 * */
extern int8_t* vmcu_system_dump_data(const vmcu_system_t *this);

/*
 * vmcu_system_set_sfr - set bit in I/O space
 * @this:   pointer to virtual system
 * @addr:   data space address { 0, 1, ..., RAM_END - 1 }
 * @bit:    bit { 0, 1, ..., 7 }
 * */
extern void vmcu_system_set_sfr(vmcu_system_t *this, const uint16_t addr, const int bit);

/*
 * vmcu_system_clear_sfr - clear bit in I/O space
 * @this:   pointer to virtual system
 * @addr:   data space address { 0, 1, ..., RAM_END - 1 }
 * */
extern void vmcu_system_clear_sfr(vmcu_system_t *this, const uint16_t addr, const int bit);

/*
 * vmcu_system_dump_eeprom - dump eeprom contents
 * @this:   pointer to virtual system
 * */
extern int8_t* vmcu_system_dump_eeprom(const vmcu_system_t *this);

#endif
