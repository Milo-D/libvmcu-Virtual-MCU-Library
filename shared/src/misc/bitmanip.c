/* Bitmanipulation Implementation */

// Project Headers
#include "misc/bitmanip.h"

int extract(const int opcode, int from, int to, int offs) {

    int res = 0;

    for(int i = from; i < to; i++) {

        const int bit = (((0x01 << i) & opcode) >> i);
        res += (bit << (i - from) + offs);
    }

    return res;
}
