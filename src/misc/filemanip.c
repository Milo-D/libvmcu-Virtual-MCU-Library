/* File Manipulation Implementation */

// C++ Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Project Headers
#include "misc/filemanip.h"
#include "misc/stringmanip.h"
#include "collections/array.h"

bool file_exists(const char *file) {

    FILE *check = fopen(file, "r");
    const bool exists = (check != NULL);

    if(exists == true)
        fclose(check);

    return exists;
}

bool is_hex_file(const char *file) {

    const int pos = strpos(file, ".hex");

    if(pos < 0)
        return false;

    if(pos + 4 != strlen(file))
        return false;

    if(pos == 0)
        return false;

    return true;
}

bool check_files(const int argc, char **argv) {

    for(int i = 1; i < argc; i++) {

        if(file_exists(argv[i]) == false)
            return false;

        if(is_hex_file(argv[i]) == false)
            return false;
    }

    return true;
}

void get_files(const int argc, char **argv, array_t *buffer) {

    for(int i = 1; i < argc; i++) {

        if(buffer->top == buffer->size)
            buffer->size += 1;

        const size_t bytes = ((strlen(argv[i]) + 1) * sizeof(char));
        array_push(buffer, (void*) argv[i], bytes);
    }
}