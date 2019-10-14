/* Simulated System Header */

#ifndef SIMSYS_HPP
#define SIMSYS_HPP

// C Headers
#include <inttypes.h>

// Project Headers
#include "simstack.hpp"

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define R8 8
#define R9 9
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define R14 14
#define R15 15
#define R16 16
#define R17 17
#define R18 18
#define R19 19
#define R20 20
#define R21 21
#define R22 22
#define R23 23
#define R24 24
#define R25 25
#define R26 26
#define R27 27
#define R28 28
#define R29 29
#define R30 30
#define R31 31

#define CF 0
#define ZF 1
#define NF 2
#define VF 3
#define SF 4
#define HF 5
#define TF 6
#define IF 7

#define REG_NO 32
#define FLAG_NO 8

extern const std::string registers[REG_NO];

class SimSys {

public:
	SimSys(unsigned int heap_size, unsigned int stack_size);
	~SimSys(void);

	int read_reg(std::string reg_string);
	void write_reg(std::string reg_string, int data);
	int read_flag(uint8_t flag_id);
	void set_flag(uint8_t flag_id, bool status);
	void reset_col(int reg_id);
	void print_reg(int segment);
	void print_flag(void);
	void exit(void);

	SimStack *stack;
	bool terminated;

private:
	int *heap;
	int reg[REG_NO];
	uint8_t flag;
	std::string reg_col[REG_NO];
	std::string flag_col[8];
};

#endif