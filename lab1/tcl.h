#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <tcl8.6/tcl.h>
#include <limits.h>
#include <syscall.h>

#define ANSI_COLOR_RED     "\x1b[31m"   // define color codes to print TCL messages //
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"


static const char *commands[] = 
{
    // system calls
    "less", "ls", "quit", "help", "man", "history", 

    // TCL commands
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
    "break", "glob", "oo::class", "string", "try",
    "catch", "global", "oo::copy", "subst", "unknown",
    "cd", "history", "oo::define", "switch", "unload",
    "chan", "http", "oo::objdefine", "tailcall", "unset",
    "clock", "if", "oo::object", "Tcl", "update",
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
    
    NULL    // should terminate with NULL //

};

// tab completion functions //
char *custom_generator(const char *text, int state);

char **custom_completer(const char *text, int start, int end);

void help_command();