#ifndef VMCU_RC_H
#define VMCU_RC_H

typedef enum {

    VMCU_RC_NONE = -1,   // no return code
    VMCU_RC_OK,          // no error. indicates success
    VMCU_RC_FAIL,        // generic error code
    VMCU_RC_ERR_PARAM,   // function called with invalid parameters (generic)
    VMCU_RC_ERR_OP_SRC,  // error related to source operand
    VMCU_RC_ERR_OP_DEST, // error related to destination operand
    VMCU_RC_ERR_IID,     // error related to instruction IDs
    VMCU_RC_ENUM_END     // end of enum

} vmcu_rc_t;

const char* vmcu_strerror(const vmcu_rc_t rc);

#endif
