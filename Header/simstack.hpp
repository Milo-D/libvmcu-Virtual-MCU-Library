/* Stack Header */

#ifndef SIMSTACK_HPP
#define SIMSTACK_HPP

class SimStack {

public:
	SimStack(unsigned int size);

	bool is_popable(void);
	bool is_pushable(void);
	void push(int data);
	int pop(void);
	void scale(void);
	void print(int row);

private:
	int *stack;
	int esp;
	int size;
	int top, bottom;
};

#endif