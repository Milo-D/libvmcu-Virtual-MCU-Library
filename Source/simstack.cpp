/* Stack Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <cstring>

// Project Headers
#include "simstack.hpp"
#include "ehandling.hpp"
#include "style.hpp"

#define UN_INIT 16843009

using namespace std;

SimStack::SimStack(unsigned int size) {

	if((this->stack = (int*) malloc(size * sizeof(int))) == NULL)
		print_status("Fatal, could not allocate Stack Memory", true);

	memset(this->stack, 1, size * sizeof(int));

	this->esp = size - 1;
	this->size = size;

	this->top = this->esp;
	this->bottom = this->esp - ROW;
}

bool SimStack::is_popable(void) {

	if(this->esp == this->size)
		return false;

	return true;	
}

bool SimStack::is_pushable(void) {

	if(this->esp == 0)
		return false;

	return true;
}

void SimStack::push(int data) {

	*(this->stack + this->esp - 1) = data;
	this->esp -= 1;	
}

int SimStack::pop(void) {

	int stack_pointer = this->esp;
	int data = *(this->stack + stack_pointer);

	this->esp += 1;

	return data;
}

void SimStack::scale(void) {

	if(this->esp > this->top) {

		this->top += 1;
		this->bottom -= 1;

		return;
	}

	if(this->esp < this->bottom) {

		this->bottom += 1;
		this->top -= 1;
	}
}

void SimStack::print(int row) {

	cout << "Stack:\n\n";

	if(this->size == 0) {
	
		cout << "[ No Stack available ]\n";
		cout << HORI_SEP;

		return;
	}

	this->scale();

	int stack_offset = this->esp - row;

	for(int i = this->bottom; i < this->top; i++) {

		if(i < 0)
			continue;

		if(i == this->esp)
			cout << BLUE;

		cout << "0x" << hex << (i + 1) << dec << ":\t";

		if(*(this->stack + i) == UN_INIT)
			cout << "---" << "\n" << DEFAULT;
		else
			cout << *(this->stack + i) << "\n" << DEFAULT;
	}

	cout << HORI_SEP;
}

