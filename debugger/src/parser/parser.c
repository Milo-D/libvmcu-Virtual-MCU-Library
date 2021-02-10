/* Parser Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (debugger)
#include "debugger/include/parser/parser.h"
#include "debugger/include/parser/commands.h"

// Project Headers (shared)
#include "shared/include/collections/list.h"
#include "shared/include/collections/map.h"
#include "shared/include/misc/ehandling.h"
#include "shared/include/misc/filemanip.h"
#include "shared/include/misc/stringmanip.h"

/* Forward Declaration of private Functions */

static bool parser_parse_arg(const struct _parser *this, list_t *items);
static bool parser_parse_debug(const struct _parser *this, list_t *items);

/* --- Public --- */

struct _parser* parser_ctor(CONTEXT context) {

    struct _parser *parser;

    if((parser = malloc(sizeof(struct _parser))) == NULL)
        return NULL;

    parser->map = map_ctor(ncmd[context]);

    for(int i = 0; i < ncmd[context]; i++)
        map_put(parser->map, commands[context][i], i);

    parser->context = context;
    return parser;
}

void parser_dtor(struct _parser *this) {

    map_dtor(this->map);
    free(this);
}

int parser_eval(const struct _parser *this, const char *cmd) {

    list_t *items = ls_ctor(NULL, NULL);
    split(cmd, ' ', items);

    if(items->size == 0)
        return -1;

    const char *keyword = (char*) ls_at(items, 0);

    if(map_get(this->map, keyword) < 0) {

        ls_dtor(items);
        return -1;
    }

    bool passed = false;

    switch(this->context) {

        case ARG: passed = parser_parse_arg(this, items); break;
        case DEBUG: passed = parser_parse_debug(this, items); break;

        default: break;
    }

    const int res = (passed ? map_get(this->map, keyword) : -1);
    ls_dtor(items);

    return res;
}

/* --- Private --- */

static bool parser_parse_arg(const struct _parser *this, list_t *items) {

    const char *keyword = ls_at(items, 0);

    const int index = map_get(this->map, keyword);
    const int argc = items->size - 1;

    switch(index) {

        case 0: case 1:

            if(argc != 1)
                print_status("Missing File.", true);

            const char *param = ls_at(items, 1);

            if(file_exists(param) == false)
                print_status("File does not exist.", true);

        break;

        case 2:

            if(argc != 0)
                print_status("Too many arguments.", true);

        break;

        default: return false;
    }

    return true;
}

static bool parser_parse_debug(const struct _parser *this, list_t *items) {

    const char *keyword = ls_at(items, 0);

    const int index = map_get(this->map, keyword);
    const int argc = items->size - 1;

    switch(index) {

        case 0: case 1: case 2: case 3: case 4:
        case 5: case 7: case 8: case 14: case 15:
        case 16: case 20: case 21: case 22: case 23:
        case 24:

            if(argc != 0)
                return false;

        break;

        case 6: case 9: case 10: case 13: case 17:
        case 18: case 25: case 26: case 28: case 29:

            if(argc != 1)
                return false;

        break;

        case 11: case 12: case 19: case 27:

            if(argc != 2)
                return false;

        break;
        
        default: print_status("Could not parse line.", true); break;
    }
        
    return true;
}
