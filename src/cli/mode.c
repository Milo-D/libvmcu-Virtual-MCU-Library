/* Mode Implementation */

// C++ Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Project Headers
#include "cli/mode.h"
#include "cli/stdmsg.h"
#include "misc/ehandling.h"
#include "misc/filemanip.h"
#include "misc/jsonwriter.h"
#include "disassembler/disassembler.h"
#include "disassembler/plain.h"
#include "system/system.h"
#include "parser/parser.h"
#include "collections/array.h"
#include "collections/list.h"
#include "collections/tuple.h"
#include "collections/queue.h"

#define TIMEOUT 5.00

/* Forward Declaration of Static Functions */

static void mode_disassembler(const char *hex_file);
static void mode_headless(const char *hex_file);
static void mode_help(void);
static void check_args(const int argc, char **argv, list_t *buffer);

/* --- Public --- */

int call_mode(int argc, char **argv) {

    if(argc < 2)
        print_status("Missing arguments.", true);

    if(check_files(argc, argv) == true)
        return -1;

    queue_t *stream = queue_ctor();
    parser_t *parser = parser_ctor(ARG);

    for(int i = 1; i < argc; i++) {

        queue_put(stream, 1, argv[i]);

        if(i < argc - 1)
            queue_put(stream, 1, " ");
    }

    char *argument = queue_str(stream);
    
    switch(parser_eval(parser, argument)) {

        case 0: mode_disassembler(argv[2]); break;
        case 1: mode_headless(argv[2]); break;
        case 2: mode_help(); break;

        default:

            free(argument);
            queue_dtor(stream);
            parser_dtor(parser);
            
            print_status("Unknown argument...", true); 

        break;
    }

    free(argument);
    queue_dtor(stream);
    parser_dtor(parser);

    return 0;
}

/* --- Private --- */

static void mode_disassembler(const char *hex_file) {

    array_t *source = array_ctor(1, plain_dtor, plain_cpy);
    disassemble(hex_file, source);

    for(int i = 0; i < source->size; i++) {

        plain_t *p = (plain_t*) array_at(source, i);
        printf("%s\n", p->mnem);
    }

    array_dtor(source);
}

static void mode_headless(const char *hex_file) {

    system_t *sys = sys_ctor(hex_file);

    if(sys_table_size(sys) <= 0)
        return;

    clock_t timer = clock();

    do {

        double now = clock() - timer;

        if((now / CLOCKS_PER_SEC) > TIMEOUT)
            break;

        if(sys_step(sys) < 0)
            sys_kill(sys);

    } while(sys_is_term(sys) == false);

    queue_t *stream = queue_ctor();
    make_json(sys, stream);

    char *json = queue_str(stream);
    printf("%s", json);

    free(json);
    
    sys_dtor(sys);
    queue_dtor(stream);
}

static void mode_help(void) {

    printf("\nNote that, MDX is still in development.\n");

    printf("\nARGUMENTS\n");
    printf(" <blank>       <HEX-FILE, ...>           - Interactive Assembly Debugger\n");
    printf(" -d            <HEX-FILE>                - Disassembler\n");
    printf(" -hl           <HEX-FILE>                - Headless Mode\n");
    printf(" -h            <blank>                   - Help\n");

    printf("\nDEBUG COMMANDS\n");
    printf(" n             <blank>                   - Stepping forward\n");
    printf(" b             <blank>                   - Stepping backwards\n");
    printf(" rn            <blank>                   - Show next GPR Page\n");
    printf(" rp            <blank>                   - Show prev. GPR Page\n");
    printf(" dn            <blank>                   - Next Data memory cell\n");
    printf(" dp            <blank>                   - Prev. Data memory cell\n");
    printf(" jb            <delay in ms>             - Jump to next Breakpoint\n");
    printf(" en            <blank>                   - Next EEPROM memory cell\n");
    printf(" ep            <blank>                   - Prev. EEPROM memory cell\n");
    printf(" q             <blank>                   - Leave / Open File Selector\n");
    printf(" ?             <blank>                   - Show commands (coming soon)\n");
    printf(" break         <line>                    - Set a Breakpoint at 'line'\n");
    printf(" unbreak       <line>                    - Remove Breakpoint at 'line'\n");
    printf(" def           <alias> <seq>             - Redefine Symbols in Table\n");
    printf(" pn            <blank>                   - Show next Source Code Page\n");
    printf(" pp            <blank>                   - Show prev. Source Code Page\n");
    printf(" xd            <address>                 - Examine Data Memory at 'address'\n");
    printf(" xe            <address>                 - Examine EEPROM Memory at 'address'\n");
    printf(" xec           <addr> <range>            - Examine EEPROM Memory as char literal\n");
    printf(" leep          <eep.hex>                 - Load an .eep.hex file into the EEPROM\n");
    printf(" cycles        <none>                    - Show current Cycles\n");
    printf(" clock         <none>                    - Show Clock Frequency\n");
    printf(" time          <none>                    - Show elapsed Time\n");

    printf("\nGITHUB\n");
    printf(" https://github.com/Milo-D/MDX-Assembly-Debugger/\n");

    printf("\nAUTHOR OF MDX\n");
    printf(" Name: David Milosevic\n");
    printf(" Email: David.Milosevic@web.de\n");
    printf(" GitHub: Milo-D\n");

    printf("\nREPORTING BUGS\n");
    printf(" https://github.com/Milo-D/MDX-Assembly-Debugger/issues/\n\n");
}

static void check_args(const int argc, char **argv, list_t *buffer) {

    print_status("Parsing arguments...", false);

    if(argc < 2)
        print_status("Missing arguments.", true);

    for(int i = 1; i < argc; i++) {

        const char *argp = argv[i];

        if(file_exists(argp) == false)
            print_status("Specified file does not exist.", true);

        is_hex_file(argp);

        const size_t bytes = strlen(argp);
        ls_push(buffer, (void*) argp, (bytes + 1) * sizeof(char));
    }

    print_status("Done!", false);
}
