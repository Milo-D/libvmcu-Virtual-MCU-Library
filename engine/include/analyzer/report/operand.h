/* Operand Structure Header */

#ifndef VMCU_OPERAND_H
#define VMCU_OPERAND_H

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

typedef struct vmcu_operand {
    
    int32_t value;
    VMCU_OPTYPE type;
    
} vmcu_operand_t;

#endif
