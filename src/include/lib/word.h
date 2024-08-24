#ifndef VMCU_WORD_H
#define VMCU_WORD_H

#include <inttypes.h>

/*
 * A structure that represents a
 * single AVR word (16-bit)
 * */
typedef struct vmcu_word {

    uint32_t addr; // address of word
    uint16_t raw;  // word

} vmcu_word_t;

#endif
