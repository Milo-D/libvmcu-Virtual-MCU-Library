/* Bitmanipulation Header */

#ifndef VMCU_BITMANIP_H
#define VMCU_BITMANIP_H

#define sp(spl, sph) ((sph << 8) + spl)
#define spl(sp) (((0x01 << 8) - 1) & sp)
#define sph(sp) (((((0x01 << 8) - 1) << 8) & sp) >> 8)

#define bit(value, i) (((0x01 << (i)) & value) >> (i))
#define comp(v, b) ((v ^ ((0x01 << b) - 1)) + 0x01)

#define setbit(value, i) value |= (0x01 << (i))
#define clearbit(value, i) value &= ~(0x01 << (i))

#define big_endian32(b) (((b & 0x000000ff)) << 8) |   \
                        (((b & 0x0000ff00)) >> 8) |   \
                        (((b & 0x00ff0000)) << 8) |   \
                        (((b & 0xff000000)) >> 8)     \

#define big_endian16(b) (((b & 0x00ff)) << 8) |       \
                        (((b & 0xff00)) >> 8)         \

#define low16(v)   (v & 0x0000ffff)
#define high16(v) ((v & 0xffff0000) >> 16)

extern int extract(const int opcode, const int from, const int to, const int offs);

#endif
