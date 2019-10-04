/* Instruction Attributes Header */

#ifndef ATTRIBUTES_HPP
#define ATTRIBUTES_HPP

struct attributes {

	int instr;
	std::string dest;
	std::string src;
	std::string label;

	/* Following attributes are only set for Interrupts */
	int int_id;
};

#endif