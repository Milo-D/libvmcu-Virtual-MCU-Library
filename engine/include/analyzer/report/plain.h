/* Plain (Data-Structure) Header */

#ifndef PLAIN_H
#define PLAIN_H

// C Headers
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/ikey.h"
#include "engine/include/analyzer/report/operand.h"

/* 
*  plain_t is the main data-structure
*  for the analyzer pipeline.
*/

typedef struct _plain {

    IKEY key;

    int opcode;
    int addr;
    
    bool exec;
    bool dword;

    char *mnem;

    operand_t src;
    operand_t dest;

} plain_t;

extern void plain_dtor(struct _plain *this);
extern struct _plain* plain_cpy(const struct _plain *src);

#endif
