/* Bitmanipulation Header */

#ifndef BITMANIP_H
#define BITMANIP_H

#define sp(spl, sph) ((sph << 8) + spl)
#define spl(sp) (((0x01 << 8) - 1) & sp)
#define sph(sp) (((((0x01 << 8) - 1) << 8) & sp) >> 8)

#define bit(value, i) (((0x01 << (i)) & value) >> (i))
#define comp(v, b) ((offs ^ ((0x01 << b) - 1)) + 0x01)

#define setbit(value, i) value |= (0x01 << (i))
#define clearbit(value, i) value &= ~(0x01 << (i))

extern int extract(const int opcode, const int from, const int to, const int offs);

#endif
