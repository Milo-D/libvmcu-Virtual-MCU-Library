/* Progmem (Data-Structure) Header */

#ifndef VMCU_PROGMEM_H
#define VMCU_PROGMEM_H

// C Headers
#include <stdbool.h>

typedef struct vmcu_progmem {
    
    int opcode;
    int addr;
    int key;
    
    bool exec;
    bool dword;
    
} vmcu_progmem_t;

#endif
