#ifndef TCL_H
#define TCL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
// #include <tcl8.6/tcl.h>
#include <tcl.h>
// #include <tcl/tcl.h>
#include <limits.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>

#include "../parser/parser.h"
#include "../CUDD_impl/cudd.h"
#include "../signals/signals.h"
#include "../CUDD_impl/toposort.h"
#include "../bdd_annotation/bdd_annotation.h"
#include "../structs/structs.h"
#include "../probabilities_annotations/probabilities_annotations.h"
#include "../gui/gui.h"
#include "../placer/placer.h"


/* Global array that stores all custom, system and TCL commands for tab-completion */
static const char *commands[] = 
{
    // system calls //
    "less", "ls", "quit", "help", "man", "history", 

    // TCL commands //
    "after", "errorInfo", "load", "re_syntax", "tcl_startOfNextWord",
    "append", "eval", "lrange", "read", "tcl_startOfPreviousWord",
    "apply", "exec", "lrepeat", "refchan", "tcl_traceCompile",
    "argc", "exit", "lreplace", "regexp", "tcl_traceExec",
    "argv", "expr", "lreverse", "registry", "tcl_version",
    "argv0", "fblocked", "lsearch", "regsub", "tcl_wordBreakAfter",
    "array", "fconfigure", "lset", "rename", "tcl_wordBreakBefore",
    "auto_execok", "fcopy", "lsort", "return", "tcl_wordchars",
    "auto_import", "file", "mathfunc", "safe", "tcltest",
    "auto_load", "fileevent", "mathop", "scan", "tell",
    "auto_mkindex", "filename", "memory", "seek", "throw",
    "auto_path", "flush", "msgcat", "self", "time",
    "auto_qualify", "for", "my", "set", "timerate",
    "auto_reset", "foreach", "namespace", "socket", "tm",
    "bgerror", "format", "next", "source", "trace",
    "binary", "gets", "nextto", "split", "transchan",
    "break", "glob", "string", "try",
    "catch", "global", "subst", "unknown",
    "cd", "switch", "unload",
    "chan", "http", "tailcall", "unset",
    "clock", "if", "Tcl", "update",
    "close", "incr", "open", "tcl::prefix", "uplevel",
    "concat", "info", "package", "tcl_endOfWord", "upvar",
    "continue", "interp", "parray", "tcl_findLibrary", "variable",
    "coroutine", "join", "pid", "tcl_interactive", "vwait",
    "dde", "lappend", "pkg::create", "tcl_library", "while",
    "dict", "lassign", "pkg_mkIndex", "tcl_nonwordchars", "yield",
    "encoding", "lindex", "platform", "tcl_patchLevel", "yieldto",
    "env", "linsert", "platform::shell", "tcl_pkgPath", "zlib",
    "eof", "list", "proc", "tcl_platform",
    "error", "llength", "puts", "tcl_precision",
    "errorCode", "lmap", "pwd", "tcl_rcFileName", 

    // custom commands //
    "read_design", "list_IOs", "list_components", "report_component_function", 
    "report_component_type", "list_component_CCS", "list_IO_CCS",  "clear_design", "list_cell",
    "list_cells", "list_component_info", "list_components_info", "convert_infix_to_postfix", 
    "report_component_postfix_boolean_function", "report_component_BDD", "compute_expression_BDD", 
    "get_toposort", "get_predecessor_pin", "report_gatepins_levelized", "report_level_gatepins", 
    "report_gatepin_level", "report_library_cell_BDD", "annotate_bdd", "report_bdd_dot_gatepin",
    "get_traverse_cudd",  "set_static_probability", "list_static_probability", "report_bdd_dot_component", "show_gui",
    "hide_gui", "report_coresite", "list_rows", "highligth_component", 
    "list_gatepins", "report_hpwl", "place_random", "minimise_QPWL", 
    
     
    NULL    // should terminate with NULL //

};

// global variables //
extern int probability_calculated;
extern pthread_t gui_thread;
extern Tcl_Interp *interp;

// tab completion functions //
char *custom_generator(const char *text, int state);
char **custom_completer(const char *text, int start, int end);
void help_command();
int read_design(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int list_IO(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int findparameter(int objc, Tcl_Obj *const* objv, char *parameter);
int findparametermain(int argc, char *argv[], char *parameter);
int report_bdd_dot_component(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int report_coresite(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int list_rows(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int highligth_component(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int list_gatepins(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int report_hpwl(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int random_place(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);
int minimise_QPWL(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);

void *main_tcl(void *arg);

#endif
