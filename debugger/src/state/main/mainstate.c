/* MainState */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/analyzer/analyzer.h"

// Project Headers (debugger)
#include "debugger/include/state/main/mode.h"
#include "debugger/include/state/debug/debugstate.h"
#include "debugger/include/dbg/dbg.h"
#include "debugger/include/cli/main/mainwindow.h"

// Project Headers (shared)
#include "shared/include/misc/ehandling.h"
#include "shared/include/misc/stringmanip.h"
#include "shared/include/misc/filemanip.h"
#include "shared/include/collections/array.h"

#define MAX_FILE_REACH "Can not open more than 4 files."
#define SYS_INIT_ERR "Could not initialize Debug Session."

static mainwindow_t *window;

/* Forward Declaration of static Functions */

static dbg_t* create_dbg(array_t *files);
static void destroy_dbg(dbg_t *dbg, const int n);

/* --- Extern --- */

int main(int argc, char **argv) {

    dbg_t *dbg; int key, x;

    if(call_mode(argc, argv) == 0)
        return EXIT_SUCCESS;
    
    array_t *files = array_ctor(4, NULL, NULL);
    get_files(argc, argv, files);

    if(files->top > 4) {

        array_dtor(files);
        print_status(MAX_FILE_REACH, true);
    }

    if((dbg = create_dbg(files)) == NULL) {

        array_dtor(files);
        print_status(SYS_INIT_ERR, true);
    }

    if(files->top == 1)
        debug(&dbg[0], argv[1]);

    window = mwin_ctor(files);

    do {

        if(((key = mwin_read_key(window)) == SELECT)) {

            mwin_destroy(window);
            x = mwin_get_choice(window);

            debug(&dbg[x], argv[x + 1]);
            mwin_reinit(&window);
        }

    } while(key != QUIT);

    destroy_dbg(dbg, files->top);
    
    array_dtor(files);
    mwin_dtor(window);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static dbg_t* create_dbg(array_t *files) {

    dbg_t *dbg = malloc(files->top * sizeof(dbg_t));

    for(int i = 0; i < files->top; i++) {

        const char *f = (char*) array_at(files, i);
        
        dbg[i].report = analyze(f);
        dbg[i].table  = table_ctor(FLASH_SIZE);
        dbg[i].sys    = sys_ctor(dbg[i].report);
    }

    return dbg;
}

static void destroy_dbg(dbg_t *dbg, const int n) {
    
    for(int i = 0; i < n; i++) {
        
        report_dtor(dbg[i].report);
        table_dtor(dbg[i].table);
        sys_dtor(dbg[i].sys);
    }

    free(dbg);
}
