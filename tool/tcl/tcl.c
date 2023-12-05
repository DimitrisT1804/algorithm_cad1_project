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

    if(gatepinhash != NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: Already a design has been loaded!\nYou can call ""clear_design"" to clear current design!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    
    filename = Tcl_GetString(objv[1]);

    #ifdef DEBUG
    printf("The filename is %s\n", filename);
    #endif

    // call the parser //
    call_parser(filename);

    return TCL_OK;
}

int list_IO(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i, j;
    Tcl_Obj *currPin, *result_pins;

    result_pins = Tcl_NewListObj(0, NULL);
    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

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

    return TCL_OK;
}

int list_components(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i, j;
    Tcl_Obj *currComp, *result_comps;

    result_comps = Tcl_NewListObj(0, NULL);
    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    for (i = 0; i < COMPHASH_SIZE; i++)
    {
        for(j = 0; j < COMP_HASHDEPTH; j++)
        {
            if(comphash[i].hashpresent[j] != 0)
            {
                currComp = Tcl_NewStringObj(comphash[i].name[j], strlen(comphash[i].name[j]));

                Tcl_ListObjAppendElement(interp, result_comps, currComp);
            }
        }
    }
    Tcl_SetObjResult(interp, result_comps);

    return TCL_OK;
}

int report_component_function(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int chash, cdepth;
    int lhash, ldepth;
    Tcl_Obj *function_obj;
    char *currComp = NULL;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "component");
        return TCL_ERROR;
    }

    currComp = Tcl_GetString(objv[1]);

    if(currComp == NULL)
    {
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_comphash_indices(currComp, &chash, &cdepth);
    if(cdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Component NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];

    function_obj =  Tcl_NewStringObj(libhash[lhash].function[ldepth], -1);

    Tcl_SetObjResult(interp, function_obj);

    return TCL_OK;
}

int report_component_type(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int chash, cdepth;
    int lhash, ldepth;
    // Tcl_Obj *cell_type;
    int cell_type;
    char *currComp = NULL;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "component");
        return TCL_ERROR;
    }

    currComp = Tcl_GetString(objv[1]);

    if(currComp == NULL)
    {
        return TCL_ERROR;
    }

    // cell_type = Tcl_NewListObj(0, NULL);
    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_comphash_indices(currComp, &chash, &cdepth);
    if(cdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Component NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];

    // cell_type =  Tcl_NewStringObj(libhash[lhash].name[ldepth], -1);
    cell_type = libhash[lhash].cell_type[ldepth];

    if(cell_type == COMBINATIONAL)
    {
        Tcl_SetObjResult(interp, Tcl_NewStringObj("Combinational", -1) );
    }
    else if(cell_type == SEQUENTIAL)
    {
        Tcl_SetObjResult(interp, Tcl_NewStringObj("Sequential", -1));
    }
    else
    {
        return TCL_ERROR;
    }

    // libhash[lhash].function[ldepth];
    //Tcl_ListObjAppendElement(interp, cell_type, function_obj);

    // Tcl_SetObjResult(interp, cell_type);

    return TCL_OK;
}

int list_component_CCS(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i, j;
    int chash, cdepth;
    int lhash, ldepth;
    int ghash, gdepth;
    int gconhash, gcondepth;
    Tcl_Obj *pin_connections, *pin_con_name;
    char *currComp = NULL;
    char *currPin = NULL;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "component");
        return TCL_ERROR;
    }

    currComp = Tcl_GetString(objv[1]);

    if(currComp == NULL)
    {
        return TCL_ERROR;
    }

    pin_connections = Tcl_NewListObj(0, NULL);
    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_comphash_indices(currComp, &chash, &cdepth);
    if(cdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Component NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];

    for(i = 0; i < libhash[lhash].pin_count[ldepth]; i++)
    {
        currPin = (char *) my_calloc(strlen(currComp) + strlen(libhash[lhash].pin_names[ldepth][i]) + 1, sizeof(char));
        strcpy(currPin, currComp);
        strcat(currPin, libhash[lhash].pin_names[ldepth][i]);

        get_gatepin_indices(currPin, &ghash, &gdepth);
        if(gdepth == -1)
        {
            printf(ANSI_COLOR_RED "ERROR: gatepin NOT found" ANSI_COLOR_RESET);
            return TCL_ERROR;
        }
        free(currPin);
        if(gatepinhash[ghash].connections_size[gdepth] != 0)
        {
            for(j = 0; j < gatepinhash[ghash].connections_size[gdepth]; j++)
            {
                gconhash = gatepinhash[ghash].pinConn[gdepth][j];
                gcondepth = gatepinhash[ghash].pinConnDepth[gdepth][j];

                pin_con_name = Tcl_NewStringObj(gatepinhash[gconhash].name[gcondepth], strlen(gatepinhash[gconhash].name[gcondepth]));
                Tcl_ListObjAppendElement(interp, pin_connections, pin_con_name);
            }
            break;
        }
    }
    // libhash[lhash].function[ldepth];
    //Tcl_ListObjAppendElement(interp, pin_connections, function_obj);

    printf(ANSI_COLOR_BLUE "Successors of pin %s are: " ANSI_COLOR_RESET , gatepinhash[ghash].name[gdepth]);
    Tcl_SetObjResult(interp, pin_connections);
    // free(currComp);
    // free(currPin);

    return TCL_OK;
}

int list_IO_CCS(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    int ghash, gdepth;
    int gconhash, gcondepth;
    Tcl_Obj *pin_connections, *result_pin;
    char *currPin = NULL;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "component");
        return TCL_ERROR;
    }

    currPin = Tcl_GetString(objv[1]);

    if(currPin == NULL)
    {
        return TCL_ERROR;
    }

    pin_connections = Tcl_NewListObj(0, NULL);
    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_gatepin_indices(currPin, &ghash, &gdepth);
    if(gdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Input pin does not exist!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if( (gatepinhash[ghash].type[gdepth] != IO_TYPE))
    {
        printf(ANSI_COLOR_RED "ERROR: Given pin is not an IO pin" ANSI_COLOR_RESET);
        return TCL_ERROR;  
    }
    else if((gatepinhash[ghash].connections_size[gdepth]) == 0)
    {
        printf(ANSI_COLOR_RED "ERROR: Given pin has not connections" ANSI_COLOR_RESET);
        return TCL_ERROR;   
    }

    for(i = 0; i < gatepinhash[ghash].connections_size[gdepth]; i++)
    {
        gconhash = gatepinhash[ghash].pinConn[gdepth][i];
        gcondepth = gatepinhash[ghash].pinConnDepth[gdepth][i];

        result_pin = Tcl_NewStringObj(gatepinhash[gconhash].name[gcondepth], strlen(gatepinhash[gconhash].name[gcondepth]));
        Tcl_ListObjAppendElement(interp, pin_connections, result_pin);
    }

    Tcl_SetObjResult(interp, pin_connections);


    return TCL_OK;
}

int clear_design(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    if(objc != 1)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "");
        return TCL_ERROR;
    }

    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded, nothing to clear!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    structs_free();
    gatepinhash = NULL;
    comphash = NULL;
    libhash = NULL;

    printf(ANSI_COLOR_GREEN "Design succesfully cleared!" ANSI_COLOR_RESET);

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
    Tcl_CreateObjCommand(interp, "list_components", list_components, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_component_function", report_component_function, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_component_type", report_component_type, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_component_CCS", list_component_CCS, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_IO_CCS", list_IO_CCS, NULL, NULL);
    Tcl_CreateObjCommand(interp, "clear_design", clear_design, NULL, NULL);

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
            if(gatepinhash != NULL)
            {
                structs_free();
            }
            printf(ANSI_COLOR_BLUE "EDA TOOL EXITING\n" ANSI_COLOR_RESET);
            Tcl_Finalize();

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
