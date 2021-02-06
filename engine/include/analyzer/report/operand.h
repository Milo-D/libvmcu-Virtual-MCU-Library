/* Operand Structure Header */

#ifndef OPERAND_H
#define OPERAND_H

typedef enum {
    
    IMM,
    IMM8,
    UIMM8,
    UIMM16, 
    REGISTER,
    REGISTERPAIR,
    IODIRECT,
    XPTR,
    YPTR,
    ZPTR,
    EMPTY
    
} OPTYPE;

typedef struct _operand {
    
    int value;
    OPTYPE type;
    
} operand_t;

#endif
