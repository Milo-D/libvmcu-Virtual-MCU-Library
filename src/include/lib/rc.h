#ifndef VMCU_RC_H
#define VMCU_RC_H

typedef enum {

    VMCU_RC_ERR_IID     = -5, // error related to instruction IDs
    VMCU_RC_ERR_OP_DEST = -4, // error related to destination operand
    VMCU_RC_ERR_OP_SRC  = -3, // error related to source operand
    VMCU_RC_ERR_PARAM   = -2, // function called with invalid parameters (generic)
    VMCU_RC_FAIL        = -1, // generic error code
    VMCU_RC_OK          =  0  // no error. indicates success

} vmcu_rc_t;

#endif
