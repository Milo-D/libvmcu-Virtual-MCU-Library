/* JSon Writer for Headless Output */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "misc/jsonwriter.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/tuple.h"
#include "collections/queue.h"
#include "system/mcudef.h"
#include "system/system.h"

#define CONST_TAB "    "

/* Forward Declaration of static Functions */

static int make_prop_obj(queue_t *buffer);
static int make_status_obj(system_t *sys, queue_t *buffer);
static int make_gpr_obj(system_t *sys, queue_t *buffer);
static int make_sreg_obj(system_t *sys, queue_t *buffer);
static int make_data_obj(system_t *sys, queue_t *buffer);
static int make_eeprom_obj(system_t *sys, queue_t *buffer);

static void add_npair(const char *key, const int val, const bool last, queue_t *buffer);
static void add_apair(const char *key, const char *val, const bool last, queue_t *buffer);

/* --- Extern --- */

int make_json(system_t *sys, queue_t *buffer) {

    queue_put(buffer, 1, "{\n");

    if(make_prop_obj(buffer) < 0)
        return -1;

    if(make_status_obj(sys, buffer) < 0)
        return -1;

    if(make_gpr_obj(sys, buffer) < 0)
        return -1;

    if(make_sreg_obj(sys, buffer) < 0)
        return -1;

    if(make_data_obj(sys, buffer) < 0)
        return -1;

    if(make_eeprom_obj(sys, buffer) < 0)
        return -1;

    queue_put(buffer, 1, "}\n");

    return 0;
}

/* --- Static --- */

static int make_prop_obj(queue_t *buffer) {

    queue_put(buffer, 1, CONST_TAB);
    queue_put(buffer, 1, "\"system_properties\": {\n\n");

    add_apair("ARCH", SYSTEM, false, buffer);
    add_npair("FLASH", (FLASH_SIZE * 2), false, buffer);
    add_npair("SRAM", SRAM_SIZE, false, buffer);
    add_npair("EEPROM", EEPROM_SIZE, true, buffer);

    queue_put(buffer, 2, CONST_TAB, "}, \n\n");

    return 0;
}

static int make_status_obj(system_t *sys, queue_t *buffer) {

    queue_put(buffer, 1, CONST_TAB);
    queue_put(buffer, 1, "\"simulation_status\": {\n\n");

    add_apair("elapsed_time", "...", false, buffer);
    add_apair("status", "terminated", true, buffer);

    queue_put(buffer, 2, CONST_TAB, "}, \n\n");

    return 0;
}

static int make_gpr_obj(system_t *sys, queue_t *buffer) {

    queue_put(buffer, 1, CONST_TAB);
    queue_put(buffer, 1, "\"general_purpose_registers\": {\n\n");

    for(int i = 0; i < GPR_SIZE; i++) {

        int8_t cell_data = sys_read_gpr(sys, i);
        const bool last = (i == (GPR_SIZE - 1));

        if(cell_data == 0x00)
            continue;

        char *reg = get_str(i);
        char *key = strxcat("R", reg);

        char hex[3];
        to_hex(cell_data, hex);
        char *val = strxcat("0x", hex);

        add_apair(key, val, last, buffer);
        nfree(3, reg, key, val);
    }

    queue_put(buffer, 2, CONST_TAB, "}, \n\n");

    return 0;
}

static int make_sreg_obj(system_t *sys, queue_t *buffer) {

    queue_put(buffer, 1, CONST_TAB);
    queue_put(buffer, 1, "\"status_register\": {\n\n");

    for(int i = 0; i < SREG_SIZE; i++) {

        const bool value = sys_read_sreg(sys, i);
        const bool last = (i == SREG_SIZE - 1);

        add_npair(flags[i], value, last, buffer);
    }

    queue_put(buffer, 2, CONST_TAB, "}, \n\n");

    return 0;
}

static int make_data_obj(system_t *sys, queue_t *buffer) {

    queue_put(buffer, 1, CONST_TAB);
    queue_put(buffer, 1, "\"data_memory\": {\n\n");

    for(int i = 0; i < (RAM_END + 1); i++) {

        int8_t cell_data = sys_read_data(sys, i);

        if(cell_data == 0x00)
            continue;

        queue_t *addr = queue_ctor();

        char *hex_addr = itoh(i);
        const int len = strlen(hex_addr);

        char *fill = strfill('0', 2 + len, 4);
        queue_put(addr, 3, "0x", fill, hex_addr);

        char bytes[3];
        to_hex(cell_data, bytes);

        char *key = queue_str(addr);
        char *value = strxcat("0x", bytes);

        const bool last = (i == (RAM_END - 1));
        add_apair(key, value, last, buffer);

        queue_dtor(addr);
        nfree(4, hex_addr, fill, key, value);
    }

    queue_put(buffer, 2, CONST_TAB, "}, \n\n");

    return 0;
}

static int make_eeprom_obj(system_t *sys, queue_t *buffer) {

    queue_put(buffer, 1, CONST_TAB);
    queue_put(buffer, 1, "\"eeprom_memory\": {\n\n");

    for(int i = 0; i < EEPROM_SIZE; i++) {

        int8_t cell_data = sys_read_eeprom(sys, i);

        if(cell_data == 0x00)
            continue;

        queue_t *addr = queue_ctor();

        char *hex_addr = itoh(i);
        const int len = strlen(hex_addr);

        char *fill = strfill('0', 2 + len, 4);
        queue_put(addr, 3, "0x", fill, hex_addr);

        char bytes[3];
        to_hex(cell_data, bytes);

        char *key = queue_str(addr);
        char *value = strxcat("0x", bytes);

        const bool last = (i == (EEPROM_SIZE - 1));
        add_apair(key, value, last, buffer);

        queue_dtor(addr);
        nfree(4, hex_addr, fill, key, value);
    }

    queue_put(buffer, 2, CONST_TAB, "} \n");

    return 0;
}

static void add_npair(const char *key, const int val, const bool last, queue_t *buffer) {

    queue_put(buffer, 2, CONST_TAB, CONST_TAB);
    queue_put(buffer, 3, "\"", key, "\": ");

    char *vstr = get_str(val);
    queue_put(buffer, 1, vstr);

    if(last == false)
        queue_put(buffer, 1, ",");

    queue_put(buffer, 1, "\n");
    free(vstr);
}

static void add_apair(const char *key, const char *val, const bool last, queue_t *buffer) {

    queue_put(buffer, 2, CONST_TAB, CONST_TAB);
    queue_put(buffer, 5, "\"", key, "\": \"", val, "\"");

    if(last == false)
        queue_put(buffer, 1, ",");

    queue_put(buffer, 1, "\n");
}
