#include "tcl.h"

char *custom_generator(const char *text, int state)
{
    static int list_index;      // should be static to has the same value in all iterations //
    static int len;

    if(!state)      // if state is 1 it is new word //
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
            return strdup(match);   // return a copy of match //
        }
    }   

    return NULL;
}

char **custom_completer(const char *text, int start, int end)
{
    char **matches = NULL;

    if (start == 0)     // if it is the first word in command //
    {
        matches = rl_completion_matches (text, custom_generator);
    }

    return (matches);       // return NULL and call default file completer //

}

void help_command()
{
    int pos = 0;    // reset pos to 0 //
    int counter = 0;
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

int read_design(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    char *filename;
    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }
    
    filename = Tcl_GetString(objv[1]);

    printf("The filename is %s\n", filename);

    // call the parser //
    call_parser(filename);

    return TCL_OK;
}

int list_IO(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i, j;
    Tcl_Obj *currPin, *result_pins;

    result_pins = Tcl_NewListObj(0, NULL);

    for (i = 0; i < HASH_SIZE; i++)
    {
        for(j = 0; j < GP_HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if(gatepinhash[i].type[j] == IO_TYPE)
                {
                    currPin = Tcl_NewStringObj(gatepinhash[i].name[j], strlen(gatepinhash[i].name[j]));

                    Tcl_ListObjAppendElement(interp, result_pins, currPin);
                }
            }
        }
    }
    Tcl_SetObjResult(interp, result_pins);

    // const char *resultString = Tcl_GetString(result_pins);
    // printf("Result List: %s\n", resultString);

    return TCL_OK;
}


int main(int argc, char *argv[])
{
    char *text = NULL; // readline result //
    char *textexpansion; // readline result history expanded //
    int expansionresult;

    Tcl_Interp *interp;

    interp = Tcl_CreateInterp();

    HIST_ENTRY **the_history_list; // readline commands history list - NULL terminated //
    char command[LINE_MAX]; // current command //
    unsigned long i;
    // Readline Initialisation //
    rl_completion_entry_function = NULL; // use rl_filename_completion_function(), the default filename completer //
    rl_attempted_completion_function = custom_completer;
    rl_completion_append_character = '\0';  // should not apply ' ' in the end of word //
    using_history(); // initialise history functions //

    Tcl_CreateObjCommand(interp, "read_design", read_design, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_IOs", list_IO, NULL, NULL);

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
            Tcl_DeleteInterp(interp);
            // Gatepins_free();
            structs_free();

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
            help_command();     // call custom command help to print all available commands //
        }

        else if (strncmp(command, "\0", 1) != 0)
        {
            if (Tcl_Eval(interp, command) == TCL_ERROR)
            {
                printf(ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, Tcl_GetStringResult(interp));
                //Tcl_Free(Tcl_GetObjResult(interp));
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
