/* String Mnemonics Header */

// Project Headers
#include "misc/mnemstr.h"

const char *mnemstr[N_MAPS][32] = {

    /* Flow Mnemonics */
    { "rjmp", "brne", "breq", "brge", "brpl", "brlo", 
      "brlt", "brcc", "brcs", "brvs", "rcall", "ret" },

    /* Misc Mnemonics */
    { "nop", "bld", "bst", "ses", "set", "sev", "sez", "seh", "sec", "sei", "sen", 
      "cls", "clt", "clv", "clz", "clh", "clc", "cli", "cln", "bclr", "bset" },

    /* Logic Mnemonics */
    { "clr", "ori", "or", "and", "andi", "com", "asr", "lsr", "ror" },

    /* Access Mnemonics */
    { "movw", "ldi", "mov", "in", "out", "ld", "ldd", "st", "sts", "xch", "swap", "std" },

    /* StackOp Mnemonics */
    { "push", "pop" },

    /* Arithmetic Mnemonics*/
    { "muls", "mulsu", "fmul", "dec", "inc", "add", "adc", "sub", "sbc", "cp", 
      "cpi", "cpc", "sbiw", "adiw" }
};