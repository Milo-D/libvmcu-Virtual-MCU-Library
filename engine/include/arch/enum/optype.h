/* Enumeration of Operand Types - Header */

#ifndef VMCU_OPTYPE_H
#define VMCU_OPTYPE_H

typedef enum {

    VMCU_IMM,
    VMCU_IMM8,
    VMCU_UIMM8,
    VMCU_UIMM16,
    VMCU_REGISTER,
    VMCU_REGISTERPAIR,
    VMCU_IODIRECT,
    VMCU_XPTR,
    VMCU_YPTR,
    VMCU_ZPTR,
    VMCU_EMPTY

} VMCU_OPTYPE;

#endif
