/* (Debug CLI) Printer Implementation */

// Project Headers
#include "printer/debugprinter.h"
#include "printer/dpnl_printer.h"
#include "printer/epnl_printer.h"
#include "printer/fpnl_printer.h"
#include "printer/gpnl_printer.h"
#include "printer/rpnl_printer.h"
#include "printer/spnl_printer.h"
#include "cli/debug/debugwindow.h"
#include "dbg/dbg.h"

extern void print_debug(debugwindow_t *window, dbg_t *dbg) {

    print_gpnl(window, dbg);
    print_spnl(window, dbg);
    print_dpnl(window, dbg);
    print_epnl(window, dbg);
    print_fpnl(window, dbg);
    print_rpnl(window, dbg);

    dwin_update_all(window);
}
