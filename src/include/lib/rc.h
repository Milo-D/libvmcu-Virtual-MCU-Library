#ifndef VMCU_RC_H
#define VMCU_RC_H

typedef enum {

    VMCU_RC_ERR_PARAM = -2, // function called with invalid parameters
    VMCU_RC_FAIL      = -1, // generic error code
    VMCU_RC_OK        =  0  // no error. indicates success

} vmcu_rc_t;

#endif
