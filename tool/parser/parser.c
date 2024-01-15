// lab 2 //
#include "parser.h"
// #define DEBUG

/* #### countIOS(enum IO_STATES_CCS currentState, char *event) #### */
/* This function handles the counting of input/output (IO) states based on
   the current state and the provided event. It implements a FSM to
   transition between states based on specific conditions. */
enum IO_STATES_CCS countIOS(enum IO_STATES_CCS currentState, char *event) 
{
    switch (currentState) 
    {
        case WAIT_IO:
            if(strcmp(event, "IO:") == 0)
            {
                return NAME_IO;
            }
            else
            {
                return WAIT_IO;
            }

        case NAME_IO:
            gatepinhash_size++;
            return CCS;

        case CCS:
            return CONNECTIONS_COMP;

        case CONNECTIONS_COMP:
            if(strcmp(event, "\n") == 0 || strcmp(event, " ") == 0)
                return WAIT_IO;

            gatepinhash_size++; // increase size of gatepinhash //
            return CONNECTIONS;

        case CONNECTIONS: // if there are no connections ignore it //
            if(strcmp(event, "IO:") == 0)
            {
                return NAME_IO;
            }
            else if (strncmp(event, "#", 1) == 0)
            {
                return WAIT_IO;
            }
            else
            {
                return CONNECTIONS_COMP;
            }

        default:
            return currentState;
    }
}

/* ## count_components_CCS(enum lib_parse currentState, char *event) ## */
/* This function parses the last part of the file and it counts components
   cells and more gatepins to initialize the gatepinhash libhash and comphash */
enum lib_parse count_components_CCS(enum lib_parse currentState, char *event) 
{
    switch (currentState) 
    {
        case WAIT:
            if(strcmp(event, "Component:") == 0)
            {
                comphash_size++;
                return CELL_TYPE;
            }
            else
            {
                return WAIT;
            }

        case CELL_TYPE:
            if(strcmp(event, "Cell_Type:") == 0)
            {
                return CELL_NAME;
            }
            return CELL_TYPE;
        
        case CELL_NAME:
            add_cell(event); // add cell temporarily array //
            return WAIT_CCS;

        case WAIT_CCS:
            if (strcmp(event, "CCs:") == 0)
            {
                return OUTPUT_PIN_1;
            }
            else
            {
                return WAIT_CCS;
            }

        case OUTPUT_PIN_1:
            gatepinhash_size++;
            return OUTPUT_PIN_2;

        case OUTPUT_PIN_2:
            
            return CONNECTED_PINS;

        case CONNECTED_PINS:
            if(strcmp(event, "Component:") == 0)
                return WAIT;

            gatepinhash_size++;

            return CONNECTED_PINS_2;
            
        case CONNECTED_PINS_2:

            return CONNECTED_PINS;

        default:
            return currentState;
    }
}

/* ## proccessIOS(enum IO_STATES currentState, char *event) ## */
/* This function is a FSM that parses first part of file to get all
   all IOs and add it in gatepinhash */
enum IO_STATES proccessIOS(enum IO_STATES currentState, char *event) 
{
    switch (currentState) 
    {
        case START: // wait for word IO //
            if(strcmp(event, "IO:") == 0)
            {
                return IO_NAME;
            }
            else
            {
                return START;
            }

        case IO_NAME: // get name of IO //
            #ifdef DEBUG
            printf("Name is %s\n", event);
            #endif
            Gatepins_add(event, IO_TYPE); // add it as IO //
            
            return START;

        default:
            return currentState;
    }
}

/* ## proccessIOS_CCS(enum IO_STATES_CCS currentState, char *event) ## */
/* This function is a FSM that parses second section of file to get
   all CCs of IOs and add them in gatepinhash */
enum IO_STATES_CCS proccessIOS_CCS(enum IO_STATES_CCS currentState, char *event) 
{
    int i, j;
    int ghash, ghashdepth;
    static char IO_pin[LINE_MAX];
    static char *connection_pin;
    static int size_event;

    switch (currentState) 
    {
        case WAIT_IO: // wait for word IO //
            if(strcmp(event, "IO:") == 0)
            {
                return NAME_IO;
            }
            else
            {
                return WAIT_IO;
            }

        case NAME_IO: // get name of IO //
            #ifdef DEBUG
            printf("Name is %s\n", event);
            #endif
            strcpy(IO_pin, event);
            return CCS;

        case CCS: // just wait for word CCs //
            return CONNECTIONS_COMP;

        case CONNECTIONS_COMP: // get first part of successor gatepin //
            if(strcmp(event, "\n") == 0 || strcmp(event, " ") == 0)
                return WAIT_IO;
            size_event = strlen(event);
            connection_pin = my_calloc(strlen(event) + 1, sizeof(char));
            strcpy(connection_pin, event);

            return CONNECTIONS;

        case CONNECTIONS: // get connection pin etc. (/A) //
            j = 0; 
            for (i = 0; i < strlen(event); i++) // remove brackets from pin //
            {
                if(event[i] != '(' && event[i] != ')' && event[i] != '\n')
                {
                    event[j++] = event[i]; 
                }
            }
            event[j] = '\0';

            connection_pin = (char *) my_realloc(connection_pin, 1 * (size_event + strlen(event) + 2));
            strcat(connection_pin, event);  // concut strings together to create connection pin //
            if(strcmp(event, "IO:") == 0) // if next word is IO there are no more CCs for this pin //
            {
                free(connection_pin);
                return NAME_IO;
            }
            else if (strncmp(event, "#", 1) == 0) // if it reads '#' it is in the end of this section //
            {
                free(connection_pin);
                return WAIT_IO;
            }
            else // else add this gatepin on gatepinhash (if it does not exists) and complete CCs //
            {
                #ifdef DEBUG
                printf("Connections is %s\n", connection_pin);
                #endif
                get_gatepin_indices(connection_pin, &ghash, &ghashdepth);
                if(ghashdepth == -1)    // it does not exist in hash //
                {
                    Gatepins_add(connection_pin, WIRE);
                }
                #ifdef DEBUG
                printf("Event is %s IOPin is %s and connection_pins is %s\n", event, IO_pin, connection_pin);
                #endif
                gatepin_add_CCs(IO_pin, connection_pin);

                free(connection_pin);
                return CONNECTIONS_COMP;
            }

        default:
            return currentState;
    }
}

/* ## enum lib_parse proccessAllComponentsCCS(enum lib_parse currentState, char *event) ## */
/* This function is a FSM that parses second section of file to get
   all CCs of IOs and add them in gatepinhash */
enum lib_parse proccessAllComponentsCCS(enum lib_parse currentState, char *event)
{
    static int ghash, ghashdepth;
    int conhash, conhashdepth; // hash and depth for CCs //
    int comphash, compdepth;
    int i, j;
    static char name_of_cell[LINE_MAX]; // static in order to keep value on iterations //
    static char cell_type_name[LINE_MAX];
    static char *comp_name;
    static int cell_type = -1;
    static char *out_pin;
    static char *con_pin;
    static int new_comp = 0;
    static char *cell_pin;
    static int pin_type;

    switch (currentState) 
    {
        case WAIT: // wait for word Component: //
            if(strcmp(event, "Component:") == 0)
            {
                return COMP_NAME;
            }
            else
            {
                return WAIT;
            }

        case COMP_NAME: // keep name of component //
            #ifdef DEBUG
            printf("Comp name is %s\n", event);
            #endif
            new_comp = 0;
            out_pin = (char *) my_calloc (strlen(event) + 1, sizeof(char));
            if(event[strlen(event)-1] == ',')
            {
                event[strlen(event)-1] = '\0';
            }

            get_comphash_indices(event, &comphash, &compdepth);
            if(compdepth != -1)
            {
                comp_name = (char *) calloc(strlen(event)+1, sizeof(char));
                strcpy(comp_name, event);
                new_comp = 1; 
                return COMPONENT_2;
            }

            comp_name = (char *) calloc(strlen(event)+1, sizeof(char));
            strcpy(comp_name, event);

            return CELL_TYPE;

        case CELL_TYPE: // wait for Cell_Type //
            if(strcmp(event, "Cell_Type:") == 0)
            {
                return CELL_NAME;
            }
            else
            {
                return CELL_TYPE;
            }

        case CELL_NAME: // get cell name //
            #ifdef DEBUG
            printf("Cell Name is %s\n", event);
            #endif
            strcpy(name_of_cell, event);
            return CELL_TIMING_TYPE;

        case CELL_TIMING_TYPE: // wait for Cell_Timing_Type: //
            if(strcmp(event, "Cell_Timing_Type:") == 0)
            {
                return GET_CELL_TYPE;
            }
            else
            {
               return CELL_TIMING_TYPE;
            }

        case GET_CELL_TYPE: // get cell type //
            strcpy(cell_type_name, event);
            if(strcmp(cell_type_name, "Combinational") == 0)
            {
                cell_type = COMBINATIONAL;
            }
            else if(strcmp(cell_type_name, "Sequential") == 0)
            {
                cell_type = SEQUENTIAL;
            }

            return WAIT_CCS;

        case WAIT_CCS: // Wait for CCs //
            if (strcmp(event, "CCs:") == 0)
            {
                return OUTPUT_PIN_1;
            }
            else
            {
                return WAIT_CCS;
            }

        case OUTPUT_PIN_1: // keep name of gatepin part 1 //
            out_pin = (char *) my_realloc (out_pin, sizeof(char) * (strlen(event) + 1)); // allocate memory every time to free it in the end //
            strcpy(out_pin, event);

            return OUTPUT_PIN_2;

        case OUTPUT_PIN_2: // keep the pin name //
            j = 0; 
            for (i = 0; i < strlen(event); i++) // remove brackets from pin //
            {
                if(event[i] != '(' && event[i] != ')' && event[i] != '\n')
                {
                    event[j++] = event[i]; 
                }
            }
            event[j] = '\0';

            out_pin = (char *) my_realloc (out_pin, strlen(out_pin) + strlen(event) + 1);
            strcat(out_pin, event);

            get_gatepin_indices(out_pin, &ghash, &ghashdepth);
            if(ghashdepth == -1) // if pin does not exists, add it on gatepinhash //
            {
                Gatepins_add(out_pin, WIRE);
            }
            
            return CONNECTED_PINS;

        case CONNECTED_PINS: // get the CCs of output pin //
            if(strcmp(event, "Component:") == 0)
                return COMPONENT_2;
            
            if(strcmp(comp_name, event) == 0)
            {
                out_pin = (char *) my_realloc (out_pin, sizeof(char) * (strlen(event) + 1)); // allocate memory every time to free it in the end //
                strcpy(out_pin, event);
                
                return OUTPUT_PIN_2;
            }
            con_pin = (char *) my_calloc(strlen(event) + 1, sizeof(char));
            strcpy(con_pin, event);

            return CONNECTED_PINS_2;

        case CONNECTED_PINS_2:
            j = 0; 
            if(event[0] != '(')
            {
                printf("THIS IS ERROR\n");
            }
            for (i = 0; i < strlen(event); i++) // remove brackets from pin //
            {
                if(event[i] != '(' && event[i] != ')' && event[i] != '\n')
                {
                    event[j++] = event[i]; 
                }
            }
            event[j] = '\0';

            con_pin = (char *) my_realloc (con_pin, strlen(con_pin) + strlen(event) + 1);
            strcat(con_pin, event);

            get_gatepin_indices(con_pin, &conhash, &conhashdepth);
            if(conhashdepth == -1)
            {
                Gatepins_add(con_pin, WIRE);
            }
            gatepin_add_CCs(out_pin, con_pin); // add CCs on prev output pin //
            free(con_pin);

            return CONNECTED_PINS;

        case COMPONENT_2: // wait for second time of word Component //
            if(strcmp(event, "Pin:") == 0)
            {
                pin_type = OUTPUT;
                return PIN_OUT;
            }
            else
            {
                pin_type = INPUT;
                return COMPONENT_2;
            }

        case PIN_OUT:
            if(event[strlen(event)-1] == ',')
            {
                event[strlen(event)-1] = '\0';
            }

            cell_pin = (char *) my_calloc(strlen(event) + 2, sizeof(char));
            strcpy(cell_pin, "/");
            strcat(cell_pin, event);
            
            return FUNCTION_WAIT;
        
        case FUNCTION_WAIT:
            if(strcmp(event, "Function:") == 0)
            {
                return FUNCTION;
            }
            else
            {
                return FUNCTION_WAIT;
            }
            
        case FUNCTION: // get function of cell //
            /* add component on componenthash and inside of this function
               add also the current cell on libhash if it does not work*/
            if(new_comp == 0)
            {
                comphash_add(comp_name, name_of_cell, cell_type, event); 
            }
            if(event[strlen(event)-1] == '\n')
                event[strlen(event)-1] = '\0';
            lib_add_func(name_of_cell, event);
            lib_add_pins(name_of_cell, cell_pin, pin_type);
            free(comp_name);
            free(out_pin);
            free(cell_pin);

            return WAIT;

        default:
            return currentState;
    }
}
/* ## proccess_lib_pins_IO(enum proccess_lib_pins_IO currentState, char *event) ##*/
/* Proccess Top-Level I/O CCs to find pins for library cells that does not exists on the components CCs*/
enum proccess_lib_pins_IO proccess_lib_pins_IO(enum proccess_lib_pins_IO currentState, char *event)
{
    int i, j;
    int chash, cdepth;
    int lhash, ldepth;
    static char *comp_name;
    static char *cell_name;

    switch (currentState) 
    {
        case BEGIN_IO: // Wait for word CCs //
            if(strcmp(event, "CCs:") == 0)
            {
                return GET_COMP_NAME_IO;
            }
            else
            {
                return BEGIN_IO;
            }

        case GET_COMP_NAME_IO: // get comp name of CC pin //
            if(strcmp(event, "IO:") == 0 || strncmp(event, "#", 1) == 0 || strncmp(event, "\n", 1) == 0)
            {
                return BEGIN_IO;
            }

            comp_name = (char *) calloc(strlen(event) + 1, sizeof(char));
            strcpy(comp_name, event);

            #ifdef DEBUG
            printf("COMP_NAME is %s\n", comp_name);
            #endif
            
            get_comphash_indices(comp_name, &chash, &cdepth);
            free(comp_name);
            if(cdepth == -1)
            {
                printf("ERROR: There is not component %s\n", comp_name);
            }
            else
            {
                lhash = comphash[chash].lib_type[cdepth];   // get cell pos in libhash //
                ldepth = comphash[chash].lib_type_depth[cdepth];
                cell_name = (char *) calloc(strlen(libhash[lhash].name[ldepth])+1, sizeof(char));
                strcpy(cell_name, libhash[lhash].name[ldepth]);
            }

            return PIN_IO;

        case PIN_IO:
            j = 0; 
            for (i = 0; i < strlen(event); i++) // remove brackets from pin //
            {
                if(event[i] != '(' && event[i] != ')' && event[i] != '\n')
                {
                    event[j++] = event[i]; 
                }
            }
            event[j] = '\0';

            lib_add_pins(cell_name, event, INPUT); // add pins for current cell //
            free(cell_name);

            return GET_COMP_NAME_IO;

        default:
            return currentState;
    }
}

/* ## proccess_lib_pins(enum proccess_lib_pins currentState, char *event) ##*/
/* Proccess all gatepin CCs to find pins for each cell */
enum proccess_lib_pins proccess_lib_pins(enum proccess_lib_pins currentState, char *event) 
{
    int i, j;
    int chash, cdepth;
    int lhash, ldepth;
    static char *comp_name;
    static char *cell_name;

    switch (currentState) 
    {
        case BEGIN:
            if(strcmp(event, "CCs:") == 0)
            {
                return GET_COMP_NAME;
            }
            else
            {
                return BEGIN;
            }

        case GET_COMP_NAME:
            if(strcmp(event, "Component:") == 0)
            {
                return BEGIN;
            }

            comp_name = (char *) calloc(strlen(event) + 1, sizeof(char));
            strcpy(comp_name, event);

            #ifdef DEBUG
            printf("COMP_NAME is %s\n", comp_name);
            #endif
            
            get_comphash_indices(comp_name, &chash, &cdepth);
            if(cdepth == -1)
            {
                printf("ERROR: There is not component %s\n", comp_name);
            }
            else
            {
                lhash = comphash[chash].lib_type[cdepth];   // get cell pos in libhash //
                ldepth = comphash[chash].lib_type_depth[cdepth];
                cell_name = (char *) calloc(strlen(libhash[lhash].name[ldepth])+1, sizeof(char));
                strcpy(cell_name, libhash[lhash].name[ldepth]);
            }
            free(comp_name);

            return PIN;

        case PIN:
            j = 0; 
            for (i = 0; i < strlen(event); i++) // remove brackets from pin //
            {
                if(event[i] != '(' && event[i] != ')' && event[i] != '\n')
                {
                    event[j++] = event[i]; 
                }
            }
            event[j] = '\0';

            lib_add_pins(cell_name, event, INPUT); // add pin without brackets on cell //

            free(cell_name);
            return GET_COMP_NAME;

        default:
            return currentState;
    }
}

/* #################### print_gatepinhash() #################### */
/* This function prints all gatepis from gatepinhash with CCs and 
   parent component and also if it is WIRE or IO*/
void print_gatepinhash()
{
    int i, j, k;

    for (i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if(gatepinhash[i].type[j] == IO_TYPE)
                {
                    printf("IO: %s CCs: ", gatepinhash[i].name[j]);
                    for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
                    {
                        if(gatepinhash[gatepinhash[i].pinConn[j][k]].name[gatepinhash[i].pinConnDepth[j][k]] != NULL)
                            printf("%s ", gatepinhash[gatepinhash[i].pinConn[j][k]].name[gatepinhash[i].pinConnDepth[j][k]]);
                    }
                    printf("\n");
                }
                else if (gatepinhash[i].type[j] == WIRE)
                {
                    printf("WIRE: %s CCs: ", gatepinhash[i].name[j]);
                    for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
                    {
                        if(gatepinhash[gatepinhash[i].pinConn[j][k]].name[gatepinhash[i].pinConnDepth[j][k]] != NULL)
                            printf("%s ", gatepinhash[gatepinhash[i].pinConn[j][k]].name[gatepinhash[i].pinConnDepth[j][k]]);
                    }
                    printf("with parent: %s", comphash[gatepinhash[i].parentComponent[j]].name[gatepinhash[i].parentComponentDepth[j]]);
                    printf("\n");
                }
            }
        }
    }
}

/* ####################### print_libhash() ####################### */
/* This function prints all cells from libhash it's type and pins
   of each cell */
void print_libhash()
{
    int i, j, k;

    for (i = 0; i < libhash_size; i++)
    {
        for (j = 0; j < HASHDEPTH; j++)
        {
            if(libhash[i].name[j] != NULL)
            {
                if(libhash[i].cell_type[j] == 1)
                    printf("Cell is %s and type is Combinational with function %s\n", libhash[i].name[j], libhash[i].function[j][0]);
                else if (libhash[i].cell_type[j] == 2)
                    printf("Cell is %s and type is Sequential with function %s\n", libhash[i].name[j], libhash[i].function[j][0]);
                else
                    printf("ERROR TYPE\n");

                printf("CCs are  ");
                for(k = 0; k < libhash[i].pin_count[j]; k++)
                {
                    printf("%s ", libhash[i].pin_names[j][k]);
                }
                printf("\n");
            }
        }
    }
}

/* ####################### print_comphash() ####################### */
/* This function prints all components from componenthash */
void print_comphash()
{
    int i, j;

    for (i = 0; i < comphash_size; i++)
    {
        for (j = 0; j < HASHDEPTH; j++)
        {
            if(comphash[i].hashpresent[j] == 1)
            {
                printf("Component is %s\n", comphash[i].name[j]);
            }
        }
    }
}

/* ####################### print_cells() ####################### */
/* This function prints all cells from libarray to be sure that 
   all of them are different */
void print_cells()
{
    int i;

    for(i = 0; i < libarray_size; i++)
    {
        if(libarray[i] != NULL)
        {
            printf("Cell is %s\n", libarray[i]);
        }
    }
    printf("Size is %d\n", libarray_size);
}

/* ################### call_parser(char *input_file) ################### */
/* This function calls all the FSMs to parse the file and handle
   them correct */
int call_parser(char *input_file)
{
    FILE *filename; 
    char *line = NULL;
    char *find_line;
    int choose_FSM = 0;
    int i = 0;
    char word[LINE_MAX] = {'\0'};
    int pos = 0;
    int j = 0;
    size_t line_size = 0;
    int read_length = 0;
    int correct_format = 0;

    enum IO_STATES currentState = START;
    enum IO_STATES_CCS currentState2 = START;
    enum lib_parse currentState3 = WAIT;
    enum proccess_lib_pins currentState4 = BEGIN;
    enum proccess_lib_pins currentState5 = BEGIN_IO;

    filename = fopen(input_file, "r");  // open file only for reading //

    if(filename == NULL)
    {
        printf("Cannot open file\n");
        return -1;  // cannot open this file //
    }

    // initialize structs //
    comphash_size = 0;
    libhash_size = 0;
    gatepinhash_size = 0;
    
    // read first section of file line by line until it ends //
    while((read_length = (getline(&line, &line_size, filename) ) )!= -1)
    {
        j = 0;
        find_line = strstr(line, "Top-Level I/O CCs:");
        if(find_line != NULL)
        {
            choose_FSM = 1;  // set the choose_FSM that the following line has IO //
            correct_format = 1;
        }

        find_line = strstr(line, "Components CCs:");
        if(find_line != NULL)
        {
            choose_FSM = 3;  // set the choose_FSM that the following line has IO //
        }

        while(j < read_length) // proccess line to split it in words //
        {
            for(i = j; i < read_length+1; i++)
            {
                if(line[i] == ' ' || line[i] == '\0' || line[i] == '\v' || line[i] == '\r') // a word ends with this one //
                {
                    word[pos] = '\0';
                    break;
                }
                else
                {
                    word[pos] = line[i];
                    pos++ ;
                }
            }
            pos = 0;
            if(choose_FSM == 1)
            {
                currentState2 = countIOS(currentState2, word);
            }
            else if (choose_FSM == 3)
            {
                currentState2 = count_components_CCS(currentState2, word);
            }

            while (line[i] == ' ') 
            {
                i++;
            }
            j = i;
        }
        free(line);
        line = NULL;
    }
    if(correct_format == 0)
    {
        return -2; // check format of file //
    }

    fseek(filename, 0, SEEK_SET); // reset file descriptor to 0 to reparse file //
    free(line);
    line = NULL; // make line NULL for getline function //
    comphash_size++; // add one to comphash size because hash functions usually does not return 0 //
    libhash_size++; // add one to libhash size because hash functions usually does not return 0 //
    structs_init(); // initialize all structs with correct sizes //

    // reparse file to store gatepins components and cells //
    while((read_length = (getline(&line, &line_size, filename) ) )!= -1)
    {
        j = 0;
        find_line = strstr(line, "Top-Level I/O Ports:");
        if(find_line != NULL)
        {
            choose_FSM = 1;  // set the choose_FSM that the following line has IO //
        }

        find_line = strstr(line, "Top-Level I/O CCs:");
        if(find_line != NULL)
        {
            choose_FSM = 2;  // set the choose_FSM that the following line has IO //
        }

        find_line = strstr(line, "Components CCs:");
        if(find_line != NULL)
        {
            choose_FSM = 3;  // set the choose_FSM that the following line has IO //
        }

        while(j < read_length)
        {
            for(i = j; i < read_length+1; i++)
            {
                if(line[i] == ' ' || line[i] == '\0'|| line[i] == '\v' || line[i] == '\r')
                {
                    word[pos] = '\0';
                    break;
                }
                else
                {
                    word[pos] = line[i];
                    pos++;
                }
            }
            pos = 0;
            if(choose_FSM == 1)
            {
                currentState = proccessIOS(currentState, word);
            }
            else if (choose_FSM == 2)
            {
                currentState2 = proccessIOS_CCS(currentState2, word);
            }
            else if (choose_FSM == 3)
            {
                currentState3 = proccessAllComponentsCCS(currentState3, word);
            }

            while (line[i] == ' ') // if there are more spaces between words //
            {
                i++;
            }
            j = i;
        }
        free(line);
        line = NULL;
    }
    fseek(filename, 0, SEEK_SET); // reset file descriptor to 0 to reparse file //
    free(line);
    line = NULL;
    
    // re-parse file to add pin to cells //
    while((read_length = (getline(&line, &line_size, filename) ) )!= -1)
    {

        find_line = strstr(line, "# Top-Level I/O CCs:");
        if(find_line != NULL)
        {
            choose_FSM = 4;
        }
        j = 0;
        find_line = strstr(line, "Components CCs:");
        if(find_line != NULL)
        {
            choose_FSM = 5;
        }

        while(j < read_length)
        {
            for(i = j; i < read_length+1; i++)
            {
                if(line[i] == ' ' || line[i] == '\0'|| line[i] == '\v' || line[i] == '\r')
                {
                    word[pos] = '\0';
                    break;
                }
                else
                {
                    word[pos] = line[i];
                    pos++ ;
                }
            }
            pos = 0;
            if(choose_FSM == 4)
            {
                currentState5 = proccess_lib_pins_IO(currentState5, word);
            }
            else if (choose_FSM == 5)
            {  
                currentState4 = proccess_lib_pins(currentState4, word);
            }

            while (line[i] == ' ') 
            {
                i++;
            }
            j = i;
        }
        free(line);
        line = NULL;
    }
    free(line);
    line = NULL;

    /* it fills the parent positions in each pin, now that all
       gatepins and components are on their hashtables is easier
       to parse the gatepinhash get from name the comp_name and
       search on comphash to find hash and hashdept and store
       it in correct pin */
    gatepins_complete_parent();
    gatepin_characterize_IOs();

    #ifdef DEBUG
    print_gatepinhash();
    print_libhash();
    print_comphash();
    print_cells();
    #endif

    int count_IOs = 0;  // count IO_Pins //
    int count2 = 0; // count all pins //

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if(gatepinhash[i].type[j] == IO_TYPE || gatepinhash[i].type[j] == PO)
                {
                    count_IOs++;
                }
                count2++;
            }
        }
    }
    #ifdef DEBUG
    printf("IO pins are %d all pins are %d\n", count_IOs, count2);
    #endif

    int count = 0;
    for(i = 0; i < comphash_size; i++) // count components //
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(comphash[i].hashpresent[j] == 1)
            {
                count++;
            }
        }
    }
    int count_cells = 0;
    for(i = 0; i < libhash_size; i++) // count components //
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(libhash[i].hashpresent[j] == 1)
            {
                count_cells++;
            }
        }
    }

    #ifdef DEBUG
    printf("comps are %d\n", count);
    printf("IO pins size is %d and compsize is %d\n", gatepinhash_size, comphash_size);
    #endif

    printf(ANSI_COLOR_BLUE "####### Summary of Design: %s #######\n" ANSI_COLOR_RESET, input_file);
    printf(ANSI_COLOR_GREEN "Components: %d\n" ANSI_COLOR_RESET, count);
    printf(ANSI_COLOR_GREEN "IOs: %d\n" ANSI_COLOR_RESET, count_IOs);
    printf(ANSI_COLOR_GREEN "Cells: %d\n" ANSI_COLOR_RESET, count_cells);

    fclose(filename); // close file //

    return 0;   // exit succesfully //
}
