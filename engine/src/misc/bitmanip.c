/* Bitmanipulation Implementation */

// Project Headers (engine)
#include "engine/include/misc/bitmanip.h"

int vmcu_extr(const int opcode, int from, int to, int offs) {

    int res = 0;

    for(int i = from; i < to; i++) {

        const int bit = (((0x01 << i) & opcode) >> i);
        res += (bit << (i - from) + offs);
    }

    return res;
}
