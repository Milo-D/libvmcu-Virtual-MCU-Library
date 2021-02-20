/* Plain (Data-Structure) Header */

#ifndef VMCU_PLAIN_H
#define VMCU_PLAIN_H

// C Headers
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/ikey.h"
#include "engine/include/analyzer/report/operand.h"

/*
*  plain_t is the main data-structure
*  for the analyzer pipeline.
*/

typedef struct vmcu_plain {

    int opcode;
    int addr;

    bool exec;
    bool dword;

    char *mnem;

    VMCU_IKEY key;

    vmcu_operand_t src;
    vmcu_operand_t dest;

} vmcu_plain_t;

#endif
