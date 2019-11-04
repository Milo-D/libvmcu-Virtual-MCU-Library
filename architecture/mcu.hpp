/* MCU Architecture Header */

#ifndef MCU_HPP
#define MCU_HPP

#ifdef ATMEGA32

	#define GPR_SIZE 32
	#define SREG_SIZE 8
	#define SFR_SIZE 64
	#define SRAM_SIZE 2048
	#define EEPROM_SIZE 1024
	#define FLASH_SIZE 16384	/* 16384 x 16-bit */
	#define SRAM_START 0x0060

	#define IF 0
	#define TF 1
	#define HF 2
	#define SF 3
	#define VF 4
	#define NF 5
	#define ZF 6
	#define CF 7

	extern const char flags[SREG_SIZE] = { 'I', 'T', 'H', 'S', 'V', 'N', 'Z', 'C' };

#endif

#ifdef ATMEGA16


#endif

#ifdef ATMEGA8


#endif




#endif