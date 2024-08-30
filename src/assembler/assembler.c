#include <stddef.h>
#include <stdarg.h>

#include <lib/assembler.h>
#include <lib/engine.h>
#include <lib/rc.h>
#include "asm_table.h"

unsigned int vmcu_asm(uint16_t* write_ptr, const vmcu_iid_t iid, ...) {

    va_list            operands;
    vmcu_operand_t*    op_ptr[2];
    asm_table_entry_t* entry;
    unsigned int       n_assembled_words;
    vmcu_rc_t          rc;

    n_assembled_words = 0;
    va_start(operands, iid);

    if ((entry = get_asm_table_entry(iid)) == NULL)
        return n_assembled_words;

    if (vmcu.assembler.expect_operand_structs == true) {

        op_ptr[0] = op_ptr[1] = NULL;

        for (unsigned int i = 0; i < entry->n_operands; i++)
            op_ptr[i] = va_arg(operands, vmcu_operand_t*); 

        rc = entry->handler(write_ptr, op_ptr[0], op_ptr[1]);

    } else {

        rc = entry->handler_unpacked(write_ptr, operands);
    }

    if (rc == VMCU_RC_OK)
        n_assembled_words = entry->n_words;

    va_end(operands);
    return n_assembled_words;
}
