/* Standard Output Messages Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "cli/stdmsg.h"
#include "collections/queue.h"

char* sim_start(const char *file) {

    queue_t *stream = queue_ctor();

    queue_put(stream, 1, "(mdx) Starting simulation of ");
    queue_put(stream, 2, file, "...\n");

    char *msg = queue_str(stream);
    queue_dtor(stream);

    return msg;
}

char* val_of(const char *cell, const char *value) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 5, "(mdx) ", cell, " ~> 0x", value, "\n");

    char *msg = queue_str(stream);
    queue_dtor(stream);

    return msg;
}

char* val_of_eep(const char *addr, const char *range, const char *value) {

    queue_t *stream = queue_ctor();

    queue_put(stream, 4, "(mdx) EEPROM[", addr, "][", range);
    queue_put(stream, 3, "] ~> \"", value, "\"\n");

    char *msg = queue_str(stream);
    queue_dtor(stream);

    return msg;
}

char* val_of_data(const char *addr, const char *range, const char *value) {

    queue_t *stream = queue_ctor();

    queue_put(stream, 4, "(mdx) DATA[", addr, "][", range);
    queue_put(stream, 3, "] ~> \"", value, "\"\n");

    char *msg = queue_str(stream);
    queue_dtor(stream);

    return msg;
}

char* bp_set_success(const char *line) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "(mdx) Breakpoint set at line ", line, ".\n");

    char *out = queue_str(stream);
    queue_dtor(stream);

    return out;
}

char* bp_del_success(const char *line) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "(mdx) Removing Breakpoint at line ", line, ".\n");

    char *out = queue_str(stream);
    queue_dtor(stream);

    return out;
}

char* eep_success(const char *file) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "(mdx) Successfully loaded \"", file, "\" into EEPROM.\n");

    char *out = queue_str(stream);
    queue_dtor(stream);

    return out;
}

char* file_err(const char *file) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "(mdx) Could not open file \"", file, "\"\n");

    char *out = queue_str(stream);
    queue_dtor(stream);

    return out;
}