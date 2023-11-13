#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
// #include "tcl.h"
#include <tcl8.6/tcl.h>
#include <limits.h>
#include <syscall.h>

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

char *custom_generator(const char *text, int state)
{
    static int list_index;      // should be static to has the same value in all iterations //
    static int len;

    if(!state)
    {
        list_index = 0;
        len = strlen(text);
    }

    while(commands[list_index] != NULL)
    {
        const char *match = commands[list_index];
        list_index++;

        if(strncmp(match, text, len) == 0)
        {
            return strdup(match);
        }
    }   

    return NULL;
}

char **custom_completer(const char *text, int start, int end)
{
    char **matches = NULL;

    /* If this word is at the start of the line, then it is a command
    to complete.  Otherwise it is the name of a file in the current
    directory. */
  if (start == 0)
    matches = rl_completion_matches (text, custom_generator);

  return (matches);

}


int main(int argc, char *argv[])
{
    char *text = NULL; // readline result //
    char *textexpansion; // readline result history expanded //
    int expansionresult;

    Tcl_Interp *interp;

    interp = Tcl_CreateInterp();

    int pos = 0, counter = 0;

    HIST_ENTRY **the_history_list; // readline commands history list - NULL terminated //
    char command[LINE_MAX]; // current command //
    unsigned long i;
    // Readline Initialisation //
    rl_completion_entry_function = NULL; // use rl_filename_completion_function(), the default filename completer //
    rl_attempted_completion_function = custom_completer;
    //rl_completion_suppress_append = 1;
    rl_completion_append_character = '\0';
    using_history(); // initialise history functions //
    while (1)
    {
        text = readline("PR> ");
        if (text != NULL)
        {
            expansionresult = history_expand(text, &textexpansion);
            if ((expansionresult == 0) || // no expansion //
                (expansionresult == 2)) // do not execute //
            {
                add_history(text);
                strcpy(command, text); // store command //
            }
            else
            {
                add_history(textexpansion);
                strcpy(command, textexpansion); // store command //
            }
            free(textexpansion);
            free(text);
        }
        
        // if last char is space do it \0 to work with strcmp //
        if(strlen(command) != 0)
        {
            if(command[strlen(command)-1] == ' ')
            {
                command[strlen(command)-1] = '\0';
            }
        }

        // handle two basic commands: history and quit //
        if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }
        else if (strcmp(command, "history") == 0)
        {
            the_history_list = history_list(); // get history list //
            if (the_history_list != NULL)
            {
                i = 0;
                while (*(the_history_list + i) != NULL) // history list - NULL terminated //
                {
                    printf("%lu: %s\n", (i + history_base), (*(the_history_list + i))->line);
                    i++;
                }
            }
        }
        else if (strncmp(command, "less", 4) == 0)
        {
            system(command);
        }
        else if(strncmp(command, "ls", 2) == 0)
        {
            system(command);
        }
        else if(strncmp(command, "man", 3) == 0)
        {
            system(command);
        }
        else if(strncmp(command, "help", 4) == 0)
        {
            printf("============================\n");
            printf("     SYSTEM COMMANDS\n");
            printf("============================\n\n");
            while(commands[pos] != NULL && pos != 6)
            {
                printf("%s",  commands[pos]);
                printf("  •  ");
                pos++;
            }
            printf("\n\n");

            printf("============================\n");
            printf("     TCL COMMANDS\n");
            printf("============================\n\n");
            while(commands[pos] != NULL)
            {
                counter++;
                printf("%s",  commands[pos]);
                if(counter == 6)
                {
                    printf("\n");
                    counter = 0;
                }
                else
                {
                    printf("  •  ");
                }
                pos++;
            }
            printf("\n");
        }

        else if (strncmp(command, "\0", 1) != 0)
        {
            #define ANSI_COLOR_RED     "\x1b[31m"
            #define ANSI_COLOR_RESET   "\x1b[0m"
            #define ANSI_COLOR_GREEN   "\x1b[32m"

            if (Tcl_Eval(interp, command) == TCL_ERROR)
            {
                printf(ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, Tcl_GetStringResult(interp));
            }
            else
            {
                if(strcmp(Tcl_GetStringResult(interp), "\n") != 0)
                    printf(ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, Tcl_GetStringResult(interp));
            }
            if(strncmp(command, "cd", 2) == 0)      // if user call command cd, print current path to know in which dir user is //
            {
                Tcl_Eval(interp, "pwd");
                printf(ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, Tcl_GetStringResult(interp));
            }
        }
    }
}
