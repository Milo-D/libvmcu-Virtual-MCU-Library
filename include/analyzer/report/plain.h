/* Plain (Data-Structure) Header */

#ifndef PLAIN_H
#define PLAIN_H

// C Headers
#include <stdbool.h>

// Project Headers
#include "analyzer/report/op.h"

/* 
*  plain_t is the main data-structure
*  for the analyzer pipeline.
*/

typedef struct _plain {

    op_t src, dest;

    int opcode;
    int addr;
    int key;
    
    bool exec;
    bool dword;
    char *mnem;

} plain_t;

extern void plain_dtor(struct _plain *this);
extern struct _plain* plain_cpy(const struct _plain *src);

#endif
