/* File Format Recognition Header */

#ifndef VMCU_FMT_H
#define VMCU_FMT_H

typedef enum {

    VMCU_FMT_UNKNOWN = -1,

    VMCU_FMT_IHEX,
    VMCU_FMT_SREC

} VMCU_FMT;

VMCU_FMT vmcu_format_of(const char *path);

#endif