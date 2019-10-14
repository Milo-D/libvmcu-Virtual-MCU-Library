/* Header for Binary Assembler Instructions */

#ifndef BINARY_HPP
#define BINARY_HPP

struct attributes;

bool bin_op_inc(struct attributes *attr, std::string line);
bool bin_op_dec(struct attributes *attr, std::string line);
bool bin_op_std(struct attributes *attr, std::string line);

#endif