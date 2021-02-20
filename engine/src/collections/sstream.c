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

void sstream_ctor(sstream_t *this) {

    sstream_flush(this);
}

void sstream_put(sstream_t *this, const char *fmt, ...) {

    va_list var;
    va_start(var, fmt);
    
    char *s = (this->str + this->length);
    size_t n = sizeof(this->str) - (this->length + 1);
    
    this->length += vsnprintf(s, n, fmt, var);
    va_end(var);
}

void sstream_put04x(sstream_t *this, const unsigned int decimal) {

    char *s = (this->str + this->length);
    size_t n = sizeof(this->str) - (this->length + 1);
    
    this->length += snprintf(s, n, "0x%04x", decimal);
}

void sstream_put02x(sstream_t *this, const uint8_t decimal) {

    char *s = (this->str + this->length);
    size_t n = sizeof(this->str) - (this->length + 1);

    this->length += snprintf(s, n, "0x%02x", decimal);
}

void sstream_pad(sstream_t *this, const int padding) {

    char *s = (this->str + this->length);
    size_t n = sizeof(this->str) - (this->length + 1);

    if(padding <= 0)
        return;

    this->length += snprintf(s, n, "%*s", padding, "");
}

char* sstream_alloc(const sstream_t *this) {
    
    char *string = malloc((this->length + 1) * sizeof(char));
    
    strcpy(string, this->str);
    string[this->length] = '\0';
    
    return string;
}

void sstream_flush(sstream_t *this) {
    
    this->length = 0;
    this->str[0] = '\0';
}

