/* MCU Architecture Header */

#ifndef MCU_HPP
#define MCU_HPP

#ifdef ATMEGA32

    #define SYSTEM "ATmega32"

    #define GPR_SIZE 32
    #define SREG_SIZE 8
    #define SFR_SIZE 64
    #define SRAM_SIZE 2048
    #define EEPROM_SIZE 1024
    #define FLASH_SIZE 16384    /* 16384 x 16-bit */
    #define SRAM_START 0x0060
    #define RAM_END 0x085f

    #define IF 7
    #define TF 6
    #define HF 5
    #define SF 4
    #define VF 3
    #define NF 2
    #define ZF 1
    #define CF 0

    const std::string flags[SREG_SIZE] = { "CF", "ZF", "NF", "VF", "SF", "HF", "TF", "IF" };

#endif

#ifdef ATMEGA16


#endif

#ifdef ATMEGA8


#endif


#endif