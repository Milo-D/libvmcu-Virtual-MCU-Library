/* Double Buffered Register Implementation */

// Project Headers
#include "system/util/buffered_register.h"

void buffered_register_write(struct _buffered_register *this, const int8_t value) {

    this->buffer = value;
}

int buffered_register_update(struct _buffered_register *this) {

    if(*(this->cmp_with) != this->condition)
        return REGISTER_NOT_WRITTEN;

    *(this->reg) = this->buffer;
    return REGISTER_WRITTEN;
}