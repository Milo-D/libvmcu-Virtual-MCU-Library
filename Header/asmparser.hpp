/* Assembler Line Parser */

#ifndef ASMPARSER_HPP
#define ASMPARSER_HPP

#define SKIP 0
#define MOV 1
#define PUSH 2
#define POP 3
#define JMP 4
#define NOP 5
#define JLE 6
#define SUB 7
#define CMP 8
#define JE 9
#define INT 10

class Table;
struct attributes;

bool parse_ln(Table *table, struct attributes *attr, int line);
std::string get_label(std::string line);

#endif
