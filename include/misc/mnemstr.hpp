/* String Mnemonics Header */

#ifndef MNEMSTR_HPP
#define MNEMSTR_HPP

#define N_FLOW 11
#define N_MISC 21
#define N_LOGIC 8
#define N_ACCESS 11
#define N_STACKOP 2
#define N_ARITHMETIC 12

extern const std::string flow_mnem[N_FLOW] =             { "rjmp", "brne", "breq", "brge", "brpl", "brlo", 
                                                           "brlt", "brcc", "brcs", "rcall", "ret" };

extern const std::string misc_mnem[N_MISC] =             { "nop", "bld", "bst", "ses", "set", "sev", "sez", "seh", "sec", "sei", "sen", 
                                                           "cls", "clt", "clv", "clz", "clh", "clc", "cli", "cln", "bclr", "bset" };

extern const std::string logic_mnem[N_LOGIC] =           { "clr", "ori", "or", "and", "andi", "com", "asr", "lsr" };


extern const std::string access_mnem[N_ACCESS] =         { "movw", "ldi", "mov", "in", "out", "ld", "ldd", "st", "sts", "xch", "swap" };


extern const std::string stackop_mnem[N_STACKOP] =       { "push", "pop" };


extern const std::string arithmetic_mnem[N_ARITHMETIC] = { "muls", "mulsu", "fmul", "dec", "inc", "add", "adc", "sub", "sbc", "cp", 
                                                           "cpi", "cpc" };

#endif
