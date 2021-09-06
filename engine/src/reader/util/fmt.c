/* File Format Recognition Implementation */

// C Headers
#include <string.h>
#include <inttypes.h>

// Project Headers (engine, reader util)
#include "engine/include/reader/util/fmt.h"

#define MIN_EXT_LENGTH 4
#define MAX_EXT_LENGTH 5

#define IHEX_EXTENSION ".hex"
#define SREC_EXTENSION ".srec"

/*
 * We have following valid file extensions
 *
 * [0] .hex  (Intel Hex, VMCU_FMT_IHEX)
 * [1] .srec (Motorola Hex, VMCU_FMT_SREC)
 * [2] .bin  (currently not supported)
 * [3] .elf  (currently not supported)
 *
 * */

/* Forward Declaration of static Functions */

static int32_t get_file_extension(const char *path, char *buffer);

/* --- Extern --- */

VMCU_FMT vmcu_format_of(const char *path) {

    char extension[MAX_EXT_LENGTH + 1];

    if(path == NULL)
        return VMCU_FMT_UNKNOWN;

    if(get_file_extension(path, extension) < 0)
        return VMCU_FMT_UNKNOWN;

    if(strcmp(extension, IHEX_EXTENSION) == 0)
        return VMCU_FMT_IHEX;

    if(strcmp(extension, SREC_EXTENSION) == 0)
        return VMCU_FMT_SREC;

    return VMCU_FMT_UNKNOWN;
}

/* --- Static --- */

static int32_t get_file_extension(const char *path, char *buffer) {

    size_t len = 0;

    if((len = strlen(path)) < MIN_EXT_LENGTH + 1)
        return -1;

    if(path[len - MIN_EXT_LENGTH] == '.') {

        strncpy(buffer, path + len - MIN_EXT_LENGTH, MIN_EXT_LENGTH);
        buffer[MIN_EXT_LENGTH] = '\0';

        return 0;
    }

    if(len < MAX_EXT_LENGTH + 1)
        return -1;

    if(path[len - MAX_EXT_LENGTH] == '.') {

        strncpy(buffer, path + len - MAX_EXT_LENGTH, MAX_EXT_LENGTH);
        buffer[MAX_EXT_LENGTH] = '\0';

        return 0;
    }

    return -1;
}