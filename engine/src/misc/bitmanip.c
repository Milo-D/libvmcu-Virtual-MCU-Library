/* Bitmanipulation Implementation */

// Project Headers (engine)
#include "engine/include/misc/bitmanip.h"

uint32_t vmcu_extr(const uint32_t opcode, uint8_t from, uint8_t to, uint8_t offs) {

    uint32_t res = 0;

    for(uint8_t i = from; i < to; i++) {

        const uint8_t bit = (((0x01 << i) & opcode) >> i);
        res += (bit << ((i - from) + offs));
    }

    return res;
}
