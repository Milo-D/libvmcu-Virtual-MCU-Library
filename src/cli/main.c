/* Entry Point */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "cli/mainwindow.h"
#include "cli/mode.h"
#include "cli/debugview.h"
#include "cli/stdmsg.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "misc/filemanip.h"
#include "parser/parser.h"
#include "table/table.h"
#include "collections/array.h"

static mainwindow_t *window;

/* Forward Declaration of static Functions */

static table_t** create_table(array_t *files);

/* --- Extern --- */

int main(int argc, char **argv) {

    if(call_mode(argc, argv) == 0)
        return EXIT_SUCCESS;

    table_t **tables;
    array_t *files = array_ctor(4, NULL, NULL);

    int key, x;
    get_files(argc, argv, files);

    if(files->top > 4) {

        array_dtor(files);
        print_status(MAX_FILE_REACH, true);
    }

    if((tables = create_table(files)) == NULL) {

        array_dtor(files);
        print_status(TABLE_INIT_ERR, true);
    }

    if(files->top == 1)
        debug(tables[0]);

    window = mwin_ctor(files);

    do {

        if(((key = mwin_read_key(window)) == SELECT)) {

            mwin_destroy(window);
            x = mwin_get_choice(window);

            debug(tables[x]);
            mwin_reinit(&window);
        }

    } while(key != QUIT);

    for(int i = 0; i < files->top; i++)
        table_dtor(tables[i]);
    
    free(tables);
    array_dtor(files);
    mwin_dtor(window);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static table_t** create_table(array_t *files) {

    table_t **tables = malloc(files->top * sizeof(table_t*));

    if(tables == NULL)
        return NULL;

    for(int i = 0; i < files->top; i++) {

        const char *path = (char*) array_at(files, i);
        tables[i] = table_ctor(path);
    }

    return tables;
}
