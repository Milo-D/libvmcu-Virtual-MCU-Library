#include <lib/rc.h>
#include <stddef.h>

static const char* const error_map[] = {

    [VMCU_RC_OK]          = "Ok",
    [VMCU_RC_FAIL]        = "Generic error",
    [VMCU_RC_ERR_PARAM]   = "Invalid function parameter",
    [VMCU_RC_ERR_OP_SRC]  = "Invalid source operand",
    [VMCU_RC_ERR_OP_DEST] = "Invalid destination operand",
    [VMCU_RC_ERR_IID]     = "Invalid instruction id"
};

const char* vmcu_strerror(const vmcu_rc_t rc) {

    if (rc <= VMCU_RC_ENUM_START || rc >= VMCU_RC_ENUM_END)
        return NULL;

    return error_map[rc];
}


