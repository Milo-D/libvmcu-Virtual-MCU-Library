/* MainState */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "state/main/mode.h"
#include "state/debug/debugstate.h"
#include "cli/main/mainwindow.h"
#include "system/system.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "misc/filemanip.h"
#include "parser/parser.h"
#include "collections/array.h"

#define MAX_FILE_REACH "Can not open more than 4 files."
#define SYS_INIT_ERR "Could not initialize System."

static mainwindow_t *window;

/* Forward Declaration of static Functions */

static system_t** create_systems(array_t *files);

/* --- Extern --- */

int main(int argc, char **argv) {

    if(call_mode(argc, argv) == 0)
        return EXIT_SUCCESS;

    system_t **system;
    array_t *files = array_ctor(4, NULL, NULL);

    int key, x;
    get_files(argc, argv, files);

    if(files->top > 4) {

        array_dtor(files);
        print_status(MAX_FILE_REACH, true);
    }

    if((system = create_systems(files)) == NULL) {

        array_dtor(files);
        print_status(SYS_INIT_ERR, true);
    }

    if(files->top == 1)
        debug(system[0], argv[1]);

    window = mwin_ctor(files);

    do {

        if(((key = mwin_read_key(window)) == SELECT)) {

            mwin_destroy(window);
            x = mwin_get_choice(window);

            debug(system[x], argv[x + 1]);
            mwin_reinit(&window);
        }

    } while(key != QUIT);

    for(int i = 0; i < files->top; i++)
        sys_dtor(system[i]);
    
    free(system);
    array_dtor(files);
    mwin_dtor(window);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static system_t** create_systems(array_t *files) {

    system_t **system = malloc(files->top * sizeof(system_t*));

    if(system == NULL)
        return NULL;

    for(int i = 0; i < files->top; i++) {

        const char *path = (char*) array_at(files, i);
        system[i] = sys_ctor(path);
    }

    return system;
}
