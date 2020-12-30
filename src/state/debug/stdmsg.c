/* Standard Output Messages Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "state/debug/stdmsg.h"
#include "misc/stringmanip.h"
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

char* bit_val_of(const char *cell, const char *value) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 5, "(mdx) ", cell, " ~> ", value, "\n");

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

char* bp_set_success(const char *addr) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "(mdx) Breakpoint set at address ", addr, ".\n");

    char *out = queue_str(stream);
    queue_dtor(stream);

    return out;
}

char* bp_set_failure(const char *addr) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "(mdx) Could not set Breakpoint at address ", addr, ".\n");

    char *out = queue_str(stream);
    queue_dtor(stream);

    return out;
}

char* bp_del_success(const char *addr) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "(mdx) Removing Breakpoint at address ", addr, ".\n");

    char *out = queue_str(stream);
    queue_dtor(stream);

    return out;
}

char* bp_del_failure(const char *addr) {

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "(mdx) Could not remove Breakpoint at address ", addr, ".\n");

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

char* get_cycles(const uint64_t cycles) {

    char *out = malloc(64 * sizeof(char));
    sprintf(out, "(mdx) Cycles: %ld\n", cycles);

    return out;
}

char* get_clock(const uint32_t clock) {

    char *frequency = nformat(clock);

    queue_t *stream = queue_ctor();
    queue_put(stream, 1, "(mdx) Clock: ");
    queue_put(stream, 2, frequency, " [Hz]\n");

    char *out = queue_str(stream);

    free(frequency);
    queue_dtor(stream);

    return out;
}

char* get_time(const double time) {

    char *out = malloc(256 * sizeof(char));
    sprintf(out, "(mdx) Elapsed Time: %f [μs]\n", time);

    return out;
}
