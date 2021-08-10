/* Progmem (Data-Structure) Header */

#ifndef VMCU_PROGMEM_H
#define VMCU_PROGMEM_H

// C Headers
#include <stdbool.h>

typedef struct vmcu_progmem {

    uint16_t opcode;
    uint32_t addr;
    int32_t key;
    
    bool exec;
    bool dword;
    
} vmcu_progmem_t;

#endif
