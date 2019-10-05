/* Main Assembly Instructions Header */

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#define INSTR_MAX 11

class Table;
class SimSys;
struct attributes;

extern void (*instructions[INSTR_MAX]) (SimSys *sys, Table *table, struct attributes *attr);

#endif