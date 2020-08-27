/* Bitmanipulation Header */

#ifndef BITMANIP_H
#define BITMANIP_H

#define bit(value, i) (((0x01 << i) & value) >> i)
#define comp(v, b) ((offs ^ ((0x01 << b) - 1)) + 0x01)

extern int extract(const int opcode, const int from, const int to, const int offs);

#endif