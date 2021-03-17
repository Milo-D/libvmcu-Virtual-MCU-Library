/* Enumeration of Operand Types - Header */

#ifndef VMCU_OP_H
#define VMCU_OP_H

typedef enum {

    VMCU_OP_IMM,
    VMCU_OP_IMM8,
    VMCU_OP_UIMM8,
    VMCU_OP_UIMM16,
    VMCU_OP_REGISTER,
    VMCU_OP_REGISTERPAIR,
    VMCU_OP_IODIRECT,
    VMCU_OP_XPTR,
    VMCU_OP_YPTR,
    VMCU_OP_ZPTR,
    VMCU_OP_NONE

} VMCU_OP;

#endif
