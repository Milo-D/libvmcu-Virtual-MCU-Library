/* StringStream Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Project Headers (engine)
#include "engine/include/collections/sstream.h"

/*
 * This small but useful datastructure was inspired by
 * a similar one in the Capstone Engine.
 * 
 * https://github.com/aquynh/capstone/blob/master/SStream.h
 * */

void vmcu_sstream_ctor(vmcu_sstream_t *this) {

    vmcu_sstream_flush(this);
}

void vmcu_sstream_put(vmcu_sstream_t *this, const char *fmt, ...) {

    va_list var;
    va_start(var, fmt);
    
    char *s = (this->str + this->length);
    size_t n = sizeof(this->str) - (this->length + 1);
    
    this->length += vsnprintf(s, n, fmt, var);
    va_end(var);
}

void vmcu_sstream_put04x(vmcu_sstream_t *this, const unsigned int decimal) {

    char *s = (this->str + this->length);
    size_t n = sizeof(this->str) - (this->length + 1);
    
    this->length += snprintf(s, n, "0x%04x", decimal);
}

void vmcu_sstream_put02x(vmcu_sstream_t *this, const uint8_t decimal) {

    char *s = (this->str + this->length);
    size_t n = sizeof(this->str) - (this->length + 1);

    this->length += snprintf(s, n, "0x%02x", decimal);
}

void vmcu_sstream_pad(vmcu_sstream_t *this, const int padding) {

    char *s = (this->str + this->length);
    size_t n = sizeof(this->str) - (this->length + 1);

    if(padding <= 0)
        return;

    this->length += snprintf(s, n, "%*s", padding, "");
}

char* vmcu_sstream_alloc(const vmcu_sstream_t *this) {
    
    char *string = malloc((this->length + 1) * sizeof(char));
    
    strcpy(string, this->str);
    string[this->length] = '\0';
    
    return string;
}

void vmcu_sstream_flush(vmcu_sstream_t *this) {
    
    this->length = 0;
    this->str[0] = '\0';
}

