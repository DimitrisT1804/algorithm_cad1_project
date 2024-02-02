// Structs to store design components gatepins and library info //
/* There are three structs, one for gatepins which stores connections
of each pin, type, and in which component it exists */

#include "structs.h"

/* #################### Gatepins_init() #################### */
/* This function just initialize all fields of gatepin hash */
void Gatepins_init()
{
    int i, j;
    gatepinhash = (gatePins*) my_calloc(gatepinhash_size, sizeof(gatePins)); // size of 10 positions //

    for(i = 0; i < gatepinhash_size; i++)
    {   
        for(j = 0; j < HASHDEPTH; j++) // what is the value of hashdepth
        {
            gatepinhash[i].name[j] = NULL;
            gatepinhash[i].pinConn[j] = NULL;    // no need i do this in add //
            gatepinhash[i].pinConnDepth[j] = NULL;

            gatepinhash[i].type[j] = -1;   // init type //
            gatepinhash[i].connections_size[j] = 1;
            gatepinhash[i].parentComponent[j] = -1;
            gatepinhash[i].parentComponentDepth[j] = -1;
            gatepinhash[i].hashpresent[j] = 0;
        }
    }
}

/* #################### Gatepins_free() #################### */
/* This function free all fields of this struct */
void Gatepins_free()
{
    int i, j;

    for(i = 0; i < gatepinhash_size; i++)
    {   
        for(j = 0; j < HASHDEPTH; j++) 
        {
            if(gatepinhash[i].name[j] != NULL)
            {
                free(gatepinhash[i].name[j]);
                free (gatepinhash[i].pinConn[j]);
                free (gatepinhash[i].pinConnDepth[j]);
            }
        }
    }
    free(gatepinhash);
}

/* ##### Gatepins_add(char *pin_name, int pin_type) ##### */
/* This function adds in gatepinhash a pin (its name and the type) */
void Gatepins_add(char *pin_name, int pin_type)
{
    int i;
    unsigned int key;

    if(pin_name == NULL)
        return;

    key = hash_function(pin_name, gatepinhash_size); // hash again string to get same key //

    for(i = 0; i < HASHDEPTH; i++)
    {
        if(gatepinhash[key].hashpresent[i] == 0)    // empty //
            break;
    }
    gatepinhash[key].name[i] = (char *) my_calloc(1, (strlen(pin_name) + 1) );

    strcpy(gatepinhash[key].name[i], pin_name);

    /* Calloc for the new pin size for each connections, 1 for begin and go on*/
    gatepinhash[key].pinConn[i] = (int *) my_calloc(1, sizeof(int));
    gatepinhash[key].pinConnDepth[i] = (int *) my_calloc(1, sizeof(int));

    gatepinhash[key].connections_size[i] = 0;
    gatepinhash[key].type[i] = pin_type;
    gatepinhash[key].hashpresent[i] = 1;

    #ifdef DEBUG
    printf("Gatepin inserted succesfully\n");
    #endif
}

/* ## get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth) ### */
/* This function search gatepinhash for this pin and returns value 
   of position in ghash and ghashdepth. If it does not find it 
   it returns ghashdepth = -1 */
void get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth)
{
    int i;
    unsigned int key;
    key = hash_function(pin_name, gatepinhash_size);
    *ghash = key;
    *ghashdepth = -1;   // add this value if it does not find pin //

    for(i = 0; i < HASHDEPTH; i++)
    {
        if(gatepinhash[*ghash].name[i] != NULL)
        {
            if(strcmp(gatepinhash[*ghash].name[i], pin_name) == 0)
            {
                *ghashdepth = i;
                break;
            }
        }
    }
}

/* #### gatepin_add_CCs(char *source_pin, char *connection_pin) #### */
/* This function adds connections for each gatepin. It keeps an array
   of gatepinhash and gatepinhashdepth which are positions in gatepinhash */
void gatepin_add_CCs(char *source_pin, char *connection_pin)
{
    int source_hash, source_hash_depth;
    int connection_hash, connection_hash_depth;
    int size_of_connections = 0;

    if(source_pin == NULL || connection_pin == NULL)
        return;

    get_gatepin_indices(source_pin, &source_hash, &source_hash_depth);
    get_gatepin_indices(connection_pin, &connection_hash, &connection_hash_depth);

    if(source_hash_depth == -1 || connection_hash_depth == -1)
    {
        printf("Source or Connection pin does not exists!\n");
        return;
    }

    if(gatepinhash[source_hash].pinConnDepth[source_hash_depth] == NULL)
    {
        printf("Invalid pointer\n");
    }

    gatepinhash[source_hash].pinConn[source_hash_depth] = (int*) my_realloc(gatepinhash[source_hash].pinConn[source_hash_depth], sizeof(int) * (gatepinhash[source_hash].connections_size[source_hash_depth] + 1));
    gatepinhash[source_hash].pinConnDepth[source_hash_depth] = (int*) my_realloc(gatepinhash[source_hash].pinConnDepth[source_hash_depth], sizeof(int) * (gatepinhash[source_hash].connections_size[source_hash_depth] + 1));
    
    size_of_connections = gatepinhash[source_hash].connections_size[source_hash_depth]; // get size of connections for current pin //

    gatepinhash[source_hash].pinConn[source_hash_depth][size_of_connections] = connection_hash;
    gatepinhash[source_hash].pinConnDepth[source_hash_depth][size_of_connections] = connection_hash_depth;

    gatepinhash[source_hash].connections_size[source_hash_depth]++;
}

/* ################ gatepins_complete_parent() ################ */
/* This function adds the component that his pin belongs. This function
   runs when all structs are full, so it searches in gatepinhash it
   gets the pin name and it keeps the component name and then finds it
   in comphash and adds it as parent on gatepinhash
   For Example: gatepin is PID/U1288/Q we just keep PID/U1288 search
   it on comphash and get chash and cdepth and add it on gatepinhash*/
void gatepins_complete_parent()
{
    int i, j, k;
    char *comp_name = NULL;
    int chash, cdepth;

    for (i = 0; i < gatepinhash_size; i++)
    {
        for (j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0  && gatepinhash[i].type[j] == WIRE)
            {
                for (k = strlen(gatepinhash[i].name[j]); gatepinhash[i].name[j][k] != '/'; k--)
                {

                }
                comp_name = (char *) calloc(k+1, sizeof(char));
                strncpy(comp_name, gatepinhash[i].name[j], k);
                get_comphash_indices(comp_name, &chash, &cdepth);
                
                free(comp_name);
                if(cdepth != -1)
                {
                    gatepinhash[i].parentComponent[j] = chash;
                    gatepinhash[i].parentComponentDepth[j] = cdepth;
                }
            }
        }
    }
}

void gatepin_characterize_IOs()
{
    int i, j, k;
    char *comp_name;
    int chash, cdepth;
    int lhash, ldepth;
    int m;
    int curr_ghash, curr_gdepth;

    for (i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0 && gatepinhash[i].type[j] == IO_TYPE)
            {
                if(gatepinhash[i].connections_size[j] <= 0)
                {
                    break;
                }
                for(m = 0; m < gatepinhash[i].connections_size[j]; m++)
                {
                    curr_ghash = gatepinhash[i].pinConn[j][m];
                    curr_gdepth = gatepinhash[i].pinConnDepth[j][m];

                    chash = gatepinhash[curr_ghash].parentComponent[curr_gdepth];
                    cdepth = gatepinhash[curr_ghash].parentComponentDepth[curr_gdepth];
                    comp_name = (char *) calloc(strlen(comphash[chash].name[cdepth]) + 1, sizeof(char));
                    strcpy(comp_name, comphash[chash].name[cdepth]);
                    
                    lhash = comphash[chash].lib_type[cdepth];
                    ldepth = comphash[chash].lib_type_depth[cdepth];
                    for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                    {
                        if(libhash[lhash].pin_type[ldepth][k] == OUTPUT)
                        {
                            comp_name = (char *) realloc(comp_name, strlen(comphash[chash].name[cdepth]) + strlen(libhash[lhash].pin_names[ldepth][k]) + 2);
                            strcat(comp_name, libhash[lhash].pin_names[ldepth][k]);

                            if(strcmp(comp_name, gatepinhash[curr_ghash].name[curr_gdepth]) == 0)
                            {
                                gatepinhash[i].type[j] = PO;
                            }
                        }
                        
                    }
                    free(comp_name);
                }
                // ghash = gatepinhash[i].pinConn[j][0];
                // gdepth = gatepinhash[i].pinConnDepth[j][0];
                // for (k = strlen(gatepinhash[ghash].name[gdepth]); gatepinhash[ghash].name[gdepth][k] != '/'; k--)
                // {

                // }
                // comp_name = (char *) calloc(k+1, sizeof(char));
                // strncpy(comp_name, gatepinhash[ghash].name[gdepth], k);

                // while(gatepinhash[ghash].name[gdepth][k] != '\0')
                // {
                //     pin_name[pos] = gatepinhash[ghash].name[gdepth][k];
                //     pos++;
                //     k++;
                // }
                // pin_name[pos] = '\0';
                // pos = 0;
                // #ifdef DEBUG
                // printf("name is %s\n", pin_name);
                // #endif

                // get_comphash_indices(comp_name, &chash, &cdepth);

                // free(comp_name);
                // if(cdepth != -1)
                // {
                //     lhash = comphash[chash].lib_type[cdepth];
                //     ldepth = comphash[chash].lib_type_depth[cdepth];

                //     for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                //     {
                //         if(strcmp(pin_name, libhash[lhash].pin_names[ldepth][k]) == 0)
                //         {
                //             if(libhash[lhash].pin_type[ldepth][k] == OUTPUT)
                //             {
                //                 gatepinhash[i].type[j] = PO;
                //             }
                //         }
                //     }
                // }

            }
        }
    }
}

/* #### hash_function(const char *str, unsigned int num_buckets) #### */
/* This is a hash function that gets as input number of backets of
   one hash and it returns a key */
unsigned int hash_function(const char *str, unsigned int num_buckets) 
{
    unsigned long hash_value = 5381; // Initial value

    while (*str != '\0') 
    {
        hash_value = (hash_value * 33) ^ (unsigned long)(*str);
        str++;
    }

    // Use modulo to restrict the hash value to the range [0, num_buckets-1]
    return (unsigned int)(hash_value % num_buckets);
}

/* ######################## Lib_init() ######################## */
/* This function initializes all values for library hash table */
void Lib_init()
{
    int i, j;
    libhash = (Lib*) my_calloc(libhash_size, sizeof(Lib));

    for(i = 0; i < libhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            libhash[i].name[j] = NULL;
            libhash[i].function[j] = NULL;
            libhash[i].pin_names[j] = NULL;
            libhash[i].pin_count[j] = 0;
            libhash[i].hashpresent[j] = 0;           
            libhash[i].cell_type[j] = -1;
            libhash[i].out_pins_count[j] = 0;
            libhash[i].pin_type[j] = NULL;
        }
    }
}

/* ## Lib_add(char *cell_name, int cell_type, char *func_expr) ## */
/* This function adds in libhash for each cell the name of
   cell the cell_type (Combinational or Sequential) */
void Lib_add(char *cell_name, int cell_type, char *func_expr)
{
    int i;
    unsigned int key;
    int lhash, ldepth;

    if(cell_name == NULL)
        return;

    key = hash_function(cell_name, libhash_size); // rehash cell_name to get pos of it in libhash //

    get_libhash_indices(cell_name, &lhash, &ldepth);
    if(ldepth == -1)
    {
        for (i = 0; i < HASHDEPTH; i++)
        {
            if(libhash[key].hashpresent[i] == 0)
                break;
        }
        libhash[key].name[i] = (char *) my_calloc((strlen(cell_name) + 1), sizeof(char));
        strcpy(libhash[key].name[i], cell_name);

        libhash[key].cell_type[i] = cell_type;
        libhash[key].hashpresent[i] = 1;

        ldepth = i;
    }
    #ifdef DEBUG
    printf("Cell inserted succesfully on libhash\n");
    #endif
}

/* ## get_libhash_indices(char *cell_name, int *lhash, int *lhashdepth) ## */
/* This function retrieves the hash index and depth for a given cell_name
   in the libhash data structure. The hash index represents the position
   in libhash where information about the specified cell is stored, and
   the depth represents the specific slot within that position. */
void get_libhash_indices(char *cell_name, int *lhash, int *lhashdepth)
{
    int i;
    unsigned int key;
    key = hash_function(cell_name, libhash_size);
    *lhash = key;

    *lhashdepth = -1; // initialize depth to -1, indicating cell not found in libhash //

    for(i = 0; i < HASHDEPTH; i++)
    {
        if(libhash[*lhash].hashpresent[i] != 0)
        {
            if(strcmp(libhash[*lhash].name[i], cell_name) == 0)
            {
                *lhashdepth = i;
                break;
            }
        }
    }    
}

/* ######### lib_add_pins(char *cell_name, char *pin_name) ######### */
/* This function adds pin information to the libhash data structure for
   a specified cell. It checks for duplicate pin names and ensures that
   the pin information is appropriately stored in the libhash. */
void lib_add_pins (char *cell_name, char *pin_name, int pin_type)
{
    int i;
    int lhash, ldepth;

    if(cell_name == NULL)
    {
        return;
    }

    // Retrieve the hash index and depth for the specified cell_name //
    get_libhash_indices(cell_name, &lhash, &ldepth);
    if(ldepth == -1)
    {
        printf("ERROR: There is not this cell\n");
        return;
    }

     // Check for duplicate pin names in the specified cell //
    for(i = 0; i < libhash[lhash].pin_count[ldepth]; i++)
    {
        if(libhash[lhash].pin_names[ldepth][i] != NULL)
        {
            if (strcmp(libhash[lhash].pin_names[ldepth][i], pin_name) == 0 )
            {
                return; // already exists //
            }
        }
    }

    // Add the new pin name at position i //
    libhash[lhash].pin_count[ldepth]++ ;
    libhash[lhash].pin_names[ldepth] = (char **) realloc(libhash[lhash].pin_names[ldepth], (libhash[lhash].pin_count[ldepth]) * sizeof(char *));
    libhash[lhash].pin_names[ldepth][libhash[lhash].pin_count[ldepth] - 1] = (char *) calloc(strlen(pin_name) + 1, sizeof(char));

    strcpy(libhash[lhash].pin_names[ldepth][libhash[lhash].pin_count[ldepth] - 1], pin_name);

    libhash[lhash].pin_type[ldepth] = (int *) realloc(libhash[lhash].pin_type[ldepth], (libhash[lhash].pin_count[ldepth]) * sizeof(int));
    libhash[lhash].pin_type[ldepth][libhash[lhash].pin_count[ldepth] - 1] = pin_type;
}

/* ######### lib_add_func(char *cell_name, char *func_expr) ######### */
/* This function adds function to libhash structure for
   a specified cell. It checks for duplicate functions and ensures that
   the function information is appropriately stored in the libhash. */
void lib_add_func(char *cell_name, char *func_expr, char *cell_pin)
{
    int i;
    int lhash, ldepth;

    get_libhash_indices(cell_name, &lhash, &ldepth);
    if(ldepth == -1)
        return;
    
    for (i = 0; i < libhash[lhash].out_pins_count[ldepth]; i++)
    {
        if (strcmp(libhash[lhash].function[ldepth][i], func_expr) == 0)     // what if two pins have same function //
        {
            return;
        } 
    }

    libhash[lhash].out_pins_count[ldepth]++;
    libhash[lhash].function[ldepth] = (char **) my_realloc(libhash[lhash].function[ldepth], sizeof(char*) * (libhash[lhash].out_pins_count[ldepth] + 1));
    libhash[lhash].function[ldepth][i] = (char *) calloc(strlen(func_expr) + 1, sizeof(char)); 
    strcpy(libhash[lhash].function[ldepth][i], func_expr);
}

/* ######################### libhash_free() ######################### */
/* This function frees the memory allocated for the libhash data structure,
   including names, functions, and pin information for each cell */
void libhash_free()
{
    int i, j, k;

    for(i = 0; i < libhash_size; i++)
    {   
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(libhash[i].hashpresent[j] != 0) // Check if the slot is occupied //
            {
                free(libhash[i].name[j]);
                for(k = 0; k < libhash[i].out_pins_count[j]; k++)
                {
                    free(libhash[i].function[j][k]);
                }
                free(libhash[i].function[j]);

                for(k = 0; k < libhash[i].pin_count[j]; k++) // Iterate through each pin in the current cell //
                {
                    free(libhash[i].pin_names[j][k]);
                }
                free(libhash[i].pin_names[j]);
                free(libhash[i].pin_type[j]);
            }
        }
    }
    free(libhash); // Free the memory allocated for the entire libhash data structure //
}

/* #################### add_cell(char *cell_name) #################### */
/* This function adds a unique cell_name to an array (libarray), which keeps track
   of all unique cell names. The use of it is to find the exact size
   for libhash */
void add_cell(char *cell_name)
{
    int i;

    for(i = 0; i < libarray_size; i++)
    {
        if(libarray[i] != NULL)
        {
            if (strcmp(cell_name, libarray[i]) == 0)
            {
                return;
            }
        }
    }
    // Reallocate memory for libarray to accommodate the new cell_name //
    libarray = (char **) my_realloc(libarray, sizeof(char *) * (libarray_size+1));
    libarray[i] = (char *) my_calloc(strlen(cell_name) + 1, sizeof(char));
    strcpy (libarray[i], cell_name);

    libarray_size++; // update size of this array //
    libhash_size++; // update size of libhash //
}

/* ##################### comphash_init() ##################### */
/* This function initializes the comphash data structure, which is used
   for storing components. It allocates memory and initializes each slot
   in the comphash with default values */
void comphash_init()
{
    int i, j;

    comphash = (Components*) my_calloc(comphash_size, sizeof(Components));
    for (i = 0; i < comphash_size; i++)
    {
        for (j = 0; j < HASHDEPTH; j++)
        {
            comphash[i].name[j] = NULL;
            comphash[i].lib_type[j] = -1;
            comphash[i].lib_type_depth[j] = -1;  
            comphash[i].hashpresent[j] = 0;  
        }
    }
}

/* ## comphash_add(char *comp_name, char *cell_name, int cell_type, char *func_expr) ## */
/* This function adds component information to the comphash data structure.
   It associates a component name with a cell from the libhash, storing
   additional information like the libhash indices and depth. If the
   cell does not exist in libhash, it adds the cell using Lib_add function. */
void comphash_add(char *comp_name, char *cell_name, int cell_type, char *func_expr)
{
    int i;
    unsigned int key;
    int lhash, ldepth;

    if (comp_name == NULL)
    {
        return;
    }
    key = hash_function(comp_name, comphash_size);

    for (i = 0; i < HASHDEPTH; i++)
    {
        if(comphash[key].hashpresent[i] == 0) // slot is empty //
        {
            break;
        }
    }
    
    if(i == HASHDEPTH)
    {
        printf("No space!\n");
        return; // no space available in comphash //
    }

    // Allocate memory for the component name and copy it to comphash //
    comphash[key].name[i] = (char *) calloc(strlen(comp_name) + 1, sizeof(char));
    strcpy(comphash[key].name[i], comp_name);

    comphash[key].hashpresent[i] = 1;
    #ifdef DEBUG
    printf("Comp inserted succesfully\n");
    #endif

    // Check if the cell already exists in libhash //
    get_libhash_indices(cell_name, &lhash, &ldepth);
    if(ldepth != -1)
    {
        comphash[key].lib_type[i] = lhash;
        comphash[key].lib_type_depth[i] = ldepth;
        
        return;
    }

    // If the cell does not exist in libhash, add it using Lib_add function //
    Lib_add(cell_name, cell_type, func_expr);
    get_libhash_indices(cell_name, &lhash, &ldepth);

    // Update the lib_type and lib_type_depth in comphash //
    comphash[key].lib_type[i] = lhash;
    comphash[key].lib_type_depth[i] = ldepth;
}

/* ## get_comphash_indices(char *comp_name, int *chash, int *chashdepth) ## */
/* This function retrieves the hash index and depth for a given component name
   in the comphash data structure. The hash index represents the position
   in comphash where information about the specified component is stored,
   and the depth represents the specific slot within that position. */
void get_comphash_indices(char *comp_name, int *chash, int *chashdepth)
{
    int i;
    unsigned int key;

    key = hash_function(comp_name, comphash_size);
    *chash = key;

    *chashdepth = -1; // Initialize depth to -1, indicating component not found in comphash //

    // Search through each slot in the hash index for the specified component name //
    for (i = 0; i < HASHDEPTH; i++)
    {
        if(comphash[key].name[i] != NULL)
        {
            if(strcmp(comphash[*chash].name[i], comp_name) == 0)
            {
                *chashdepth = i;
                break;
            }
        }
    }
}

/* ####################### comphash_free() ####################### */
/* This function frees the memory allocated for the comphash data structure,
   including component names, within each slot of the comphash. */
void comphash_free()
{
    int i, j;

    for(i = 0; i < comphash_size; i++)
    {   
        for(j = 0; j < HASHDEPTH; j++) 
        {
            if(comphash[i].hashpresent[j] != 0)
            {
                free(comphash[i].name[j]);
            }
        }
    }
    free(comphash);
}

/* ######################## structs_init() ######################## */
/* This function initializes the various data structures used in the program,
   including gatepinhahs, libhash, and comphash. It calls individual initialization
   functions for each data structure. */
void structs_init()
{
    Gatepins_init();
    Lib_init();
    comphash_init();
    gatepinhashVisited_init();
}

/* ######################## structs_free() ######################## */
/* This function frees the memory allocated for various data structures used
   in the program, including gatepinhash, libhash, and comphash. It calls
   individual free functions for each data structure. */
void structs_free()
{
    int i;

    Gatepins_free();
    libhash_free();
    comphash_free();
    gatepinhashVisited_free();

    for(i = 0; i < libarray_size; i++)
    {
        if(libarray[i] != NULL)
        {
            free(libarray[i]);
        }
    }
    free(libarray);

    gatepinhash = NULL;
    comphash = NULL;
    libhash = NULL;
    gatepinhashv = NULL;

    libarray = NULL;
    libarray_size = 0;
}

void gatepinhashVisited_init()
{
    int i, j;
    gatepinhashv = (GatepinhashVisited*) my_calloc(gatepinhash_size, sizeof(GatepinhashVisited)); // size of 10 positions //

    for(i = 0; i < gatepinhash_size; i++)
    {   
        for(j = 0; j < HASHDEPTH; j++) // what is the value of hashdepth
        {
            gatepinhashv[i].isVisited[j] = 0;
            gatepinhashv[i].level[j] = -1;
        }
    }
}

void gatepinhashVisited_make_visited(int ghash, int gdepth)
{
    gatepinhashv[ghash].isVisited[gdepth] = 1;
}

void gatepinhashVisited_remove_visited(int ghash, int gdepth)
{
    gatepinhashv[ghash].isVisited[gdepth] = 0;
}

void gatepinhashVisited_free()
{
    free(gatepinhashv);
}

void get_predecessors_pin(char *gatepin, int *ghash, int *gdepth)
{
    int i;
    int j;
    int k;
    int con_ghash;
    int con_gdepth;

    *ghash = -1;
    *gdepth = -1;
    if(gatepin == NULL)
    {
        return;
    }

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] == 1)
            {
                for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
                {
                    con_ghash = gatepinhash[i].pinConn[j][k];
                    con_gdepth = gatepinhash[i].pinConnDepth[j][k];
                    if(strcmp(gatepinhash[con_ghash].name[con_gdepth], gatepin) == 0)
                    {
                        *ghash = i;
                        *gdepth = j;
                        return;
                    }
                }
            }
        }
    }
}

int check_gatepin_type(int ghash, int gdepth)
{
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    int k;
    char *curr_pin = NULL;

    chash = gatepinhash[ghash].parentComponent[gdepth];
    cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];
    lhash = comphash[chash].lib_type[cdepth];
    ldepth = comphash[chash].lib_type_depth[cdepth];


    for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
    {
        curr_pin = (char *) calloc(strlen(comphash[chash].name[cdepth]) + 1 + strlen(libhash[lhash].pin_names[ldepth][k]), sizeof(char));
        strcpy(curr_pin, comphash[chash].name[cdepth]);
        strcat(curr_pin, libhash[lhash].pin_names[ldepth][k]);
        if(strcmp(gatepinhash[ghash].name[gdepth], curr_pin) == 0)
        {
            free(curr_pin);
            curr_pin = NULL;
            break;
        }
        free(curr_pin);
        curr_pin = NULL;
    }

    if(libhash[lhash].pin_type[ldepth][k] == OUTPUT)
    {
        return 1;
    }
    else if(libhash[lhash].pin_type[ldepth][k] == INPUT)
    {
        return 0;
    }
    else
    {
        return -1;
    }


    return -1;
}