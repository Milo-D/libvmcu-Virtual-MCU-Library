/* Mode Implementation */

// C++ Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Project Headers
#include "state/main/mode.h"
#include "state/debug/stdmsg.h"
#include "misc/ehandling.h"
#include "misc/filemanip.h"
#include "misc/jsonwriter.h"
#include "disassembler/disassembler.h"
#include "analyzer/analyzer.h"
#include "analyzer/report/report.h"
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

    report_t *report = analyze(hex_file);
    system_t *sys = sys_ctor(report);

    if(report->progsize <= 0)
        return;

    clock_t timer = clock();

    do {

        double now = clock() - timer;

        if((now / CLOCKS_PER_SEC) > TIMEOUT)
            break;

        if(sys_step(sys) < 0)
            break;

    } while(true);

    queue_t *stream = queue_ctor();
    make_json(sys, stream);

    char *json = queue_str(stream);
    printf("%s", json);

    free(json);
    
    sys_dtor(sys);
    report_dtor(report);
    queue_dtor(stream);
}

static void mode_help(void) {

    printf("\nMDX - A Debugger for AVR Microcontrollers.\n");

    printf("\nARGUMENTS\n");
    printf(" <blank>       <HEX-FILE, ...>           - Interactive Assembly Debugger\n");
    printf(" -d            <HEX-FILE>                - Disassembler\n");
    printf(" -hl           <HEX-FILE>                - Headless Mode\n");
    printf(" -h            <none>                    - Help\n");

    printf("\nDEBUG COMMANDS\n");
    printf(" n             <none>                    - Stepping forward\n");
    printf(" b             <none>                    - Stepping backwards\n");
    printf(" rn            <none>                    - Show next GPR Page\n");
    printf(" rp            <none>                    - Show prev. GPR Page\n");
    printf(" dn            <none>                    - Next Data memory cell\n");
    printf(" dp            <none>                    - Prev. Data memory cell\n");
    printf(" jb            <delay in ms>             - Jump to next Breakpoint\n");
    printf(" jc            <cycles>                  - Jump n Cycles forward\n");
    printf(" en            <none>                    - Next EEPROM memory cell\n");
    printf(" ep            <none>                    - Prev. EEPROM memory cell\n");
    printf(" q             <none>                    - Leave / Open File Selector\n");
    printf(" ?             <none>                    - Show commands (coming soon)\n");
    printf(" break         <line>                    - Set a Breakpoint at 'line'\n");
    printf(" unbreak       <line>                    - Remove Breakpoint at 'line'\n");
    printf(" def           <alias> <seq>             - Redefine Symbols in Table\n");
    printf(" pn            <none>                    - Show next Source Code Page\n");
    printf(" pp            <none>                    - Show prev. Source Code Page\n");
    printf(" xd            <address>                 - Examine Data Memory at 'address'\n");
    printf(" xe            <address>                 - Examine EEPROM Memory at 'address'\n");
    printf(" xec           <addr> <range>            - Examine EEPROM Memory as char literal\n");
    printf(" xdc           <addr> <range>            - Examine Data Memory as char literal\n");
    printf(" xdb           <address>                 - Examine Data Memory as bitmap\n");
    printf(" xeb           <address>                 - Examine EEPROM Memory as bitmap\n");
    printf(" leep          <eep.hex>                 - Load an .eep.hex file into the EEPROM\n");
    printf(" cycles        <none>                    - Show current Cycles\n");
    printf(" clock         <none>                    - Show Clock Frequency\n");
    printf(" time          <none>                    - Show elapsed Time\n");
    printf(" cc            <line> <comment>          - Create comment in disassembly\n");

    printf("\nGITHUB\n");
    printf(" https://github.com/Milo-D/MDX-Micro-Debugger/\n");

    printf("\nCOMPLETE WIKI\n");
    printf(" https://github.com/Milo-D/MDX-Micro-Debugger/wiki\n");

    printf("\nREPORTING BUGS\n");
    printf(" https://github.com/Milo-D/MDX-Micro-Debugger/issues/\n");
    
    printf("\nAUTHOR OF MDX\n");
    printf(" Name: David Milosevic\n");
    printf(" Email: David.Milosevic@web.de\n");
    printf(" GitHub: Milo-D\n\n");
}
