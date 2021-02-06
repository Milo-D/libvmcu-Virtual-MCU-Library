/* Progmem (Data-Structure) Header */

#ifndef PROGMEM_H
#define PROGMEM_H

// C Headers
#include <stdbool.h>

typedef struct _progmem {
    
    int opcode;
    int addr;
    int key;
    
    bool exec;
    bool dword;
    
} progmem_t;

#endif
