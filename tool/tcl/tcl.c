#include "tcl.h"

int probability_calculated = 0;
pthread_t gui_thread;
Tcl_Interp *interp;

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

int ls_command(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    char *custom_command = NULL;
    char *temp = NULL;
    int total_size = 0;
    int len = 0;

    if(objc > 3)
    {
        Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, "[option] [filename]");

        return TCL_ERROR;
    }

    custom_command = (char *)my_realloc(custom_command, 1);
    custom_command[0] = '\0';

    for(i = 0; i < objc; i++)
    {
        temp = Tcl_GetStringFromObj(objv[i], NULL);
        len = strlen(temp);
        custom_command = (char *) my_realloc(custom_command, total_size + len + 2);
        if(i >= 1)
        {
            strcat(custom_command, " ");
        }
        strcat(custom_command, temp);
        total_size = total_size + len + 2;
    }
    system(custom_command);

    free(custom_command);

    return TCL_OK;
}

int less_command(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    char *custom_command = NULL;
    char *temp = NULL;
    int total_size = 0;
    int len = 0;

    if(objc > 3)
    {
        Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, "[option] [filename]");

        return TCL_ERROR;
    }

    custom_command = (char *)my_realloc(custom_command, 1);
    custom_command[0] = '\0';

    for(i = 0; i < objc; i++)
    {
        temp = Tcl_GetStringFromObj(objv[i], NULL);
        len = strlen(temp);
        custom_command = (char *) my_realloc(custom_command, total_size + len + 2);
        if(i >= 1)
        {
            strcat(custom_command, " ");
        }
        strcat(custom_command, temp);
        total_size = total_size + len + 2;
    }
    system(custom_command);

    free(custom_command);

    return TCL_OK;
}

int man_command(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    char *custom_command = NULL;
    char *temp = NULL;
    int total_size = 0;
    int len = 0;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, (Tcl_Obj * const *)objv, "manual");

        return TCL_ERROR;
    }


    custom_command = (char *)my_realloc(custom_command, 1);
    custom_command[0] = '\0';


    for(i = 0; i < objc; i++)
    {
        temp = Tcl_GetStringFromObj(objv[i], NULL);
        len = strlen(temp);
        custom_command = (char *) my_realloc(custom_command, total_size + len + 2);
        if(i >= 1)
        {
            strcat(custom_command, " ");
        }
        strcat(custom_command, temp);
        total_size = total_size + len + 2;
    }
    system(custom_command);

    free(custom_command);

    return TCL_OK;
}

int read_design(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    char *filename;
    int parser_result = 0;
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
    parser_result = call_parser(filename);
    if (parser_result == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Cannot open file %s" ANSI_COLOR_RESET, filename);
    }
    else if (parser_result == -2)
    {
        printf(ANSI_COLOR_RED "ERROR: File %s has not the correct format!\n" ANSI_COLOR_RESET, filename);
    }

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

    for (i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if(gatepinhash[i].type[j] == IO_TYPE || gatepinhash[i].type[j] == PO)
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

    for (i = 0; i < comphash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
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
    Tcl_Obj *function_obj, *curr_func;
    char *currComp = NULL;
    int i;

    function_obj = Tcl_NewListObj(0, NULL);

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

    printf(ANSI_COLOR_BLUE "Cell is %s with function " ANSI_COLOR_RESET, libhash[lhash].name[ldepth]);
    for(i = 0; i < libhash[lhash].out_pins_count[ldepth]; i++)
    {
        if(libhash[lhash].function[ldepth][i] != NULL)
        {
            curr_func = Tcl_NewStringObj(libhash[lhash].function[ldepth][i], strlen(libhash[lhash].function[ldepth][i]));
            Tcl_ListObjAppendElement(interp, function_obj, curr_func);
        }
    }

    Tcl_SetObjResult(interp, function_obj);

    return TCL_OK;
}

int report_component_type(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int chash, cdepth;
    int lhash, ldepth;
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

    printf(ANSI_COLOR_BLUE "Successors of pin %s are: " ANSI_COLOR_RESET , gatepinhash[ghash].name[gdepth]);
    Tcl_SetObjResult(interp, pin_connections);

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

    if( (gatepinhash[ghash].type[gdepth] != IO_TYPE) && (gatepinhash[ghash].type[gdepth] != PO))
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
    if(gatepinhash[ghash].type[gdepth] == IO_TYPE)
    {
        printf(ANSI_COLOR_BLUE "Successors of PI pin %s are: " ANSI_COLOR_RESET , gatepinhash[ghash].name[gdepth]);
    }
    else
    {
        printf(ANSI_COLOR_BLUE "Successors of PO pin %s are: " ANSI_COLOR_RESET , gatepinhash[ghash].name[gdepth]);
    
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
    pthread_mutex_lock(&mutex);

    structs_free();
    gatepinhash = NULL;
    comphash = NULL;
    libhash = NULL;
    gatepinhashv = NULL;
    gatepinhash_prob = NULL;

    printf(ANSI_COLOR_GREEN "Design succesfully cleared!" ANSI_COLOR_RESET);

    isLevelized = 0;
    max_design_level = -2;

    free(ghash_added);
    free(gdepth_added);
    ghash_added = NULL;
    gdepth_added = NULL;
    ghash_added_size = 0;

    design_is_placed = 0;
    pthread_mutex_unlock(&mutex);

    if(gbm != NULL)
    {
        Cudd_Quit(gbm);
        gbm = NULL;
    }
    return TCL_OK;
}

int list_cell(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    int lhash, ldepth;
    char *currCell = NULL;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "cell");
        return TCL_ERROR;
    }

    currCell = Tcl_GetString(objv[1]);

    if(currCell == NULL)
    {
        return TCL_ERROR;
    }

    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_libhash_indices(currCell, &lhash, &ldepth);
    if(ldepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Cell %s does not exists!" ANSI_COLOR_RESET, currCell);
        return TCL_ERROR;
    }

    printf(ANSI_COLOR_BLUE"------------- INFO CELL: %s -------------\n" ANSI_COLOR_RESET, currCell);
    if(libhash[lhash].cell_type[ldepth] == COMBINATIONAL)
    {
        printf(ANSI_COLOR_ORANGE"Cell Type is: Combinational\n" ANSI_COLOR_RESET);
    }
    else
    {
        printf("Cell Type is: Sequential\n");
    }

    printf("Pin names are: \n");
    for(i = 0; i < libhash[lhash].pin_count[ldepth]; i++)
    {
        printf(ANSI_COLOR_GREEN "%d) %s " ANSI_COLOR_RESET, i+1, (libhash[lhash].pin_names[ldepth][i] + 1));
        if(libhash[lhash].pin_type[ldepth][i] == OUTPUT)
        {
            printf(ANSI_COLOR_GREEN "output pin with function %s\n" ANSI_COLOR_RESET, libhash[lhash].function[ldepth][i]);
        }
        else
        {
            printf(ANSI_COLOR_GREEN "input pin\n" ANSI_COLOR_RESET); 
        }
    }

    printf(ANSI_COLOR_BLUE "---------------------------------------------\n" ANSI_COLOR_RESET);

    return TCL_OK;
}

int list_cells(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    int lhash, ldepth;

    if(objc != 1)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "no argument");
        return TCL_ERROR;
    }

    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    for(lhash = 0; lhash < libhash_size; lhash++)
    {
        for(ldepth = 0; ldepth < HASHDEPTH; ldepth++)
        {
            if(libhash[lhash].hashpresent[ldepth] == 1)
            {
                printf(ANSI_COLOR_BLUE"------------- INFO CELL: %s -------------\n" ANSI_COLOR_RESET, libhash[lhash].name[ldepth]);
                if(libhash[lhash].cell_type[ldepth] == COMBINATIONAL)
                {
                    printf(ANSI_COLOR_ORANGE"Cell Type is: Combinational\n" ANSI_COLOR_RESET);
                }
                else
                {
                    printf(ANSI_COLOR_MAGENDA "Cell Type is: Sequential\n" ANSI_COLOR_RESET);
                }
                printf(ANSI_COLOR_BLUE"Cell_width: %.3f\n", libhash[lhash].width[ldepth]);
                printf(ANSI_COLOR_BLUE"Cell_height: %.3f\n" ANSI_COLOR_RESET, libhash[lhash].height[ldepth]);


                printf("Pin names are: \n");
                for(i = 0; i < libhash[lhash].pin_count[ldepth]; i++)
                {
                    printf(ANSI_COLOR_GREEN "%d) %s " ANSI_COLOR_RESET, i+1, (libhash[lhash].pin_names[ldepth][i] + 1));
                    if(libhash[lhash].pin_type[ldepth][i] == OUTPUT)
                    {
                        printf(ANSI_COLOR_GREEN "output pin with function %s\n" ANSI_COLOR_RESET, libhash[lhash].function[ldepth][i]);
                    }
                    else
                    {
                        printf(ANSI_COLOR_GREEN "input pin\n" ANSI_COLOR_RESET); 
                    }
                }

                printf(ANSI_COLOR_BLUE "---------------------------------------------\n\n" ANSI_COLOR_RESET);
            }
        }
    }

    return TCL_OK;
}

int list_component_info(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i, j;
    int chash, cdepth;
    int lhash, ldepth;
    int ghash, gdepth;
    int gconhash, gcondepth;
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

    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_comphash_indices(currComp, &chash, &cdepth);
    if(cdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Component %s does not exists!" ANSI_COLOR_RESET, currComp);
        return TCL_ERROR;
    }

    printf(ANSI_COLOR_BLUE"------------- INFO COMPONENT: %s -------------\n" ANSI_COLOR_RESET, currComp);

    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];
    printf(ANSI_COLOR_ORANGE"Component is of type: %s\n" ANSI_COLOR_RESET, libhash[lhash].name[ldepth]);

    for(i = 0; i < libhash[lhash].pin_count[ldepth]; i++)
    {
        currPin = (char *) my_calloc(strlen(currComp) + strlen(libhash[lhash].pin_names[ldepth][i]) + 1, sizeof(char));
        strcpy(currPin, currComp);
        strcat(currPin, libhash[lhash].pin_names[ldepth][i]);

        get_gatepin_indices(currPin, &ghash, &gdepth);
        if(gdepth == -1)
        {
            printf(ANSI_COLOR_RED "ERROR: gatepin NOT found" ANSI_COLOR_RESET);
            free(currPin);
            return TCL_ERROR;
        }
        free(currPin);
        if(gatepinhash[ghash].connections_size[gdepth] != 0)
        {
            printf(ANSI_COLOR_BLUE "Successors of pin %s are: \n" ANSI_COLOR_RESET, gatepinhash[ghash].name[gdepth]);
            for(j = 0; j < gatepinhash[ghash].connections_size[gdepth]; j++)
            {
                gconhash = gatepinhash[ghash].pinConn[gdepth][j];
                gcondepth = gatepinhash[ghash].pinConnDepth[gdepth][j];

                printf(ANSI_COLOR_GREEN "• %s \n" ANSI_COLOR_RESET, gatepinhash[gconhash].name[gcondepth]);
            }
            break;
        }
    }
    for(i = 0; i < strlen(currComp) + 44; i++)
    {
        printf(ANSI_COLOR_BLUE "-" ANSI_COLOR_RESET);
    }
    printf("\n");

    return TCL_OK;
}


int list_components_info(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i, j;
    int chash, cdepth;
    int lhash, ldepth;
    int ghash, gdepth;
    int gconhash, gcondepth;
    char *currPin = NULL;

    if(objc != 1)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "no arguments");
        return TCL_ERROR;
    }

    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    for(chash = 0; chash < comphash_size; chash++)
    {
        for(cdepth = 0; cdepth < HASHDEPTH; cdepth++)
        {
            if(comphash[chash].hashpresent[cdepth] == 1)
            {
                printf(ANSI_COLOR_BLUE"------------- INFO COMPONENT: %s -------------\n" ANSI_COLOR_RESET, comphash[chash].name[cdepth]);

                lhash = comphash[chash].lib_type[cdepth];
                ldepth = comphash[chash].lib_type_depth[cdepth];
                printf(ANSI_COLOR_ORANGE"Component is of type: %s\n" ANSI_COLOR_RESET, libhash[lhash].name[ldepth]);

                for(i = 0; i < libhash[lhash].pin_count[ldepth]; i++)
                {
                    currPin = (char *) my_calloc(strlen(comphash[chash].name[cdepth]) + strlen(libhash[lhash].pin_names[ldepth][i]) + 1, sizeof(char));
                    strcpy(currPin, comphash[chash].name[cdepth]);
                    strcat(currPin, libhash[lhash].pin_names[ldepth][i]);

                    get_gatepin_indices(currPin, &ghash, &gdepth);
                    if(gdepth == -1)
                    {
                        printf(ANSI_COLOR_RED "ERROR: gatepin NOT found" ANSI_COLOR_RESET);
                        free(currPin);
                        return TCL_ERROR;
                    }
                    free(currPin);
                    if(gatepinhash[ghash].connections_size[gdepth] != 0)
                    {
                        printf(ANSI_COLOR_BLUE "Successors of pin %s are: \n" ANSI_COLOR_RESET, gatepinhash[ghash].name[gdepth]);
                        for(j = 0; j < gatepinhash[ghash].connections_size[gdepth]; j++)
                        {
                            gconhash = gatepinhash[ghash].pinConn[gdepth][j];
                            gcondepth = gatepinhash[ghash].pinConnDepth[gdepth][j];

                            printf(ANSI_COLOR_GREEN "• %s \n" ANSI_COLOR_RESET, gatepinhash[gconhash].name[gcondepth]);
                        }
                    }
                }
                printf(ANSI_COLOR_MAGENDA"Location is: (X: %f, Y: %f)\n" ANSI_COLOR_RESET, compslocation[chash].x[cdepth], compslocation[chash].y[cdepth]);
                printf(ANSI_COLOR_MAGENDA "Drawing location is (X: %f, Y: %f)\n" ANSI_COLOR_RESET, compslocation[chash].drawing_x[cdepth], compslocation[chash].drawing_y[cdepth]);
                for(i = 0; i < strlen(comphash[chash].name[cdepth]) + 44; i++)
                {
                    printf(ANSI_COLOR_BLUE "-" ANSI_COLOR_RESET);
                }
                printf("\n\n");
            }
        }
    }

    return TCL_OK;
}

int convert_infix_to_postfix(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    char *infix = NULL;
    char *result = NULL;
    char *postfix = NULL;
    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "infix_expression");
        return TCL_ERROR;
    }

    infix = Tcl_GetString(objv[1]);
    if(infix == NULL)
    {
        printf("ERROR: Infix is NULL\n");
        return TCL_ERROR;
    }

    postfix = parse_infix(infix);
    if(postfix == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: Postfix has not correct format\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    result = malloc(strlen(infix) + 5 + strlen("The postfix of  is ") + strlen(postfix));
    
    strcpy(result, "The postfix of ");
    strcat(result, infix);
    strcat(result, " is: ");
    strcat(result, postfix);

    Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));

    free(postfix);
    free(result);

    return TCL_OK;
}

int report_component_postfix_boolean_function(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    char *component = NULL;
    int chash, cdepth;
    int lhash, ldepth;
    int i;
    int size = 0;
    char *infix = NULL;
    char *result = NULL;
    char *postfix = NULL;
    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "component_name");
        return TCL_ERROR;
    }

    component = Tcl_GetString(objv[1]);
    if(component == NULL)
    {
        printf("ERROR: Infix is NULL\n");
        return TCL_ERROR;
    }
    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_comphash_indices(component, &chash, &cdepth);
    if(cdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Component NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR; 
    }

    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];
    result = NULL;
    for(i = 0; i < libhash[lhash].out_pins_count[ldepth]; i++)
    {
        infix = strdup(libhash[lhash].function[ldepth][i]);
        postfix = parse_infix(infix);
        size = size + strlen(infix) + 3 + strlen("The postfix of  is ") + strlen(postfix);

        result = my_realloc(result, (size) * sizeof(char));
        if(i == 0)
        {
            strcpy(result, "The postfix of ");
        }
        else
        {
            strcat(result, "The postfix of ");
        }

        strcat(result, infix);
        strcat(result, " is: ");
        strcat(result, postfix);
        strcat(result, "\n");

        free(infix);
        free(postfix);
    }
    
    Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));

    free(result);

    return TCL_OK;
}

int report_library_cell_BDD(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{    
    int i;
    char *cell = NULL;
    int lhash;
    int ldepth;
    int size = 0;
    char *infix = NULL;
    char *result = NULL;
    char *postfix = NULL;
    char *command = NULL;
    char number[5];

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "library cell_name");
        return TCL_ERROR;
    }

    cell = Tcl_GetString(objv[1]);
    if(cell == NULL)
    {
        printf("ERROR: cell is NULL\n");
        return TCL_ERROR;
    }
    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_libhash_indices(cell, &lhash, &ldepth);
    if(ldepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Cell NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR; 
    }

    if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)
    {
        printf(ANSI_COLOR_RED "ERROR: Component is of type SEQUENTIAL" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    result = NULL;
    for(i = 0; i < libhash[lhash].out_pins_count[ldepth]; i++)
    {
        infix = strdup(libhash[lhash].function[ldepth][i]);
        size = size + strlen(infix) + 3 + strlen("The BDD of  is ") + strlen(" succesfully generated!");

        postfix = parse_infix(infix);

        // generate_bdd(infix, libhash[lhash].name[ldepth]);
        generate_bdd(infix, libhash[lhash].name[ldepth]);

        command = malloc(strlen("dot -Tpng ") + strlen(libhash[lhash].name[ldepth]) + strlen(".dot -o .png  ") + strlen(libhash[lhash].name[ldepth]) + strlen("bdd_output/") + strlen("bdd_output/_ "));
        strcpy(command, "dot -Tpng ");
        strcat(command, "bdd_output/");
        strcat(command, libhash[lhash].name[ldepth]);
        strcat(command, ".dot -o ");
        strcat(command, "bdd_output/");
        strcat(command, libhash[lhash].name[ldepth]);
        strcat(command, "_");

        snprintf(number, strlen(number) + 2, "%d", i+1);
        strcat(command, number);

        strcat(command, ".png");
        system(command);

        #ifdef DEBUG
        printf("command is %s\n", command);
        #endif

        strcpy(command, "xdg-open ");
        strcat(command, "bdd_output/");
        strcat(command, libhash[lhash].name[ldepth]);
        strcat(command, "_");
        strcat(command, number);
        strcat(command, ".png");
        system(command);

        result = my_realloc(result, (size) * sizeof(char));
        if(i == 0)
        {
            strcpy(result, "The BDD of ");
        }
        else
        {
            strcat(result, "The BDD of ");
        }

        strcat(result, infix);
        strcat(result, " succesfully generated!\n");

        free(infix);
        free(postfix);
        free(command);
    }
    
    Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));

    free(result);

    return TCL_OK;

}

int report_component_BDD(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    char *component = NULL;
    int chash, cdepth;
    int lhash, ldepth;
    int i;
    int size = 0;
    char *infix = NULL;
    char *result = NULL;
    char *postfix = NULL;
    char *command = NULL;
    char number[5];

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "component_name");
        return TCL_ERROR;
    }

    component = Tcl_GetString(objv[1]);
    if(component == NULL)
    {
        printf("ERROR: Component is NULL\n");
        return TCL_ERROR;
    }
    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_comphash_indices(component, &chash, &cdepth);
    if(cdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Component NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR; 
    }

    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];
    if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)
    {
        printf(ANSI_COLOR_RED "ERROR: Component is of type SEQUENTIAL" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    result = NULL;
    for(i = 0; i < libhash[lhash].out_pins_count[ldepth]; i++)
    {
        infix = strdup(libhash[lhash].function[ldepth][i]);
        size = size + strlen(infix) + 3 + strlen("The BDD of  is ") + strlen(" succesfully generated!");

        postfix = parse_infix(infix);

        // generate_bdd(infix, libhash[lhash].name[ldepth]);
        generate_bdd(infix, libhash[lhash].name[ldepth]);

        command = malloc(strlen("dot -Tpng ") + strlen(libhash[lhash].name[ldepth]) + strlen(".dot -o .png  ") + strlen(libhash[lhash].name[ldepth]) + strlen("bdd_output/") + strlen("bdd_output/_ "));
        strcpy(command, "dot -Tpng ");
        strcat(command, "bdd_output/");
        strcat(command, libhash[lhash].name[ldepth]);
        strcat(command, ".dot -o ");
        strcat(command, "bdd_output/");
        strcat(command, libhash[lhash].name[ldepth]);
        strcat(command, "_");

        snprintf(number, strlen(number) + 2, "%d", i+1);
        strcat(command, number);

        strcat(command, ".png");
        system(command);
        printf("command is %s\n", command);

        strcpy(command, "xdg-open ");
        strcat(command, "bdd_output/");
        strcat(command, libhash[lhash].name[ldepth]);
        strcat(command, "_");
        strcat(command, number);
        strcat(command, ".png");
        system(command);

        result = my_realloc(result, (size) * sizeof(char));
        if(i == 0)
        {
            strcpy(result, "The BDD of ");
        }
        else
        {
            strcat(result, "The BDD of ");
        }

        strcat(result, infix);
        strcat(result, " succesfully generated!\n");

        free(infix);
        free(postfix);
        free(command);
    }
    
    Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));

    free(result);

    return TCL_OK;
}

int compute_expression_BDD(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int size = 0;
    char *infix = NULL;
    char *result = NULL;
    char *postfix = NULL;
    char *command = NULL;
    char filename[] = "currDot";

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "expression");
        return TCL_ERROR;
    }

    infix = Tcl_GetString(objv[1]);
    if(infix == NULL)
    {
        printf("ERROR: Infix is NULL\n");
        return TCL_ERROR;
    }

    result = NULL;

    size = size + strlen(infix) + 3 + strlen("The BDD of  is ") + strlen(" succesfully generated!");

    postfix = parse_infix(infix);
    if(postfix == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: Expression has not correct format\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    // generate_bdd(infix, filename);
    generate_bdd(infix, filename);

    command = malloc(strlen("dot -Tpng ") + strlen(filename) + strlen(".dot -o .png  ") + strlen(filename) + strlen("bdd_output/") + strlen("bdd_output/_ "));
    strcpy(command, "dot -Tpng ");
    strcat(command, "bdd_output/");
    strcat(command, filename);
    strcat(command, ".dot -o ");
    strcat(command, "bdd_output/");
    strcat(command, filename);

    strcat(command, ".png");
    system(command);
    printf("command is %s\n", command);

    strcpy(command, "xdg-open ");
    strcat(command, "bdd_output/");
    strcat(command, filename);
    strcat(command, ".png");
    system(command);

    result = my_realloc(result, (size) * sizeof(char));

    strcpy(result, "The BDD of ");


    strcat(result, infix);
    strcat(result, " succesfully generated!\n");

    free(postfix);
    free(command);

    
    Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));

    free(result);

    return TCL_OK;
}

int get_toposort(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    add_startpoints();  // call toposort //

    isLevelized = 1;

    return TCL_OK;
}

int get_predecessor_pin(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int ghash;
    int gdepth;
    char *gatepin = NULL;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "gatepin-name");
        return TCL_ERROR;
    }

    gatepin = Tcl_GetString(objv[1]);
    if(gatepin == NULL)
    {
        printf("ERROR: Gatepin is NULL\n");
        return TCL_ERROR;
    }
    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_predecessors_pin(gatepin, &ghash, &gdepth);
    if(gdepth == -1)
    {
        printf(ANSI_COLOR_ORANGE "ERROR: Gatepin has not predecessors" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    printf(ANSI_COLOR_ORANGE "Gatepin %s has predecessor %s\n" ANSI_COLOR_RESET, gatepin, gatepinhash[ghash].name[gdepth]);

    return TCL_OK;
}

int report_gatepins_levelized(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    int j;
    int max_level;

    if(objc != 1)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "nothing");
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if(isLevelized == 0)    // check if design is levelized //
    {
        printf(ANSI_COLOR_ORANGE "ERROR: Design is not levelized\nCall get_toposort to levelize design!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    max_level = 0;

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                max_level = max(max_level, gatepinhashv[i].level[j]);
            }
        }
    }

    for (int level = -1; level <= max_level; level++) 
    {
        printf(ANSI_COLOR_MAGENDA "------ Level %d: ------\n" ANSI_COLOR_RESET, level);
        for (i = 0; i < gatepinhash_size; i++) 
        {
            for(j = 0; j < HASHDEPTH; j++)
            {
                if(gatepinhash[i].hashpresent[j] != 0)
                {
                    if (gatepinhashv[i].level[j] == level) 
                    {
                        printf(ANSI_COLOR_GREEN "• %s\n" ANSI_COLOR_RESET, gatepinhash[i].name[j]);
                    }
                }
            }
        }  
        if(level > 9)
        {
            printf(ANSI_COLOR_MAGENDA "-----------------------\n\n" ANSI_COLOR_RESET);
        }
        else
        {
            printf(ANSI_COLOR_MAGENDA "----------------------\n\n" ANSI_COLOR_RESET);
        }
    }
    printf(ANSI_COLOR_BLUE "INFO: Max depth of design is: %d\n" ANSI_COLOR_RESET, max_level);

    return TCL_OK;
}


int report_level_gatepins(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    int j;
    int level;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "<level>");
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if(isLevelized == 0)    // check if design is levelized //
    {
        printf(ANSI_COLOR_ORANGE "ERROR: Design is not levelized\nCall get_toposort to levelize design!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    level = atoi(Tcl_GetString(objv[1]));

    if(level < -1 || level > max_design_level)
    {
        printf(ANSI_COLOR_ORANGE "Warning: Level %d out of bounds\n" ANSI_COLOR_RESET, level);
        return TCL_ERROR;
    }


    printf(ANSI_COLOR_MAGENDA "------ Level %d: ------\n" ANSI_COLOR_RESET, level);
    for (i = 0; i < gatepinhash_size; i++) 
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if (gatepinhashv[i].level[j] == level) 
                {
                    printf(ANSI_COLOR_GREEN "• %s\n" ANSI_COLOR_RESET, gatepinhash[i].name[j]);
                }
            }
        }
    }  
    if(level > 9 || level < 0)
    {
        printf(ANSI_COLOR_MAGENDA "-----------------------\n\n" ANSI_COLOR_RESET);
    }
    else
    {
        printf(ANSI_COLOR_MAGENDA "----------------------\n\n" ANSI_COLOR_RESET);
    }

    return TCL_OK;
}

int report_gatepin_level(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i;
    int level;
    int ghash;
    int gdepth;
    char **gatepins_name = NULL;
    char *list_gatepins = NULL;
    int list_len;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "<gatepins_list>");
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if(isLevelized == 0)    // check if design is levelized //
    {
        printf(ANSI_COLOR_ORANGE "ERROR: Design is not levelized\nCall get_toposort to levelize design!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    list_gatepins = Tcl_GetString(objv[1]);

    if(Tcl_SplitList(interp, (const char *) list_gatepins, &list_len,  (const char ***) &gatepins_name) != TCL_OK)
    {
        return TCL_ERROR;
    }
    for(i = 0; i < list_len; i++)
    {
        get_gatepin_indices(gatepins_name[i], &ghash, &gdepth);
        if(gdepth == -1)
        {
            printf(ANSI_COLOR_RED "ERROR: Gatepin %s NOT found" ANSI_COLOR_RESET, gatepins_name[i]);
            return TCL_ERROR;
        }
    }

    for(i = 0; i < list_len; i++)
    {
        get_gatepin_indices(gatepins_name[i], &ghash, &gdepth);
        level = gatepinhashv[ghash].level[gdepth];
        printf(ANSI_COLOR_GREEN "• gatepin %s has level %d\n" ANSI_COLOR_RESET, gatepins_name[i], level);
    }   

    return TCL_OK;
}

int annotate_bdd(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    annotate_bdds();

    return TCL_OK;
}

int report_bdd_dot_gatepin(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    char *gatepin = NULL;
    char *filename = NULL;
    int ghash;
    int gdepth;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    FILE *dotfile;
    DdNode *temp_node = NULL;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "gatepin");
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    gatepin = Tcl_GetString(objv[1]);
    if(gatepin == NULL)
    {
        printf("ERROR: Gatepin is NULL\n");
        return TCL_ERROR;
    }

    get_gatepin_indices(gatepin, &ghash, &gdepth);
    if(gdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Gatepin NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if(check_gatepin_type(ghash, gdepth) == 0)  // it is input //
    {
        printf(ANSI_COLOR_RED "ERROR: Gatepin is input" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    if(gatepinhash[ghash].type[gdepth] == PO)
    {
        get_predecessors_pin(gatepinhash[ghash].name[gdepth], &ghash, &gdepth);
    }

    chash = gatepinhash[ghash].parentComponent[gdepth];
    cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];
    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];

    filename = malloc(strlen(libhash[lhash].name[ldepth]) + strlen(".dot") + strlen("bdd_output/") + 1);
    strcpy(filename, "bdd_output/");
    strcat(filename, libhash[lhash].name[ldepth]);
    strcat(filename, ".dot");

    dotfile = fopen(filename, "w");

    temp_node = gatepinhashv[ghash].gatepin_bdd[gdepth];
    temp_node = Cudd_BddToAdd(gbm, temp_node);    
    char *convert_dot = malloc( (2 * strlen(libhash[lhash].name[ldepth]) ) + strlen("dot -Tpng -Gpdi=1000 -o bdd_output/ bdd_output/ .png .dot") + 5);

    Cudd_DumpDot(gbm , 1, &temp_node, (const char **) NamesDot, NULL, dotfile);
    fclose(dotfile);
    
    printf(ANSI_COLOR_GREEN "Done writing dot file for gatepin %s\n" ANSI_COLOR_RESET, gatepin);

    strcpy(convert_dot, "dot -Tpng -Gdpi=1000 -o bdd_output/");
    strcat(convert_dot, libhash[lhash].name[ldepth]);
    strcat(convert_dot, ".png bdd_output/");
    strcat(convert_dot, libhash[lhash].name[ldepth]);
    strcat(convert_dot, ".dot");

    system(convert_dot);

    strcpy(convert_dot, "xdg-open bdd_output/");
    strcat(convert_dot, libhash[lhash].name[ldepth]);
    strcat(convert_dot, ".png");

    system(convert_dot);

    free(convert_dot);
    free(filename);

    printf(ANSI_COLOR_BLUE "Number of Minterms for gatepin %s is %lf\n" ANSI_COLOR_RESET, gatepin, Cudd_CountMinterm(gbm, gatepinhashv[ghash].gatepin_bdd[gdepth], Cudd_ReadSize(gbm)));
    printf(ANSI_COLOR_BLUE "Number of paths to 1 for gatepin %s is %lf\n" ANSI_COLOR_RESET, gatepin, Cudd_CountPathsToNonZero(gatepinhashv[ghash].gatepin_bdd[gdepth]));

    return TCL_OK;
}


int report_bdd_dot_component(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    char *component = NULL;
    char *filename = NULL;
    int ghash;
    int gdepth;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    int i;
    FILE *dotfile;
    DdNode *temp_node = NULL;
    char *gatepin_name = NULL;
    char number_string[15];

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "component");
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    component = Tcl_GetString(objv[1]);
    if(component == NULL)
    {
        printf("ERROR: component is NULL\n");
        return TCL_ERROR;
    }

    get_comphash_indices(component, &chash, &cdepth);
    if(chash == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Component NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    
    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];

    if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)
    {
        printf(ANSI_COLOR_RED "ERROR: Component is of type SEQUENTIAL" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if(isLevelized == 0)    // check if design is levelized //
    {
        printf(ANSI_COLOR_ORANGE "ERROR: Design is not levelized\nCall get_toposort to levelize design!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    for(i = 0; i < libhash[lhash].pin_count[ldepth]; i++)
    {
        gatepin_name = (char *) my_calloc(strlen(comphash[chash].name[cdepth]) + strlen(libhash[lhash].pin_names[ldepth][i]) + 1, sizeof(char));
        strcpy(gatepin_name, comphash[chash].name[cdepth]);
        strcat(gatepin_name, libhash[lhash].pin_names[ldepth][i]);
        get_gatepin_indices(gatepin_name, &ghash, &gdepth);

        if(check_gatepin_type(ghash, gdepth) == 0)  // it is input //
        {
            free(gatepin_name);
            gatepin_name = NULL;
            continue;
        }

        filename = malloc(strlen(libhash[lhash].name[ldepth]) + strlen(".dot") + strlen("bdd_output/") + 3);
        strcpy(filename, "bdd_output/");
        strcat(filename, libhash[lhash].name[ldepth]);
        strcat(filename, "_");
        sprintf(number_string, "%d", i);
        strcat(filename, number_string);
        strcat(filename, ".dot");

        dotfile = fopen(filename, "w");

        temp_node = gatepinhashv[ghash].gatepin_bdd[gdepth];
        temp_node = Cudd_BddToAdd(gbm, temp_node);    
        char *convert_dot = malloc( (2 * strlen(libhash[lhash].name[ldepth]) ) + strlen("dot -Tpng -Gpdi=1000 -o bdd_output/ bdd_output/ .png .dot") + 8);

        Cudd_DumpDot(gbm , 1, &temp_node, (const char **) NamesDot, NULL, dotfile);
        fclose(dotfile);
        
        printf(ANSI_COLOR_GREEN "Done writing dot file for gatepin %s\n" ANSI_COLOR_RESET, gatepin_name);

        strcpy(convert_dot, "dot -Tpng -Gdpi=1000 -o bdd_output/");
        strcat(convert_dot, libhash[lhash].name[ldepth]);
        strcat(convert_dot, "_");
        strcat(convert_dot, number_string);
        strcat(convert_dot, ".png bdd_output/");
        strcat(convert_dot, libhash[lhash].name[ldepth]);
        strcat(convert_dot, "_");
        strcat(convert_dot, number_string);
        strcat(convert_dot, ".dot");

        system(convert_dot);

        strcpy(convert_dot, "xdg-open bdd_output/");
        strcat(convert_dot, libhash[lhash].name[ldepth]);
        strcat(convert_dot, "_");
        strcat(convert_dot, number_string);
        strcat(convert_dot, ".png");

        system(convert_dot);
   
        printf(ANSI_COLOR_BLUE "Number of Minterms for gatepin %s is %lf\n" ANSI_COLOR_RESET, gatepin_name, Cudd_CountMinterm(gbm, gatepinhashv[ghash].gatepin_bdd[gdepth], Cudd_ReadSize(gbm)));
        printf(ANSI_COLOR_BLUE "Number of paths to 1 for gatepin %s is %lf\n" ANSI_COLOR_RESET, gatepin_name, Cudd_CountPathsToNonZero(gatepinhashv[ghash].gatepin_bdd[gdepth]));
        
        free(convert_dot);
        free(filename);
        free(gatepin_name);
        convert_dot = NULL;
        filename = NULL;
        gatepin_name = NULL;
    }


    return TCL_OK;
}

int findparameter(int objc, Tcl_Obj *const* objv, char *parameter)
{
    int i;
    for(i = 0; i < objc; i++)
    {
        if(strcmp(Tcl_GetString(objv[i]), parameter) == 0)
        {
            return i;
        }
    }
    return 0;
}

int findparametermain(int argc, char *argv[], char *parameter)
{
    int i;
    for(i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], parameter) == 0)
        {
            return i;
        }
    }
    return 0;
}

int set_static_probability(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int value_found;
    int allstartpoints_found;
    int gatepins_found;
    int i;
    int j;
    char *list_gatepins = NULL;
    char **gatepins_name = NULL;
    int list_size;
    int ghash;
    int gdepth;
    double value = 0.0;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;


    if(objc < 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "-value <probability value> -gatepins {gatepins list} | -allstartpoints");
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    value_found = findparameter(objc, objv, "-value");
    allstartpoints_found = findparameter(objc, objv, "-allstartpoints");
    gatepins_found = findparameter(objc, objv, "-gatepins");

    if(gatepins_found >= 1 && allstartpoints_found >= 1)
    {
        printf(ANSI_COLOR_RED "ERROR: -gatepins and -allstartpoints cannot be assigned in same command\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    if(gatepins_found == 0 && allstartpoints_found == 0)
    {
        printf(ANSI_COLOR_RED "ERROR: -gatepins or -allstartpoints parameter is obligatory\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    if(value_found == 0)
    {
        printf(ANSI_COLOR_RED "ERROR: -value parameter is obligatory\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    value = atof(Tcl_GetString(objv[value_found + 1]));
    if(value < 0 || value > 1)
    {
        printf(ANSI_COLOR_RED "ERROR: Probability value out of bounds\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if(isLevelized == 0)    // check if design is levelized //
    {
        printf(ANSI_COLOR_ORANGE "ERROR: Design is not levelized\nCall get_toposort to levelize design!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    probability_calculated = 0;

    if(allstartpoints_found > 0)
    {
        for(i = 0; i < gatepinhash_size; i++)
        {
            for(j = 0; j < HASHDEPTH; j++)
            {
                if(gatepinhash[i].hashpresent[j] != 0)
                {   
                    if(gatepinhashv[i].level[j] == 0)
                    {
                        if(gatepinhash[i].type[j] == WIRE)
                        {
                            chash = gatepinhash[i].parentComponent[j];
                            cdepth = gatepinhash[i].parentComponentDepth[j];
                            lhash = comphash[chash].lib_type[cdepth];
                            ldepth = comphash[chash].lib_type_depth[cdepth];
                            if(strcmp(libhash[lhash].function[ldepth][0], "1") == 0)
                            {
                                continue;
                            }
                            if(strcmp(libhash[lhash].function[ldepth][0], "0") == 0)
                            {
                                continue;
                            }
                        }

                        gatepinhash_prob[i].one_prob[j] = value;
                        gatepinhash_prob[i].zero_prob[j] = 1 - value;
                    }
                }
            }
        }
    }

    if(gatepins_found > 0)
    {
        list_gatepins = Tcl_GetString(objv[gatepins_found + 1]);
        if(Tcl_SplitList(interp, (const char *) list_gatepins, &list_size,  (const char ***) &gatepins_name) != TCL_OK)
        {
            return TCL_ERROR;
        }
        for(i = 0; i < list_size; i++)
        {
            get_gatepin_indices(gatepins_name[i], &ghash, &gdepth);
            if(gdepth == -1)
            {
                printf(ANSI_COLOR_ORANGE "Warning: Gatepin %s NOT found\n" ANSI_COLOR_RESET, gatepins_name[i]);
                continue;
            }
            if(gatepinhashv[ghash].level[gdepth] != 0)
            {
                printf(ANSI_COLOR_ORANGE "Warning: Gatepin %s is not a startpoint\n" ANSI_COLOR_RESET, gatepins_name[i]);
                continue;
            }
            gatepinhash_prob[ghash].one_prob[gdepth] = atof(Tcl_GetString(objv[value_found + 1]));
            gatepinhash_prob[ghash].zero_prob[gdepth] = 1 - gatepinhash_prob[ghash].one_prob[gdepth];
        }
    }

    printf(ANSI_COLOR_GREEN "Static probabilities updated\n" ANSI_COLOR_RESET);
    
    return TCL_OK;
}

int list_static_probability(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int allgatepins_found;
    int gatepins_found;
    int i;
    int j;
    char *list_gatepins = NULL;
    char **gatepins_name = NULL;
    int list_size;
    int ghash;
    int gdepth;
    st_table *table = NULL;
    clock_t start, end;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;


    if(objc < 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "-gatepins {gatepins list} | -allgatepins");
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    allgatepins_found = findparameter(objc, objv, "-allgatepins");
    gatepins_found = findparameter(objc, objv, "-gatepins");

    if(gatepins_found >= 1 && allgatepins_found >= 1)
    {
        printf(ANSI_COLOR_RED "ERROR: -gatepins and -allgatepins cannot be assigned in same command\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    if(gatepins_found == 0 && allgatepins_found == 0)
    {
        printf(ANSI_COLOR_RED "ERROR: -gatepins or -allgatepins parameter is obligatory\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    if(gatepins_found > 0)
    {
        if(objc <= 2)
        {
            printf(ANSI_COLOR_RED "ERROR: -gatepins needs a TCL list\n" ANSI_COLOR_RESET);
            return TCL_ERROR;
        }
    }

    if(isLevelized == 0)    // check if design is levelized //
    {
        printf(ANSI_COLOR_ORANGE "ERROR: Design is not levelized\nCall get_toposort to levelize design!" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    printf(ANSI_COLOR_ORANGE "---------------- Static Probabilities ----------------\n" ANSI_COLOR_RESET);
    // printf(ANSI_COLOR_ORANGE "Gatepin Name          One Probability          Zero Probability\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_MAGENDA "%-20s%-12s%-12s%-12s\n", "Gatepin Name", "Level", "Logic-1", "Logic-0" ANSI_COLOR_RESET);

    nodes_array = realloc(nodes_array, (ghash_added_size + 1) * sizeof(int) );

    // #ifdef DEBUG
    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if (check_gatepin_type(i, j) == 1)
                {
                    if(gatepinhashv[i].gatepin_bdd[j] == NULL)
                    {
                        printf("it is %s\n", gatepinhash[i].name[j]);
                    }
                }
            }
        }
    }  
    // #endif 


    start = clock();

    if(allgatepins_found > 0)
    {
        for(i = 0; i < gatepinhash_size; i++)
        {
            for(j = 0; j < HASHDEPTH; j++)
            {
                if(gatepinhash[i].hashpresent[j] != 0)
                {   
                    if(check_gatepin_type(i, j) == 1)
                    {
                        chash = gatepinhash[i].parentComponent[j];
                        cdepth = gatepinhash[i].parentComponentDepth[j];
                        lhash = comphash[chash].lib_type[cdepth];
                        ldepth = comphash[chash].lib_type_depth[cdepth];
                        if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)  // if it is Sequential just skip it //
                        {
                            continue;
                        }
                        // if(gatepinhash[i].type[j] == PO)
                        // {
                        //     get_predecessors_pin(gatepinhash[i].name[j], &i, &j);
                        // }
                        if(gatepinhashv[i].gatepin_bdd[j] == NULL)
                        {
                            printf(ANSI_COLOR_RED "ERROR: BDD for gatepin %s is not generated\nBe sure to run annotate_bdd command" ANSI_COLOR_RESET, gatepinhash[i].name[j]);
                            return TCL_ERROR;
                        }
                        // write_minterms(i, j);    // calculate probabilities //
                        // read_minterms(gatepinhash[i].name[j]);    // calculate probabilities //
                        for(int i = 0; i < ghash_added_size; i++)
                        {
                            nodes_array[i] = -1;
                        }
                        probability_gatepin = 0;
                        
                        if(probability_calculated != 1)
                        {
                            table = st_init_table(st_ptrcmp, st_ptrhash);
                            my_ddCountPathsToNonZero(Cudd_BddToAdd(gbm, gatepinhashv[i].gatepin_bdd[j]), table);
                            gatepinhash_prob[i].one_prob[j] = probability_gatepin;
                            gatepinhash_prob[i].zero_prob[j] = 1 - probability_gatepin;
                            probability_gatepin = 0;
                            st_free_table(table);
                        }
                        // printf(ANSI_COLOR_GREEN "• Gatepin %s          %lf          %lf\n" ANSI_COLOR_RESET, gatepinhash[i].name[j], gatepinhash_prob[i].one_prob[j], gatepinhash_prob[i].zero_prob[j]);
                        printf(ANSI_COLOR_GREEN "• %-20s%-12d%-12.6lf%-12.6lf\n" ANSI_COLOR_RESET, gatepinhash[i].name[j], gatepinhashv[i].level[j], gatepinhash_prob[i].one_prob[j], gatepinhash_prob[i].zero_prob[j]);
                    }
                }
            }
        }
    }

    if(gatepins_found > 0)
    {
        list_gatepins = Tcl_GetString(objv[gatepins_found + 1]);
        if(Tcl_SplitList(interp, (const char *) list_gatepins, &list_size,  (const char ***) &gatepins_name) != TCL_OK)
        {
            return TCL_ERROR;
        }
        for(i = 0; i < list_size; i++)
        {
            get_gatepin_indices(gatepins_name[i], &ghash, &gdepth);
            if(gdepth == -1)
            {
                printf(ANSI_COLOR_ORANGE "Warning: Gatepin %s NOT found\n" ANSI_COLOR_RESET, gatepins_name[i]);
                continue;
            }
            if(check_gatepin_type(ghash, gdepth) != 1)
            {
                printf(ANSI_COLOR_ORANGE "Warning: Gatepin %s is not an Output\n" ANSI_COLOR_RESET, gatepins_name[i]);
                continue;
            }
            
            chash = gatepinhash[ghash].parentComponent[gdepth];
            cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];
            lhash = comphash[chash].lib_type[cdepth];
            ldepth = comphash[chash].lib_type_depth[cdepth];
            if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)  // if it is Sequential just skip it //
            {
                printf(ANSI_COLOR_ORANGE "Warning: Gatepin %s is of type SEQUENTIAL. Skipping...\n" ANSI_COLOR_RESET, gatepins_name[i]);
                continue;
            }
            if(gatepinhashv[ghash].gatepin_bdd[gdepth] == NULL)
            {
                printf(ANSI_COLOR_RED "ERROR: BDD for gatepin %s is not generated\nBe sure to run annotate_bdd command" ANSI_COLOR_RESET, gatepinhash[ghash].name[gdepth]);
                return TCL_ERROR;
            }
            // write_minterms(ghash, gdepth);
            // read_minterms(gatepinhash[ghash].name[gdepth]);    // calculate probabilities //
            for(int i = 0; i < ghash_added_size; i++)
            {
                nodes_array[i] = -1;
            }
            probability_gatepin = 0;

            table = st_init_table(st_ptrcmp, st_ptrhash);

            my_ddCountPathsToNonZero(Cudd_BddToAdd(gbm, gatepinhashv[ghash].gatepin_bdd[gdepth]), table);
            gatepinhash_prob[ghash].one_prob[gdepth] = probability_gatepin;
            gatepinhash_prob[ghash].zero_prob[gdepth] = 1 - probability_gatepin;
            probability_gatepin = 0;
            st_free_table(table);
            printf(ANSI_COLOR_GREEN "• %-20s%-12d%-12.6lf%-12.6lf\n" ANSI_COLOR_RESET, gatepinhash[ghash].name[gdepth], gatepinhashv[ghash].level[gdepth], gatepinhash_prob[ghash].one_prob[gdepth], gatepinhash_prob[ghash].zero_prob[gdepth]);
        }
    }

    printf(ANSI_COLOR_ORANGE "-------------- End Static Probabilities --------------\n\n" ANSI_COLOR_RESET);
    
    probability_calculated = 1;
    // free(nodes_array);

    end = clock();
    printf(ANSI_COLOR_ORANGE "Calculation time is %lf\n" ANSI_COLOR_RESET, (double)(end - start) / CLOCKS_PER_SEC);

    // what should i do on flip-flops and bdds annotation? just skip it? //

    return TCL_OK;  
}

int get_traverse_cudd(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    char *gatepin = NULL;
    int ghash;
    int gdepth;
    st_table	*table;
    clock_t start, end;

    start = clock();

    path = NULL;
    all_paths = NULL;
    path_size = 0;
    all_paths_size = 0;

    if(objc != 2)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "gatepin");
        return TCL_ERROR;
    }

    if(comphash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    gatepin = Tcl_GetString(objv[1]);
    if(gatepin == NULL)
    {
        printf("ERROR: Gatepin is NULL\n");
        return TCL_ERROR;
    }

    get_gatepin_indices(gatepin, &ghash, &gdepth);
    if(gdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Gatepin NOT found" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if(check_gatepin_type(ghash, gdepth) == 0)  // it is input //
    {
        printf(ANSI_COLOR_RED "ERROR: Gatepin is input" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    number_of_paths = 0;
    nodes_array = realloc(nodes_array, (ghash_added_size + 1) * sizeof(int) );
    for(int i = 0; i < ghash_added_size; i++)
    {
        nodes_array[i] = -1;
    }
    probability_gatepin = 0;
    // traverse_cudd(Cudd_BddToAdd(gbm, gatepinhashv[ghash].gatepin_bdd[gdepth]));

    // print_paths();

    table = st_init_table(st_ptrcmp, st_ptrhash);

    // printf("Testing function paths is %lf\n", my_ddCountPathsToNonZero(Cudd_BddToAdd(gbm, gatepinhashv[ghash].gatepin_bdd[gdepth]), table));
    // printf("Testing function paths is %lf\n", my_ddCountPathsToNonZero(gatepinhashv[ghash].gatepin_bdd[gdepth], table));
    // printf("Total paths are %lf\n", number_of_paths);
    printf("Paths from system are %lf\n", Cudd_CountPathsToNonZero(gatepinhashv[ghash].gatepin_bdd[gdepth]));
    printf("Total probability is %lf\n", probability_gatepin);
    // Cudd_PrintMinterm(gbm, gatepinhashv[ghash].gatepin_bdd[gdepth]);
    printInorder(Cudd_Regular(gatepinhashv[ghash].gatepin_bdd[gdepth]));

    st_free_table(table);

    end = clock();
    printf(ANSI_COLOR_ORANGE "Calculation time is %lf\n" ANSI_COLOR_RESET, (double)(end - start) / CLOCKS_PER_SEC);

    return TCL_OK;
}

int show_gui(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    // pthread_t gui_thread;
    // start_gui();
    pthread_create(&gui_thread, NULL, start_gui, NULL);

    // pthread_join(gui_thread, NULL);

    return TCL_OK;
}

int hide_gui(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    // gtk_main_quit();
    // pthread_cancel(gui_thread);
    gtk_widget_hide(mainwindow);
    // pthread_join(gui_thread, NULL);

    return TCL_OK;
}

int report_coresite(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    dump_coresite();

    return TCL_OK;
}

int list_rows(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    dump_rows();

    return TCL_OK;
}

int highligth_component(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    int i, j;
    int chash, cdepth;
    int lhash, ldepth;
    int ghash, gdepth;
    int gconhash, gcondepth;
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

    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    get_comphash_indices(currComp, &chash, &cdepth);
    if(cdepth == -1)
    {
        printf(ANSI_COLOR_RED "ERROR: Component %s does not exists!" ANSI_COLOR_RESET, currComp);
        return TCL_ERROR;
    }

    highlighted_component = my_calloc(strlen(currComp) + 1, sizeof(char));
    strcpy(highlighted_component, currComp);

    return TCL_OK;
}

int list_gatepins(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    dump_gatepinhash();

    return TCL_OK;
}

int report_hpwl(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    double hpwl = 0.0;

    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    if(design_is_placed == 0)
    {
        printf(ANSI_COLOR_RED "ERROR: Design is not placed\n" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }
    
    hpwl = calculate_HPWL();

    printf(ANSI_COLOR_BLUE "INFO: HPWL is %lf\n" ANSI_COLOR_RESET, hpwl);

    return TCL_OK;
}

int place_random(ClientData clientdata, Tcl_Interp *interp, int objc, Tcl_Obj *const* objv)
{
    if(gatepinhash == NULL)
    {
        printf(ANSI_COLOR_RED "ERROR: No design loaded" ANSI_COLOR_RESET);
        return TCL_ERROR;
    }

    random_placer();

    design_is_placed = 1;

    printf(ANSI_COLOR_BLUE "INFO: Design placed randomly\n" ANSI_COLOR_RESET);

    return TCL_OK;
}

void *main_tcl(void *arg)
{
    char *text = NULL; // readline result //
    char *textexpansion; // readline result history expanded //
    int expansionresult;
    // int kati = 0;
    const char *directory = "bdd_output";

    interp = Tcl_CreateInterp();

    HIST_ENTRY **the_history_list; // readline commands history list - NULL terminated //
    char command[LINE_MAX]; // current command //
    unsigned long i;
    // Readline Initialisation //
    rl_completion_entry_function = NULL; // use rl_filename_completion_function(), the default filename completer //
    rl_attempted_completion_function = custom_completer;
    rl_completion_append_character = '\0';  // should not apply ' ' in the end of word //
    using_history(); // initialise history functions //

    /* Create all system calls */
    Tcl_CreateObjCommand(interp, "ls", ls_command, NULL, NULL);
    Tcl_CreateObjCommand(interp, "less", less_command, NULL, NULL);
    Tcl_CreateObjCommand(interp, "man", man_command, NULL, NULL);

    /* Create all custom TCl commands */
    Tcl_CreateObjCommand(interp, "read_design", read_design, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_IOs", list_IO, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_components", list_components, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_component_function", report_component_function, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_component_type", report_component_type, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_component_CCS", list_component_CCS, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_IO_CCS", list_IO_CCS, NULL, NULL);
    Tcl_CreateObjCommand(interp, "clear_design", clear_design, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_cell", list_cell, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_cells", list_cells, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_component_info", list_component_info, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_components_info", list_components_info, NULL, NULL);
    Tcl_CreateObjCommand(interp, "convert_infix_to_postfix", convert_infix_to_postfix, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_component_postfix_boolean_function", report_component_postfix_boolean_function, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_component_BDD", report_component_BDD, NULL, NULL);
    Tcl_CreateObjCommand(interp, "compute_expression_BDD", compute_expression_BDD, NULL, NULL);
    Tcl_CreateObjCommand(interp, "get_toposort", get_toposort, NULL, NULL);
    Tcl_CreateObjCommand(interp, "get_predecessor_pin", get_predecessor_pin, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_gatepins_levelized", report_gatepins_levelized, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_level_gatepins", report_level_gatepins, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_gatepin_level", report_gatepin_level, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_library_cell_BDD", report_library_cell_BDD, NULL, NULL);
    Tcl_CreateObjCommand(interp, "annotate_bdd", annotate_bdd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_bdd_dot_gatepin", report_bdd_dot_gatepin, NULL, NULL);
    Tcl_CreateObjCommand(interp, "get_traverse_cudd", get_traverse_cudd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "set_static_probability", set_static_probability, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_static_probability", list_static_probability, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_bdd_dot_component", report_bdd_dot_component, NULL, NULL);
    Tcl_CreateObjCommand(interp, "show_gui", show_gui, NULL, NULL);
    Tcl_CreateObjCommand(interp, "hide_gui", hide_gui, NULL, NULL);

    // TCL commands for placement //
    Tcl_CreateObjCommand(interp, "report_coresite", report_coresite, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_rows", list_rows, NULL, NULL);
    Tcl_CreateObjCommand(interp, "highligth_component", highligth_component, NULL, NULL);
    Tcl_CreateObjCommand(interp, "list_gatepins", list_gatepins, NULL, NULL);
    Tcl_CreateObjCommand(interp, "report_hpwl", report_hpwl, NULL, NULL);
    Tcl_CreateObjCommand(interp, "place_random", place_random, NULL, NULL);

    signal(SIGSEGV, segfault_handler);
    signal(SIGINT, sigint_handler);
    
    DIR *dir = opendir(directory);
    
    if (dir) 
    {
        // Directory exists
        closedir(dir);
        #ifdef DEBUG
        printf("Directory \"%s\" already exists.\n", directory);
        #endif
    } 
    else 
    {
        // Directory doesn't exist, so create it
        if (mkdir(directory, 0777) == 0) 
        {
            #ifdef DEBUG
            printf("Directory \"%s\" created successfully.\n", directory);
            #endif
        } 
        else 
        {
            #ifdef DEBUG
            printf("Error creating directory \"%s\".\n", directory);
            #endif
            return 1; // Return an error code
        }
    }

    exit_requested = false;
    ctrl_c_pressed = false;
    isLevelized = 0;
    max_design_level = -2;

    // const char *ascii_art = "   ____    _    ____    _____ ___   ___  _     \n"
    //     "  / ___|  / \\  |  _ \\  |_   _/ _ \\ / _ \\| |    \n"
    //     " | |     / _ \\ | | | |   | || | | | | | | |    \n"
    //     " | |___ / ___ \\| |_| |   | || |_| | |_| | |___ \n"
    //     "  \\____/_/   \\_\\____/    |_| \\___/ \\___/|_____|\n";

    // printf("%s\n\n\n", ascii_art);

    while (!exit_requested)
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
        if (strcmp(command, "quit") == 0 || ctrl_c_pressed)
        {
            // const char *folder = "bdd_output";
            // if(isFolderEmpty(folder) == 1)
            // {
            //     rmdir(folder);
            // }
            removeFolder("bdd_output");
            Tcl_DeleteInterp(interp);
            if(gatepinhash != NULL)
            {
                structs_free();
            }
            printf(ANSI_COLOR_BLUE BOLD_LETTERS "EDA TOOL EXITING\n" ANSI_COLOR_RESET);
            Tcl_Finalize();

            pthread_mutex_destroy(&mutex);

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


int main(int argc, char *argv[])
{
    pthread_t tcl_thread;
    pthread_t gui_thread;

    int hideguiindex = 0;
    int helpindex = 0;
    int findex = 0;

    // pthread_create(&tcl_thread, NULL, main_tcl, NULL);
    // pthread_join(tcl_thread, NULL);

    const char *ascii_art = "   ____    _    ____    _____ ___   ___  _     \n"
    "  / ___|  / \\  |  _ \\  |_   _/ _ \\ / _ \\| |    \n"
    " | |     / _ \\ | | | |   | || | | | | | | |    \n"
    " | |___ / ___ \\| |_| |   | || |_| | |_| | |___ \n"
    "  \\____/_/   \\_\\____/    |_| \\___/ \\___/|_____|\n";

    printf("%s\n\n\n", ascii_art);

    hideguiindex = findparametermain(argc, argv, "-hide_gui");
    helpindex = findparametermain(argc, argv, "-help");
    findex = findparametermain(argc, argv, "-f");

    if(helpindex > 0)
    {
        help_command();
    }
    if(findex > 0)
    {
        char *filename = argv[findex + 1];
        call_parser(filename);
    }
    if(hideguiindex > 0)
    {
        main_tcl(NULL);
        // return 1;   // no gui //
    }

    if(hideguiindex == 0)
    {
        gdk_threads_init();

        gdk_threads_enter();

        pthread_create(&gui_thread, NULL, start_gui, NULL);
        main_tcl(NULL);
        
        gdk_threads_leave();
    }

    // pthread_join(gui_thread, NULL);

    return 0;
}
