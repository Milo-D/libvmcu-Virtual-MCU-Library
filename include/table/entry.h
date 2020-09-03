/* Table Entry Header */

#ifndef ENTRY_H
#define ENTRY_H

typedef struct _entry {

    char *ln;                   // line of code
    int addr;                   // FLASH address
    bool breakp;                // breakpoint status

} entry_t;

#endif