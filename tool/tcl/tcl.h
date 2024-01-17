#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <tcl8.6/tcl.h>
#include <limits.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "../parser/parser.h"
#include "../CUDD_impl/cudd.h"
#include "../signals/signals.h"
#include "../CUDD_impl/toposort.h"


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
    "get_toposort", 
     
    NULL    // should terminate with NULL //

};

// tab completion functions //
char *custom_generator(const char *text, int state);

char **custom_completer(const char *text, int start, int end);

void help_command();

int read_design(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);

int list_IO(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv);