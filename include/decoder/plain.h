/* Plain Header */

#ifndef PLAIN_H
#define PLAIN_H

// C Headers
#include <stdbool.h>

/* 
*  plain_t is the main data-structure
*  for the decoder and disassembler.
*/

typedef struct _plain {

    int opcode;
    int addr;
    int key;

    char *mnem;
    
    bool exec;
    bool dword;

} plain_t;

extern void plain_dtor(struct _plain *this);
extern struct _plain* plain_cpy(const struct _plain *src);


#endif