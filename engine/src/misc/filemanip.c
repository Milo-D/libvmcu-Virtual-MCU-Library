/* File Manipulation Implementation */

// C Headers
#include <stdio.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/misc/filemanip.h"

long fbytes(const char *file) {

    FILE *fp = fopen(file, "r");

    if(fp == NULL)
        return -1;

    fseek(fp, 0, SEEK_END);
    const long bytes = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    fclose(fp);

    return bytes;
}