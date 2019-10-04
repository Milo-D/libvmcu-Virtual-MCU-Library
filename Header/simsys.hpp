/* Simulated System Header */

#ifndef SIMSYS_HPP
#define SIMSYS_HPP

// C Headers
#include <inttypes.h>

#define EAX 0
#define EBX 1
#define ECX 2
#define EDX 3
#define ESP 4
#define EDI 5
#define ESI 6
#define EBP 7

#define CF 0
#define ZF 1
#define SF 2
#define OF 3
#define PF 4
#define IF 5
#define AF 6
#define BF 7

#define REG_NO 8
#define FLAG_NO 8

extern const std::string registers[REG_NO];

class SimSys {

public:
	SimSys(unsigned int heap_size, unsigned int stack_size);
	~SimSys(void);

	bool is_popable(void);
	int push_stack(int data);
	int pop_stack(void);
	int read_reg(std::string reg_string);
	void write_reg(std::string reg_string, int data);
	uint8_t read_flag(uint8_t flag_id);
	void set_flag(uint8_t flag_id, bool status);
	void reset_col(int reg_id);
	void print_stack(int row);
	void print_reg(void);
	void print_flag(void);
	void exit(void);

	bool terminated;

private:
	void scale_stack(void);

	int *heap;
	int *stack;
	int reg[8];
	uint8_t flag;
	int off_top, off_bottom;
	unsigned int stack_end;
	std::string reg_col[8];
	std::string flag_col[8];
};

#endif