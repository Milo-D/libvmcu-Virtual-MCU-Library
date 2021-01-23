/* Operand Structure Header */

#ifndef OP_H
#define OP_H

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

typedef struct _op {
    
    int value;
    OPTYPE type;
    
} op_t;

#endif
