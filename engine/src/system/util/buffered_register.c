/* Double Buffered Register Implementation */

// Project Headers (engine)
#include "engine/include/system/util/buffered_register.h"

void vmcu_buffered_register_write(vmcu_buffered_register_t *this, const int8_t value) {

    this->buffer = value;
}

int buffered_register_update(vmcu_buffered_register_t *this) {

    if(*(this->cmp_with) != this->condition)
        return VMCU_BUFFERED_REGISTER_NOT_WRITTEN;

    *(this->reg) = this->buffer;
    return VMCU_BUFFERED_REGISTER_WRITTEN;
}