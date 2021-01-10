/* PropCol Implementation */

// Project Headers
#include "printer/propcol.h"

COLOR propcol(const MEMPROP prop) {

    COLOR col;

    switch(prop) {

        case SRC:  col = R; break;
        case DEST: col = G; break;

        default:   col = D; break;
    }

    return col;
} 
